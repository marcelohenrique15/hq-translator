#include "struct/struct_data.h"
#include "translation/LibreTranslate.h"
#include "threads/translate_thread.h"

#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <nlohmann/json.hpp>

using namespace std;

// --- CONSTRUTOR (Implementação) ---
// Esta é a implementação do construtor da classe `TranslateThread`. Ele usa a lista
// de inicialização de membros (`: ai(api), ...`) para configurar o estado inicial do
// objeto. Cada membro privado é inicializado com as referências para os recursos
// compartilhados (API, filas, mutexes, etc.) fornecidos durante a criação do objeto.
// Isso garante que a thread de tradução tenha tudo o que precisa para operar
// corretamente antes mesmo de ser iniciada.

TranslateThread::TranslateThread(
    LibreTranslateAPI& api,
    queue<std::vector<textData>>& inputQueue,
    vector<textData>& outputQueue,
    mutex& inputMtx,
    mutex& outputMtx,
    condition_variable& cond
) :
    ai(api),
    toTranslateQueue(inputQueue),
    translatedText(outputQueue),
    toTranslateMutex(inputMtx),
    translatedMutex(outputMtx),
    condition(cond),
    running(false){} // A thread começa no estado "não executando".

// --- MÉTODOS PÚBLICOS (Implementação) ---
// Os métodos `start`, `stop` e `join` formam a interface pública para controlar
// o ciclo de vida da thread, encapsulando os detalhes da sua gestão.

// Inicia a execução da thread.
void TranslateThread::start()
{
    running = true; // Sinaliza que o loop principal deve começar.
    // Cria a thread, associando-a ao método privado 'run'.
    thread = std::thread(&TranslateThread::run, this);
}

// Sinaliza para a thread parar sua execução de forma segura.
void TranslateThread::stop()
{
    running = false; // Altera a flag para que o loop 'while' em 'run()' termine.
    condition.notify_one(); // Acorda a thread caso ela esteja dormindo, para que possa verificar a flag 'running' e encerrar.
}

// Espera a thread terminar sua execução.
void TranslateThread::join()
{
    // Garante que a thread pode ser "joined" (está em execução).
    if(thread.joinable())
    {
        // Bloqueia a thread chamadora até que esta thread termine.
        thread.join();
    }
}

// --- MÉTODO PRIVADO (Lógica Principal da Thread) ---
// O método 'run' contém o trabalho que a thread executa continuamente.
// Ele consome dados da fila de entrada, processa-os (traduzindo o texto)
// e armazena o resultado final.
void TranslateThread::run()
{
    // O loop principal continua enquanto 'running' for verdadeiro.
    while (running) 
    {
        vector<textData> toTranslate; // Vetor local para guardar os dados a serem traduzidos.

        // --- SEÇÃO CRÍTICA: CONSUMIR DA FILA DE ENTRADA ---
        {
            unique_lock<mutex> lock(toTranslateMutex);
            
            // Espera eficientemente até que a fila de entrada tenha dados
            // ou até que a thread seja sinalizada para parar.
            condition.wait(lock, [&]() {
                return !toTranslateQueue.empty() || !running;
            });

            // Se a thread foi acordada para parar e a fila está vazia, sai do loop.
            if(!running && toTranslateQueue.empty()) break;
    
            // Remove o item da fila de forma segura, pois o 'lock' ainda está ativo.
            toTranslate = toTranslateQueue.front();
            toTranslateQueue.pop();
        } // O 'lock' da fila de entrada é liberado aqui.

        // --- PROCESSAMENTO (TRADUÇÃO E CACHE) ---
        // Itera sobre cada item de texto detectado para traduzi-lo.
        for (auto& dt : toTranslate)
        {
            // PASSO 1: Verificar o cache.
            // Procura o texto original no nosso cache de traduções.
            auto it = translation_cache.find(dt.original);

            // PASSO 2: Agir com base no resultado do cache.
            if (it != translation_cache.end()) // Cache HIT: O texto já foi traduzido antes.
            {
                dt.translated = it->second; // Usa o resultado do cache, evitando uma chamada de API.
            } 
            else // Cache MISS: É a primeira vez que vemos este texto.
            {
                // Chama a API para traduzir o texto.
                string result = ai.translateText(dt.original, "en", "pt");
                dt.translated = result; // Armazena o resultado.
                translation_cache[dt.original] = result; // **Adiciona o novo resultado ao cache para uso futuro.**
            }
        }

        // --- SEÇÃO CRÍTICA: ATUALIZAR O RESULTADO FINAL ---
        {
            lock_guard<mutex> lock(translatedMutex);
            // Atualiza o vetor de saída com os dados recém-traduzidos.
            // Note que aqui estamos substituindo (=) o conteúdo, não adicionando.
            // Isso significa que o vetor de saída sempre conterá apenas o resultado do último lote processado.
            translatedText = toTranslate;
        }
        
        // Esta notificação acorda uma thread que espera na mesma variável de condição.
        // Neste design específico, ela notificaria a si mesma na próxima iteração,
        // o que pode não ser o comportamento desejado, mas não causa problemas.
        condition.notify_one();
    }
}