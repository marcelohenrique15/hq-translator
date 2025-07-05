// Garante que o cabeçalho só sera incluído uma vez durante a compilação
#pragma once


#include <opencv2/opencv.hpp> // Inclui a biblioteca OpenCV para manipulação de imagens
#include <iostream> // Inclui a biblioteca iostream para entrada e saída de dados

// --- CLASSE ---
// A classe WebcamCapture é um modelo ou projeto que define a estrutura e o comportamento
// de objetos que interagem com uma webcam. Ela agrupa dados (atributos) e funções (métodos)
// relacionadas à captura de vídeo, promovendo organização e reutilização de código.

class WebcamCapture
{

    // --- ENCAPSULAMENTO (Membro Privado) ---
    // 'cap' é um membro privado. Isso significa que ele só pode ser acessado diretamente
    // pelos métodos dentro da própria classe WebcamCapture. Essa restrição protege o
    // estado interno do objeto, impedindo que seja modificado indevidamente por código externo.
    // O acesso e a manipulação de 'cap' são controlados pelos métodos públicos da classe.

    private:
        cv::VideoCapture cap; // Objeto cv::VideoCapture para controlar a webcam.

    public:

        // --- CONSTRUTOR ---
        // O construtor é um método especial que é chamado automaticamente quando um objeto
        // da classe WebcamCapture é criado. Ele é responsável por inicializar o estado do objeto,
        // neste caso, abrindo a conexão com a webcam e configurando suas propriedades (largura, altura, FPS).

        WebcamCapture(int cam = cv::CAP_V4L2, int width = 1280, int height = 720, double fps = 60.0);

        // ---DESTRUTOR ---
        // O destrutor é outro método especial, chamado automaticamente quando um objeto
        // da classe é destruído (por exemplo, quando sai do escopo ou é explicitamente deletado).
        // Sua função principal é liberar os recursos alocados pelo objeto durante sua vida útil,
        // garantindo que a câmera seja liberada (`cap.release()`) e evitando vazamento de recursos.

        ~WebcamCapture();

        // --- MÉTODOS PÚBLICOS ---
        // 'isOpened()' é um método público que fornece uma interface para o mundo externo
        // verificar se a webcam está aberta. Como é 'const', ele garante que não modificará
        // o estado interno do objeto.

        bool isOpened() const; // 'const' indica que o método não modifica o estado do objeto.

        // --- MÉTODOS PÚBLICOS ---
        // 'readFrame()' é um método público que permite a leitura de um frame da webcam.
        // Ele interage com o membro privado 'cap' para realizar a operação. A passagem de
        // 'cv::Mat& frame' por referência otimiza o desempenho e permite que o método
        // preencha o objeto 'frame' fornecido externamente.

        bool readFrame(cv::Mat& frame);
};

