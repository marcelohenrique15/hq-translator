#include "detection/text_detector.h"

using namespace tesseract;
using namespace cv;
using namespace std;

bool Detector::is_alphanumeric(const string& text)
{
    if (text.empty()) return false;

    for (char c : text) 
    {
        // Aceita letras, números, espaços, ponto, vírgula, hífen e barra
        if (!isalnum(c) && !isspace(c) && c != '.' && c != ',' && c != '-' && c != '?' && c != '!') 
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
    adaptiveThreshold(processed_frame, processed_frame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 25, 5);

    // imshow("tesseract", processed_frame);

    return processed_frame;
}

vector<Detector::TextDetection> Detector::detect_text_box(const Mat& frame)
{
    vector<TextDetection> detected_texts;   // Serivrá para guardar os textos detectados e suas boxes em um array.
    Mat processed_frame = preprocess_image(frame);

    unique_ptr<TessBaseAPI> ocr(new TessBaseAPI());                     // Cria um objeto único (ocr) para acessar a API do Tesseract
    ocr->Init("/usr/local/share/tessdata/", "eng", OEM_LSTM_ONLY);   // Escolhe o arquivo do modelo de IA treinada para detecção de texto
    ocr->SetPageSegMode(PSM_AUTO);                                      // Informa como os textos serão lidos (por palavra, por bloco, etc.)

    // Passa a lista de caracteres que será permitido ser lido pelo Tesseract
    ocr->SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,.!? ");

    // Passa todos os dados da matriz do frame para ser lido pelo Tesseract
    ocr->SetImage(processed_frame.data, 
        processed_frame.cols, 
        processed_frame.rows, 
        processed_frame.channels(), 
        processed_frame.step
    );

    // Serve para iterar os dados lidos pelo Tesseract (palavras, linhas, coordenadas, etc.)
    ocr->GetUTF8Text();
    ResultIterator* data_read = ocr->GetIterator();
    // Se houver dados, começa a filtrar e retornar as strings que queremos
    if(data_read != nullptr)                               
    {
        do 
        {
            const char* word = data_read->GetUTF8Text(RIL_TEXTLINE);        // Lê o texto por linhas.
            // Se houver linhas de texto e elas tiverem mais de 2 caracteres:
            if(word != nullptr && strlen(word) > 2)
            {
                float conf = data_read->Confidence(RIL_TEXTLINE); // Calcula a confiança da linha coletada
                // Se a confiança for maior que 60, a palavra não for um espaço em branco e tiver apenas caracteres
                // alfanuméricos (e pontuações), segue em frente.
                if(conf > 70 && word[0] != ' ' && is_alphanumeric(string(word)))
                {
                    int x1, y1; //Canto superior esquerdo da tela
                    int x2, y2; //Canto inferior direito da tela
                    data_read->BoundingBox(RIL_TEXTLINE, &x1, &y1, &x2, &y2); //Pede as coordenadas da caixa ao redor do texto detectado
                    Rect bbox(x1, y1, x2 - x1, y2 - y1); // Cria um retângulo ao redor do texto detectado, permitindo desenhar ou sobrepor.

                     // Ajustei a filtragem de tamanho da caixa
                    int min_width = 10;     // Tamanho mínimo da largura da caixa de texto
                    int min_height = 10;    // Tamanho mínimo da altura da caixa de texto
                    int max_width = processed_frame.cols * 0.9;     // Tamanho máximo é 90% do frame para não passar do fim da tela. (LARGURA)
                    int max_height = processed_frame.rows * 0.9;    // Tamanho máximo é 90% do frame para não passar do fim da tela. (ALTURA)

                    if (bbox.width > min_width && bbox.height > min_height && bbox.width < max_width && bbox.height < max_height)
                    {
                        // Salva a box e o texto detectados, depois guarda esses valores no array criado para que possa ser retornado.
                        TextDetection detected;
                        detected.text = string(word);
                        detected.box = bbox;
                        detected_texts.push_back(detected);
                        cout << "DEBUG Tesseract - Detectado: '" << detected.text << "' Confianca: " << conf << " BBox: (" << x1 << "," << y1 << ")-(" << x2 << "," << y2 << ")" << endl;
                    }
                }
            } 
            delete[] word; // Libera a memória após o fim da utilidade do texto atual, pronto para uma nova detecção
        }
        while(data_read->Next(RIL_TEXTLINE));
        delete data_read;
    }
    return detected_texts;
}