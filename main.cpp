#include "capture/webcam_capture.h"  // Inclui a definição da classe para captura da webcam.
#include "detection/text_detector.h" // Inclui a definição da classe para detecção de texto.
#include "overlayer/overlay_renderer.h" // Inclui a definição da classe para renderização de overlay.
#include "translation/LibreTranslate.h" // Inclui a definição da classe para a API de tradução.

#include "threads/webcam_thread.h"  // Inclui a definição da thread de captura da webcam.
#include "threads/translate_thread.h" // Inclui a definição da thread de tradução.
#include "threads/detection_thread.h" // Inclui a definição da thread de detecção de texto.

#include <string> // Para manipulação de strings.
#include <iostream>  // Para entrada e saída de dados no console.
#include <vector>  // Para o uso de vetores dinâmicos.
#include <sstream> // Para manipulação de streams de string.
#include <chrono> // Para medição de tempo e controle de intervalos.
#include <thread> // Para o uso de threads.
#include <unordered_map> // Para o uso de mapas hash (ex: para cache de tradução).
#include <queue> // Para filas thread-safe.
#include <mutex> // Para controle de acesso a recursos compartilhados entre threads.
#include <condition_variable> // Para sincronização de threads.
#include <atomic> // Para variáveis atômicas (controle de estado das threads).

using namespace std; // Usa o namespace padrão C++.
using namespace cv; // Usa o namespace OpenCV para funções e classes.
using namespace std::chrono; // Usa o namespace chrono para funções de tempo.

// --- INSTANCIAÇÃO DE OBJETOS E COLABORAÇÃO ---
// A função 'main' é o ponto de entrada do programa e o ORQUESTRADOR principal.
// Ela demonstra um conceito fundamental da POO: a **criação de objetos**
// a partir das classes definidas (`WebcamCapture`, `Overlay`, `Detector`, `LibreTranslateAPI`,
// `WebcamThread`, `DetectionThread`, `TranslateThread`).
//
// Mais importante, 'main' mostra a **colaboração entre esses objetos**.
// Cada objeto é uma instância de uma classe com uma responsabilidade específica,
// e eles trabalham juntos (interagindo via filas e métodos) para alcançar
// o objetivo geral do programa (captura, detecção, tradução e exibição).
// Isso é a essência de um sistema orientado a objetos bem projetado.

int main() {

    // --- INSTANCIAÇÃO DE OBJETOS ---
    // Criação de instâncias das classes principais. Cada variável é um objeto
    // com seus próprios dados e comportamentos, conforme definido por sua classe.
 
    WebcamCapture webcam; // Objeto para capturar vídeo da webcam.
    Overlay overlayer; // Objeto para desenhar overlays na imagem (usado via método estático).
    Detector tesseract; // Objeto para detecção de texto (OCR).
    LibreTranslateAPI ai("http://libretranslate_api:5000/"); // Objeto para comunicação com a API LibreTranslate.

    // Verifica se a webcam foi inicializada corretamente.
    if (!webcam.isOpened()) { // Chamada de método público de 'webcam' (objeto 'WebcamCapture').
        cerr << "Camera nao foi iniciada corretamente.\n";  // Mensagem de erro.
        return 0; // Sai do programa se a câmera não abrir.
    }

    // ========== FILAS E SINCRONIZAÇÃO ==========
    // Estas são as estruturas de dados compartilhadas que permitem a comunicação
    // e sincronização entre os diferentes objetos de thread.
    // Elas são passadas como referências para os construtores das threads,
    // permitindo a colaboração segura.

    // Fila para frames da webcam: armazena os frames capturados.

    queue<Mat> frameQueue;
    mutex frameMutex; // Mutex para proteger o acesso à frameQueue.
    condition_variable frameCond; // Variável de condição para sinalizar novos frames.

    // Fila para textos a serem traduzidos: armazena os textos detectados pelo OCR.
    queue<vector<textData>> toTranslateQueue;
    mutex toTranslateMutex; // Mutex para proteger o acesso à toTranslateQueue.
    condition_variable translationCond; // Variável de condição para sinalizar novos textos para tradução.

    // Vetor para textos traduzidos: armazena os textos após a tradução.
    vector<textData> translatedText;
    mutex translatedMutex; // Mutex para proteger o acesso ao translatedText.

    // ========== THREADS ==========

    // --- INSTANCIAÇÃO E COLABORAÇÃO COM THREADS ---
    // Criação de objetos de thread, cada um encapsulando uma operação específica
    // (captura, detecção, tradução). Eles recebem as referências para os objetos
    // de funcionalidade (webcam, tesseract, ai) e as filas/mutexes/condições
    // para estabelecer a comunicação e sincronização.
    
    // Cria e inicia a thread de captura da webcam.

    WebcamThread webcamThread(webcam, frameQueue, frameMutex, frameCond);
    webcamThread.start(); // Chamada de método público 'start()' do objeto 'webcamThread'.

    // Cria e inicia a thread de detecção de texto (OCR).
    DetectionThread detectionThread(tesseract, frameQueue, toTranslateQueue, frameMutex, toTranslateMutex, frameCond, translationCond);
    detectionThread.start();

    // Cria e inicia a thread de tradução.
    TranslateThread translateThread(ai, toTranslateQueue, translatedText, toTranslateMutex, translatedMutex, translationCond);
    translateThread.start();

    // ========== LOOP PRINCIPAL ==========

    bool running = true; // Flag para controlar o loop principal.
    long long last_detection_time = 0; // Tempo da última detecção (não usado diretamente no loop principal atual, mas pode ser para controle de frequência).
    const long long detection_interval_ms = 500; // Intervalo de 500ms (originalmente para controle de OCR).

    // Loop principal de exibição e overlay.
    while (running) {
        Mat frame; // Variável para armazenar o frame atual.

        // Bloco protegido por mutex para acessar a fila de frames.
        {
            unique_lock<mutex> lock(frameMutex); // Adquire o lock.
            // Espera até que a fila de frames não esteja vazia.
            frameCond.wait(lock, [&]() { return !frameQueue.empty(); });
            frame = frameQueue.front(); // Pega o frame da frente.
            frameQueue.pop(); // Remove o frame da fila.
        }

        // Lógica de tempo para detecção (mantida, mas o controle de frequência está nas threads).
        long long current_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if (current_time - last_detection_time >= detection_interval_ms) {
            last_detection_time = current_time;
        }

        // Atualiza e obtém os textos traduzidos.
        vector<textData> detected_text;
        {
            lock_guard<mutex> lock(translatedMutex); // Adquire o lock para acessar translatedText.
            detected_text = translatedText; // Adquire o lock para acessar translatedText.
        }

        // --- USO DE MÉTODO ESTÁTICO E AGRUPAMENTO DE DADOS (textData) ---
        // Aqui o método estático da classe 'Overlay' é chamado para desenhar no frame.
        // O `dt.translated` e `dt.box` são exemplos de como a 'struct textData'
        // (que é uma forma de encapsulamento de dados) é utilizada para agrupar
        // informações relacionadas e passá-las de forma coesa.

        for (const auto& dt : detected_text) {
            cout << dt.translated << endl; // Imprime o texto traduzido no console.
            Overlay::drawTextOverlay(frame, dt.translated, dt.box); // Desenha o overlay no frame.
        }

        imshow("Webcam", frame); // Exibe o frame na janela "Webcam".

        // Verifica se a tecla ESC foi pressionada para sair.
        if (waitKey(1) == 27) {  // waitKey(1) espera 1ms por uma tecla; 27 é o código ASCII para ESC.
            running = false; // Define a flag para sair do loop.
        }
    }

    // ========== FINALIZAÇÃO ==========

    // --- DESTRUIÇÃO CONTROLADA DE OBJETOS ---
    // A fase final demonstra o gerenciamento do ciclo de vida dos objetos.
    // Os métodos 'stop()' são chamados para sinalizar o término das threads de forma segura,
    // e 'join()' é usado para garantir que cada thread complete sua execução antes que
    // o programa finalize. Isso garante que os recursos sejam liberados corretamente.
    
    // Para e sincroniza a thread da webcam.

    webcamThread.stop(); // Sinaliza para a thread parar.
    frameCond.notify_all(); // Notifica todas as threads esperando em frameCond para que possam verificar a flag 'running'.
    webcamThread.join(); // Espera a thread da webcam finalizar.

    // Para e sincroniza a thread de detecção.
    detectionThread.stop(); // Sinaliza para a thread parar.
    frameCond.notify_all(); // Notifica novamente, caso a thread de detecção estivesse esperando por frames.
    detectionThread.join(); // Espera a thread de detecção finalizar.

    // Para e sincroniza a thread de tradução.
    translateThread.stop(); // Sinaliza para a thread parar.
    translationCond.notify_all(); // Notifica todas as threads esperando em translationCond.
    translateThread.join(); // Espera a thread de tradução finalizar.

    destroyAllWindows(); // Fecha todas as janelas OpenCV.
    return 0; // Retorna 0 indicando sucesso.
}
