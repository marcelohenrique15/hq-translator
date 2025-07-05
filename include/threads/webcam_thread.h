#pragma once // Garante que este cabeçalho seja incluído apenas uma vez.

#include <thread> // Para manipulação de threads.
#include <atomic>  // Para variáveis atômicas (garante operações seguras em ambiente multi-threaded).
#include <queue> // Para o uso de filas thread-safe.
#include <mutex> // Para exclusão mútua (proteção de acesso a dados compartilhados).
#include <condition_variable> // Para sincronização de threads (espera/notificação).
#include <opencv2/opencv.hpp> // Inclui a biblioteca OpenCV para manipulação de imagens (cv::Mat).
#include "capture/webcam_capture.h" // Inclui o cabeçalho da classe WebcamCapture.

// --- CLASSE ---
// A classe 'WebcamThread' serve como um molde para criar objetos que encapsulam
// toda a lógica e os recursos necessários para capturar frames de uma webcam
// em uma thread de execução separada. Ela centraliza a gestão da thread,
// da fila de saída dos frames e dos mecanismos de sincronização.

class WebcamThread {

public:
    // --- CONSTRUTOR ---
    // O construtor é um método especial que é invocado automaticamente quando um objeto
    // 'WebcamThread' é criado. Ele é responsável por inicializar o estado do objeto,
    // recebendo referências para a instância da `WebcamCapture` (que gerencia a câmera)
    // e para a fila, o mutex e a variável de condição que serão usados para a comunicação
    // inter-thread. Isso garante que o objeto esteja configurado e pronto para operar.

    WebcamThread(WebcamCapture& capture,
                 std::queue<cv::Mat>& outputQueue,
                 std::mutex& mtx,
                 std::condition_variable& cond);

   // --- MÉTODOS PÚBLICOS ---
    // `start()`, `stop()`, e `join()` são os métodos **públicos** da classe.
    // Eles formam a interface através da qual outras partes do programa podem
    // interagir e controlar o ciclo de vida da thread de captura. Eles expõem
    // as ações que podem ser realizadas no objeto `WebcamThread`, enquanto
    // os detalhes de como essas ações são executadas (internamente, pelo método `run()`)
    // são encapsulados.

    void start();
    void stop();
    void join();

private:

    // --- ENCAPSULAMENTO (Método Privado) ---
    // O método `run()` é **privado**. Ele contém a lógica principal do que a thread
    // de captura fará em seu ciclo de vida (ler frames da webcam e colocá-los na fila).
    // Sendo privado, ele não pode ser chamado diretamente de fora da classe,
    // garantindo que o ciclo de vida da thread seja gerenciado exclusivamente através
    // dos métodos públicos (`start()`, `stop()`, `join()`). Isso protege a integridade
    // e o comportamento esperado da thread.

    void run();

    // --- ENCAPSULAMENTO (Atributos Privados) ---
    // Todos os membros declarados aqui são **privados**. Isso é uma demonstração clara
    // do **encapsulamento**. O estado interno do objeto `WebcamThread` (como a referência
    // para a `WebcamCapture`, a fila de frames, o mutex, a variável de condição,
    // o objeto `std::thread` e a flag de controle `running`) é protegido. Ele só pode
    // ser acessado e modificado pelos métodos da própria classe. Isso previne o acesso
    // direto e potencialmente inconsistente de código externo, promovendo a integridade
    // dos dados e o controle sobre a thread.
 
    WebcamCapture& cam; // Referência para o objeto WebcamCapture.
    std::queue<cv::Mat>& frameQueue; // Referência para o objeto WebcamCapture.
    std::mutex& queueMutex; // Mutex para proteger o acesso à frameQueue.
    std::condition_variable& condition; // Variável de condição para sinalizar novos frames.
    std::atomic<bool> running; // Flag atômica para controlar o estado de execução da thread.
    std::thread thread; // Objeto std::thread que representa a thread de execução.
};
