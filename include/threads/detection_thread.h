#pragma once // Garante que este cabeçalho seja incluído apenas uma vez.

#include <thread> // Para manipulação de threads.
#include <atomic> // Para variáveis atômicas (garante operações seguras em ambiente multi-threaded).
#include <queue> // Para o uso de filas thread-safe.
#include <mutex> // Para exclusão mútua (proteção de acesso a dados compartilhados).
#include <condition_variable> // Para exclusão mútua (proteção de acesso a dados compartilhados).

#include <opencv2/opencv.hpp> // Funções básicas do OpenCV.
#include <tesseract/baseapi.h> // API base do Tesseract OCR.
#include <leptonica/allheaders.h> // Leptonica, biblioteca auxiliar do Tesseract.
#include <opencv2/imgproc.hpp> // Funções de processamento de imagem do OpenCV.

#include <string>  // Para manipulação de strings.
#include <vector> // Para o uso de vetores.
#include <iostream> // Para entrada/saída (debug).

#include "detection/text_detector.h" // Inclui o cabeçalho da classe Detector.
#include "struct/struct_data.h" // Inclui a definição da estrutura de dados para texto.

// --- CLASSE ---
// A classe 'DetectionThread' é um modelo para criar objetos que encapsulam
// toda a lógica e os recursos necessários para executar a detecção de texto
// (usando um 'Detector') em uma thread de execução separada.
// Ela centraliza a gestão da thread, das filas de comunicação e da sincronização.

class DetectionThread
{
    public:

        // --- CONSTRUTOR ---
        // O construtor é um método especial que inicializa o estado de um objeto 'DetectionThread'
        // quando ele é criado. Ele recebe referências para o motor OCR ('Detector') e para
        // as filas, mutexes e variáveis de condição que serão usadas para a comunicação
        // inter-thread. Isso garante que o objeto esteja pronto para operar assim que instanciado.

        DetectionThread(Detector& tesseract,
                        std::queue<cv::Mat>& inputQueue,
                        std::queue<std::vector<textData>>& outputQueue,
                        std::mutex& inputMtx,
                        std::mutex& outputMtx,
                        std::condition_variable& inputCond,
                        std::condition_variable& outputCond);

        // --- MÉTODOS PÚBLICOS ---
        // 'start()', 'stop()', e 'join()' são métodos públicos que definem a interface
        // para controlar o ciclo de vida da thread de detecção. Eles expõem as ações
        // que podem ser realizadas no objeto 'DetectionThread', enquanto os detalhes
        // de como essas ações são executadas (por exemplo, dentro de 'run()') são encapsulados.

        // Inicia a thread de detecção.
        void start();
        // Sinaliza para a thread de detecção parar.    
        void stop();
        // Espera a thread de detecção finalizar sua execução.
        void join();

    private:

        // --- ENCAPSULAMENTO (Método Privado) ---
        // O método 'run()' é **privado**. Ele contém a lógica principal que será executada
        // pela thread de detecção. Sendo privado, ele não pode ser chamado diretamente de fora da classe,
        // garantindo que a thread seja iniciada e gerenciada apenas através dos métodos públicos ('start').
        // Isso protege a integridade do ciclo de vida da thread.

        void run();

        // --- ENCAPSULAMENTO (Atributos Privados) ---
        // Todos os membros abaixo são **privados**. Isso demonstra o encapsulamento,
        // garantindo que o estado interno da thread (como as filas, os mutexes e a flag de
        // controle) só possa ser acessado e modificado através da interface pública da classe
        // ('start()', 'stop()'). Isso previne o uso incorreto e condições de corrida (race conditions).


        Detector& ai; // Referência ao motor de detecção de texto (Tesseract/Detector).
        std::queue<cv::Mat>& toDetectQueue; // Referência à fila de entrada, de onde a thread consome os frames (cv::Mat) a serem processados.
        std::queue<std::vector<textData>>& detectedQueue; // Referência à fila de saída, onde a thread publica os resultados da detecção.
        std::mutex& toDetectMutex; // Referência ao mutex que protege a fila de entrada ('toDetectQueue') contra acesso concorrente.
        std::mutex& detectedMutex; // Referência à variável de condição para a fila de entrada. Usada para "acordar" esta thread quando um novo frame chega.
        std::condition_variable& inCondition; // Referência à variável de condição para a fila de saída. Usada para "acordar" outra thread (consumidora) quando um novo resultado está pronto.
        std::condition_variable& outCondition;

        // --- Membros de Controle Interno ---
        // Estes membros são gerenciados exclusivamente pela própria classe para controlar seu estado e ciclo de vida.


        std::atomic<bool> running; // Flag atômica que controla o loop principal no método 'run()'. Sendo atômica, pode ser modificada de forma segura pela thread principal (via 'stop()') e lida pela thread de detecção.
        std::thread thread; // O objeto de thread que efetivamente representa e executa a tarefa em segundo plano (o método 'run()').
};