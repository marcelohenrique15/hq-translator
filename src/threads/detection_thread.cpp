#include "threads/detection_thread.h" // Inclui o cabeçalho para a classe DetectionThread.
#include "struct/struct_data.h" // Inclui a estrutura de dados personalizada para informações de texto.
#include "detection/text_detector.h" // Inclui o cabeçalho para a classe Detector (wrapper do Tesseract OCR).

#include <thread> // Para gerenciamento de threads.
#include <atomic> // Para operações atômicas (ex: controle do estado de execução da thread).
#include <queue> // Para operações de fila (seguras para threads).
#include <mutex> // Para exclusão mútua (travamento) para proteger dados compartilhados.
#include <condition_variable> // Para sincronização de threads baseada em condições.

using namespace std; // Usa o namespace padrão C++.
using namespace std::chrono; // Usa o namespace chrono para funções relacionadas ao tempo.

// --- CONSTRUTOR (Implementação) ---
// Esta é a implementação do construtor da classe `DetectionThread`.
// Ele é chamado quando um objeto `DetectionThread` é criado. Sua função é **inicializar
// todos os atributos privados** do objeto usando a "lista de inicialização de membros"
// (`: ai(tesseract), toDetectQueue(inputQueue), ...`). Isso garante que o objeto seja
// configurado corretamente com as referências para o `Detector`, as filas de comunicação
// (entrada e saída), os mutexes e as variáveis de condição, antes que qualquer método
// da classe seja chamado. Isso é crucial para a segurança e o correto funcionamento
// da comunicação entre threads.

DetectionThread::DetectionThread(
    Detector& tesseract, // Referência para a instância do Detector (OCR).
    queue<cv::Mat>& inputQueue, // Fila de entrada para frames a serem detectados.
    queue<std::vector<textData>>& outputQueue, // Fila de saída para dados de texto detectados.
    mutex& inputMtx, // Mutex para proteger a fila de entrada (toDetectQueue).
    mutex& outputMtx, // Mutex para proteger a fila de saída (detectedQueue).
    condition_variable& inputCond, // Variável de condição para sinalizar novos frames na fila de entrada.
    condition_variable& outputCond // Variável de condição para sinalizar novos textos detectados na fila de saída.
) :
    ai(tesseract), // Inicializa a referência 'ai' com o Detector fornecido.
    toDetectQueue(inputQueue), // Inicializa a referência 'toDetectQueue'.
    detectedQueue(outputQueue), // Inicializa a referência 'detectedQueue'.
    toDetectMutex(inputMtx),  // Inicializa a referência 'toDetectMutex'.
    detectedMutex(outputMtx), // Inicializa a referência 'detectedMutex'.
    inCondition(inputCond), // Inicializa a referência 'inCondition'.
    outCondition(outputCond), // Inicializa a referência 'outCondition'.
    running(false){} // Estado inicial: thread não está rodando.

// --- MÉTODOS PÚBLICOS (Implementação de start) ---
// Este é a implementação do método público `start()`. Ele demonstra como o controle
// do ciclo de vida da thread é exposto de forma controlada. Ao chamar `start()`,
// a flag `running` é ativada e o método **privado** `run()` é iniciado em uma nova thread.
// Isso mostra o **encapsulamento**: o usuário da classe apenas chama `start()`,
// sem se preocupar com os detalhes de como a thread é criada ou qual método ela executa.

void DetectionThread::start()
{
    running = true; // Define a flag 'running' como verdadeira.

    // Cria e inicia uma nova thread que executará o método 'run' deste objeto.
    thread = std::thread(&DetectionThread::run, this);
}

// --- MÉTODOS PÚBLICOS (Implementação de stop) ---
// Implementação do método público `stop()`. Ele fornece uma forma segura e controlada
// de sinalizar para a thread em execução que ela deve parar. A alteração da flag
// `running` atômica é um mecanismo de comunicação thread-safe.

void DetectionThread::stop()
{
    running = false; // Define a flag 'running' como falsa, sinalizando para a thread terminar.
    inCondition.notify_one(); // Acorda a thread caso ela esteja esperando na condição, para que ela possa verificar a flag 'running' e sair.
}

// --- MÉTODOS PÚBLICOS (Implementação de join) ---
// Implementação do método `join()`. Sua função é bloquear a thread que o chamou
// (geralmente a thread principal) até que esta thread de detecção tenha concluído
// sua execução. Isso é fundamental para garantir um encerramento limpo do programa.
void DetectionThread::join()
{
    // Verifica se a thread pode ser "joined" (ou seja, se ela está em execução e não foi destacada).
    // Esta é uma verificação de segurança para evitar exceções.
    if(thread.joinable())
    {
        thread.join(); // Bloqueia e espera a thread terminar.
    }
}

// --- MÉTODO PRIVADO (Implementação de run) ---
// Este é o método principal executado pela thread. Ele contém um loop infinito
// que implementa o padrão de design "Produtor-Consumidor". A thread "consome"
// frames de `toDetectQueue` e "produz" resultados em `detectedQueue`.
void DetectionThread::run()
{
    // O loop continua enquanto a flag 'running' for verdadeira.
    while(running)
    {
        cv::Mat toDetect; // Variável local para armazenar o frame a ser processado.

        // --- SEÇÃO CRÍTICA: CONSUMIR DA FILA DE ENTRADA ---
        {
            // `unique_lock` adquire o mutex. Ele é mais flexível que `lock_guard` e é
            // necessário para trabalhar com variáveis de condição.
            unique_lock<mutex> lock(toDetectMutex);

            // `inCondition.wait()` é o ponto chave da sincronização:
            // 1. Libera o 'lock' e coloca a thread para "dormir" de forma eficiente (sem gastar CPU).
            // 2. A thread só acorda quando `inCondition.notify_one()` ou `notify_all()` é chamado por outra thread.
            // 3. Ao acordar, ela tenta readquirir o 'lock' e verifica a condição lambda `[&](){...}`.
            // 4. A condição verifica se a fila não está vazia OU se a thread foi sinalizada para parar.
            //    Isso evita "despertas espúrias" e garante uma saída limpa do loop.
            inCondition.wait(lock, [&](){
                return !toDetectQueue.empty() || !running;
            });

            // Se o loop foi interrompido (running = false) e a fila está vazia, sai do loop principal.
            if(!running && toDetectQueue.empty()) break;

            // Retira o item da fila de forma segura, pois o lock ainda está ativo.
            toDetect = toDetectQueue.front();
            toDetectQueue.pop();
        } // O `lock` é liberado automaticamente quando o escopo `{}` termina.

        // --- PROCESSAMENTO (FORA DA SEÇÃO CRÍTICA) ---
        // A detecção de texto (operação demorada) é feita aqui, fora do lock,
        // para não impedir que a thread produtora adicione novos frames à fila.
        vector<textData> detectedText = ai.detect_text_box(toDetect);

        // --- SEÇÃO CRÍTICA: PRODUZIR NA FILA DE SAÍDA ---
        {
            // `lock_guard` adquire o mutex da fila de saída. É mais simples e eficiente
            // quando não precisamos de uma variável de condição.
            lock_guard<mutex> lock(detectedMutex);
            detectedQueue.push(detectedText); // Adiciona o resultado à fila de saída.
        } // O `lock` é liberado automaticamente no fim do escopo.

        // Notifica uma thread que possa estar esperando por resultados na fila de saída.
        outCondition.notify_one();

        // Pausa a thread por um curto período. Isso pode ser útil para evitar que a thread
        // consuma 100% de um núcleo da CPU em um loop muito rápido, cedendo tempo de processamento.
        this_thread::sleep_for(milliseconds(30));
    }
}