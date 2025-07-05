#pragma once // Garante que este cabeçalho seja incluído apenas uma vez.

#include <thread> // Para manipulação de threads.
#include <atomic> // Para variáveis atômicas (garante operações seguras em ambiente multi-threaded).
#include <queue> // Para o uso de filas thread-safe.
#include <mutex> // Para exclusão mútua (proteção de acesso a dados compartilhados).
#include <condition_variable> // Para sincronização de threads (espera/notificação).
#include <nlohmann/json.hpp> // Inclui a biblioteca Nlohmann JSON (pode ser usada internamente pelo LibreTranslateAPI).

#include "translation/LibreTranslate.h" // Inclui o cabeçalho da classe LibreTranslateAPI.
#include "struct/struct_data.h" // Inclui a definição da estrutura de dados para texto.

// --- CLASSE ---
// A classe 'TranslateThread' é um modelo para criar objetos que encapsulam
// toda a lógica e os recursos necessários para executar a tradução de texto
// em uma thread de execução separada. Ela centraliza a gestão da thread,
// das filas de comunicação, da sincronização e de um cache de tradução.

class TranslateThread
{
public:

    // --- ONSTRUTOR ---
    // O construtor é um método especial que é invocado automaticamente quando um objeto
    // 'TranslateThread' é criado. Ele é responsável por inicializar o estado do objeto,
    // recebendo referências para a API de tradução (`LibreTranslateAPI`) e para os recursos
    // de comunicação inter-thread (filas, mutexes, variável de condição). Isso garante que
    // o objeto esteja configurado e pronto para operar assim que instanciado.

    TranslateThread(LibreTranslateAPI& api, 
                    std::queue<std::vector<textData>>& inputQueue,
                    std::vector<textData>& outputQueue,
                    std::mutex& inputMtx,
                    std::mutex& outputMtx,
                    std::condition_variable& cond);

    // --- MÉTODOS PÚBLICOS ---
    // `start()`, `stop()`, e `join()` são os métodos **públicos** da classe.
    // Eles formam a interface através da qual outras partes do programa podem
    // interagir e controlar o ciclo de vida da thread de tradução. Eles expõem
    // as ações que podem ser realizadas no objeto `TranslateThread`, enquanto
    // os detalhes de como essas ações são executadas (internamente, pelo método `run()`)
    // são escondidos.

    void start();
    void stop();
    void join();

private:

    // --- ENCAPSULAMENTO (Método Privado) ---
    // O método `run()` é **privado**. Ele contém a lógica principal do que a thread
    // de tradução fará em seu ciclo de vida. Sendo privado, ele não pode ser chamado
    // diretamente de fora da classe, garantindo que o ciclo de vida da thread seja
    // gerenciado exclusivamente através dos métodos públicos (`start()`, `stop()`, `join()`).
    // Isso protege a integridade e o comportamento esperado da thread.

    void run();

    // --- ENCAPSULAMENTO (Atributos Privados) ---
    // Todos os membros declarados aqui são **privados**. Isso reforça o encapsulamento,
    // garantindo que o estado interno da thread (como as filas, o cache e a flag de controle)
    // seja acessado e modificado apenas através da interface pública bem-definida da classe (`start()`, `stop()`).
    // Isso previne o uso incorreto e condições de corrida (race conditions).

    // --- Membros Compartilhados (Referências) ---
    // Referências a objetos externos que permitem a comunicação com outras threads.

    LibreTranslateAPI& ai; // Referência ao motor de tradução (LibreTranslate).

    std::queue<std::vector<textData>>& toTranslateQueue; // Referência à fila de entrada, de onde a thread consome os dados de texto detectados.
    std::vector<textData>& translatedText; // Referência ao vetor de saída, onde a thread acumula todos os resultados já traduzidos.
    std::mutex& toTranslateMutex; // Referência ao mutex que protege a fila de entrada ('toTranslateQueue').
    std::mutex& translatedMutex; // Referência ao mutex que protege o vetor de saída ('translatedText').
    std::condition_variable& condition; // Referência à variável de condição usada para notificar esta thread de que há novos itens na fila de entrada.

    // --- Membros de Controle e Estado Interno ---
    // Atributos gerenciados pela própria classe para seu funcionamento.

    std::thread thread; // O objeto que representa a thread de execução real.
    std::atomic<bool> running; // Flag atômica para controlar o loop de execução no método 'run()'. Garante um encerramento seguro da thread.

    // Cache para armazenar traduções já realizadas e evitar chamadas repetidas à API.
    // A chave (key) é o texto original (std::string) e o valor (value) é o texto traduzido (std::string).
    // O uso de `unordered_map` oferece uma busca com complexidade média de O(1), sendo muito eficiente.
    
    std::unordered_map<std::string, std::string> translation_cache;
};
