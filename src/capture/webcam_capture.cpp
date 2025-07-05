// Inclui o cabeçalho da classe WebcamCapture que define a interface da captura de vídeo.
#include "capture/webcam_capture.h"

// Usa o namespace std para evitar escrever std:: antes de funções como cout
using namespace std;

// --- CONSTRUTOR (Implementação) ---
// Esta é a implementação do construtor da classe WebcamCapture.
// Ele é chamado quando um objeto WebcamCapture é criado e é responsável por INICIALIZAR
// o estado desse objeto. A parte `: cap(cam)` é uma "lista de inicialização de membros"
// e é a forma mais eficiente de inicializar o membro 'cap' (o objeto cv::VideoCapture)
// com o índice da câmera fornecido. Dentro do construtor, são realizadas as configurações
// da câmera (codec, largura, altura, FPS).

WebcamCapture::WebcamCapture(int cam, int width, int height, double fps) : cap(cam)
{
    // Verifica se a câmera foi aberta com sucesso
    if(!cap.isOpened())
    {
        cout << "Erro ao ligar a câmera.\n";
        return;
    }
    // Define o codec de vídeo MJPG (Motion-JPEG) para melhor desempenho
    int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    cap.set(cv::CAP_PROP_FOURCC, fourcc);
    // Define a largura da imagem capturada
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    // Define a altura da imagem capturada
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    // Define o número de frames por segundo (FPS) da captura
    cap.set(cv::CAP_PROP_FPS, fps);
}

// --- DESTRUTOR (Implementação) ---
// Esta é a implementação do destrutor da classe WebcamCapture.
// Ele é chamado automaticamente quando um objeto WebcamCapture é DESTRUÍDO (por exemplo,
// quando sai do escopo). Sua principal responsabilidade é realizar a "limpeza" dos
// recursos alocados pelo objeto. Aqui, ele garante que a conexão com a webcam (`cap`)
// seja liberada (`cap.release()`), evitando vazamentos de recursos ou bloqueios da câmera.

WebcamCapture::~WebcamCapture()
{

    // Libera a câmera se ela ainda estiver aberta
    if(cap.isOpened())
    {
        cap.release();
    }
}

// --- MÉTODO PÚBLICO (Implementação de isOpened) ---
// Esta é a implementação do método público 'isOpened()'.
// Ele permite que qualquer parte do código que tenha um objeto WebcamCapture
// verifique se a câmera foi aberta com sucesso e está pronta para uso.
// A implementação acessa o membro privado 'cap' para consultar seu estado,
// demonstrando como os métodos públicos interagem com os dados encapsulados.

bool WebcamCapture::isOpened() const
{
    return cap.isOpened();
}

// --- MÉTODO PÚBLICO (Implementação de readFrame) ---
// Esta é a implementação do método público 'readFrame()'.
// Ele é a interface para capturar um novo frame da webcam.
// A implementação utiliza o membro privado 'cap' para realizar a leitura,
// demonstrando novamente o **encapsulamento**: os detalhes de como o frame
// é lido (chamando `cap.read(frame)`) estão contidos dentro deste método da classe.

bool WebcamCapture::readFrame(cv::Mat& frame)
{
    return cap.read(frame);
}