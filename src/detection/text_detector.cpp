#include "detection/text_detector.h"
#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>

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

cv::Mat Detector::preprocess_image(const cv::Mat& frame)
{
    // Passa o frame de colorido para escala de cinza
    cv::Mat gray_frame;
    cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);

    // Clona o frame cinza para um novo espaço na memória
    // para que possa se processador sem afetar o frame original,
    // assim pode-se evitar bugs.
    cv::Mat processed_frame = gray_frame.clone();

    // Suaviza a imagem, diminuindo pequenos ruídos e preservando bordas.
    medianBlur(processed_frame, processed_frame, 3);

    // Transforma a imagem em preto e branco (0 ou 255), e utiliza
    // a média Gaussiana para definir o limite dos pixels vizinhos 
    // que serão detectados, alterar esses valores (21, 5)         --> 21 = limite da vizinhança, 5 = subtrai em 5 a média da vizinhança
    // pode ajudar ou piorar a leitura de dados                       --> Resultado: Letra preta em fundo branco.
    adaptiveThreshold(processed_frame, processed_frame, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 21, 5);

    imshow("tesseract", processed_frame);

    return processed_frame;
}

vector<textData> Detector::detect_text_box(const cv::Mat& frame)
{
    vector<textData> detected_texts;    // Serivrá para guardar os textos detectados e suas boxes em um array.
    cv::Mat processed_frame = preprocess_image(frame);

    unique_ptr<tesseract::TessBaseAPI> ocr(new tesseract::TessBaseAPI());           // Cria um objeto único (ocr) para acessar a API do Tesseract
    ocr->Init("/usr/local/share/tessdata/", "eng", tesseract::OEM_LSTM_ONLY);   // Escolhe o arquivo do modelo de IA treinada para detecção de texto
    ocr->SetPageSegMode(tesseract::PSM_AUTO);                                       // Informa como os textos serão lidos (por palavra, por bloco, etc.)

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
    tesseract::ResultIterator* data_read = ocr->GetIterator();

    // >>> NOVA ALTERAÇÃO: Lista de Exceções
    // Explicação: Aqui criamos a lista com as palavras curtas que você pediu para não serem filtradas.
    // Usamos 'static const unordered_set' por dois motivos:
    // 1. 'unordered_set': É uma estrutura de dados extremamente rápida para verificar se um item existe.
    // 2. 'static const': Garante que esta lista seja criada apenas uma vez, otimizando o desempenho,
    //    em vez de recriá-la a cada chamada da função.
    static const unordered_set<string> short_word_exceptions = {
        "a", "I", "am", "an", "as", "at", "be", "by", "do", "go", "he",
        "if", "in", "is", "it", "me", "my", "no", "of", "on", "or",
        "so", "to", "up", "us", "we", "she"
    };

    // Se houver dados, começa a filtrar e retornar as strings que queremos
    if(data_read != nullptr)                                        
    {
        do 
        {
            const char* word = data_read->GetUTF8Text(tesseract::RIL_TEXTLINE);         // Lê o texto por linhas.
            
            // >>> NOVA ALTERAÇÃO: Lógica de verificação e filtro
            // Explicação: O bloco 'if' original foi substituído por este para acomodar a lista de exceções.
            if(word != nullptr)
            {
                // Passo 1: Tratamento da linha detectada
                // O Tesseract geralmente retorna o texto com um caractere de nova linha (\n) no final.
                // Para que a comparação com a nossa lista de exceções funcione, precisamos primeiro
                // converter o texto para o tipo 'string' e remover esse '\n' do final.
                string line(word);
                if (!line.empty() && line.back() == '\n') {
                    line.pop_back();
                }

                // Passo 2: O novo filtro de aceitação
                // Esta é a nova condição principal que substitui o antigo 'strlen(word) > 2'.
                // Uma linha de texto será considerada válida se:
                //   (1) ela tiver mais de 2 caracteres, OU
                //   (2) ela for exatamente igual a uma das palavras na nossa lista de exceções.
                bool is_valid_line = (line.length() > 2) || (short_word_exceptions.count(line) > 0);

                // Se a linha for válida (passou no filtro acima), o código continua como antes.
                if(is_valid_line)
                {
                    float conf = data_read->Confidence(tesseract::RIL_TEXTLINE); // Calcula a confiança da linha coletada
                    // Se a confiança for maior que 70, a palavra não for um espaço em branco e tiver apenas caracteres
                    // alfanuméricos (e pontuações), segue em frente.
                    // Nota: Estamos usando a variável 'line' (que foi tratada) em vez de 'word'.
                    if(conf > 70 && !line.empty() && line[0] != ' ' && is_alphanumeric(line))
                    {
                        int x1, y1; //Canto superior esquerdo da tela
                        int x2, y2; //Canto inferior direito da tela
                        data_read->BoundingBox(tesseract::RIL_TEXTLINE, &x1, &y1, &x2, &y2); //Pede as coordenadas da caixa ao redor do texto detectado
                        cv::Rect bbox(x1, y1, x2 - x1, y2 - y1); // Cria um retângulo ao redor do texto detectado, permitindo desenhar ou sobrepor.

                        // Ajustei a filtragem de tamanho da caixa
                        int min_width = 10;     // Tamanho mínimo da largura da caixa de texto
                        int min_height = 10;    // Tamanho mínimo da altura da caixa de texto
                        int max_width = processed_frame.cols * 0.9;     // Tamanho máximo é 90% do frame para não passar do fim da tela. (LARGURA)
                        int max_height = processed_frame.rows * 0.9;    // Tamanho máximo é 90% do frame para não passar do fim da tela. (ALTURA)

                        if (bbox.width > min_width && bbox.height > min_height && bbox.width < max_width && bbox.height < max_height)
                        {
                            // Salva a box e o texto detectados, depois guarda esses valores no array criado para que possa ser retornado.
                            textData detected;
                            detected.original = line; // Usamos 'line' aqui também
                            detected.box = bbox;
                            detected_texts.push_back(detected);
                            cout << "DEBUG Tesseract - Detectado: '" << detected.original << "' Confianca: " << conf << " BBox: (" << x1 << "," << y1 << ")-(" << x2 << "," << y2 << ")" << endl;
                        }
                    }
                }
            } 
            delete[] word; // Libera a memória após o fim da utilidade do texto atual, pronto para uma nova detecção
        }
        while(data_read->Next(tesseract::RIL_TEXTLINE));
        delete data_read;
    }
    return detected_texts;
}