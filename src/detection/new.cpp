#include "detection/new.h"

using namespace tesseract;

bool Detector::is_alphanumeric(const string& text)
{
    if (text.empty()) return false;

    for (char c : text) 
    {
        // Aceita letras, números, espaços, ponto, vírgula, hífen e barra
        if (!isalnum(c) && !isspace(c) && c != '.' && c != ',' && c != '-' && c != '/') 
        {
            return false;
        }
    }

    return true;
}

Mat Detector::preprocess_image(const Mat& frame)
{
    // Passa o frame de colorido para escala de cinza
    Mat gray_frame;
    cvtColor(frame, gray_frame, COLOR_BGR2GRAY);

    // Clona o frame cinza para um novo espaço na memória
    // para que possa se processador sem afetar o frame original,
    // assim pode-se evitar bugs.
    Mat processed_frame = gray_frame.clone();

    // Suaviza a imagem, diminuindo pequenos ruídos e preservando bordas.
    medianBlur(processed_frame, processed_frame, 3);

    // Transforma a imagem em preto e branco (0 ou 255), e utiliza
    // a média Gaussiana para definir o limite dos pixels vizinhos 
    // que serão detectados, alterar esses valores (21, 5)          --> 21 = limite da vizinhança, 5 = subtrai em 5 a média da vizinhança
    // pode ajudar ou piorar a leitura de dados                     --> Resultado: Letra preta em fundo branco.
    adaptiveThreshold(processed_frame, processed_frame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 21, 5);

    return processed_frame;
}

vector<Detector::TextDetection> Detector::detect_text_box(const Mat& frame)
{
    Mat processed_frame = preprocess_image(frame);

    unique_ptr<TessBaseAPI> ocr(new TessBaseAPI());
    ocr->Init("/usr/local/share/tessdata/", "eng", OEM_LSTM_ONLY);

}