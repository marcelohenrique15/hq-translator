#include "threads/webcam_thread.h" // Inclui o cabeçalho da classe correspondente.
#include "capture/webcam_capture.h" // Inclui o cabeçalho da classe que gerencia a webcam.

#include <thread> // Para manipulação de threads.
#include <atomic> // Para variáveis atômicas (flag de controle 'running').
#include <queue> // Para a fila de frames.
#include <mutex> // Para proteger o acesso à fila.
#include <condition_variable> // Para notificar outras threads sobre novos frames.
#include <chrono> // Para a função de 'sleep' (controlar a taxa de frames).

using namespace std;
using namespace std::chrono;

// --- CONSTRUTOR (Implementação) ---
// O construtor inicializa a thread da webcam com todas as dependências necessárias.
// Ele utiliza a lista de inicialização de membros para configurar as referências
// ao objeto de captura da webcam, à fila de saída de frames e aos mecanismos de
// sincronização (mutex e variável de condição) que são compartilhados com as
// threads consumidoras.

WebcamThread::WebcamThread(
    WebcamCapture& capture, 
    queue<cv::Mat>& outputQueue, 
    mutex& mtx, 
    condition_variable& cond
): 
    cam(capture), // Inicializa a referência para o objeto de captura.
    frameQueue(outputQueue), // Inicializa a referência para a fila de frames.
    queueMutex(mtx), // Inicializa a referência para o mutex da fila.
    condition(cond), // Inicializa a referência para a variável de condição.
    running(false) {} // A thread começa no estado "não executando".

// --- MÉTODOS PÚBLICOS (Implementação) ---
// Interface pública para controlar o ciclo de vida da thread.

// Inicia a thread de captura da webcam.
void WebcamThread::start() 
{
    running = true; // Ativa a flag para o loop principal começar.
    // Cria e inicia a thread, que executará o método privado 'run'.
    thread = std::thread(&WebcamThread::run, this);
}

// Sinaliza para a thread parar de capturar frames.
void WebcamThread::stop() 
{
    running = false; // Desativa a flag, fazendo com que o loop em 'run' termine.
}

// Espera a thread finalizar completamente sua execução.
void WebcamThread::join() 
{
    // Verifica se a thread está em um estado que permite o join.
    if (thread.joinable()) 
    {
        // Bloqueia a thread atual até que a thread da webcam termine.
        thread.join();
    }
}

// --- MÉTODO PRIVADO (Lógica Principal da Thread) ---
// O método 'run' é o coração da classe. Ele contém o loop que continuamente
// captura frames da webcam e os coloca na fila compartilhada.
void WebcamThread::run() {
    // Continua executando enquanto a flag 'running' for verdadeira.
    while (running) {
        cv::Mat frame; // Matriz para armazenar o frame capturado.

        // Tenta ler um novo frame da câmera.
        // Se a leitura falhar (ex: câmera desconectada), o 'continue' pula para a próxima iteração do loop.
        if (!cam.readFrame(frame)) continue;

        // --- SEÇÃO CRÍTICA: PRODUZIR NA FILA ---
        {
            // `lock_guard` garante que o mutex seja travado ao entrar neste escopo
            // e liberado automaticamente ao sair, protegendo a fila contra acesso concorrente.
            lock_guard<mutex> lock(queueMutex);
            frameQueue.push(frame); // Adiciona (produz) o novo frame na fila.
        }

        // --- NOTIFICAÇÃO AO CONSUMIDOR ---
        // Notifica uma das threads que está esperando (ex: a thread de detecção)
        // que um novo frame está disponível na fila.
        condition.notify_one();

        // --- CONTROLE DE TAXA DE FRAMES (FPS) ---
        // Pausa a thread por 30 milissegundos. Isso limita a taxa de captura a
        // aproximadamente 33 frames por segundo (1000ms / 30ms), evitando o uso
        // desnecessário de 100% da CPU.
        this_thread::sleep_for(milliseconds(30));
    }
}