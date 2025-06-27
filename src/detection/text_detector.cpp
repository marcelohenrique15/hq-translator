#include "detection/text_detector.h"


namespace detection {

    
    bool is_alphanumeric(const std::string& s) {
        if (s.empty()) return false;
        
        return std::all_of(s.begin(), s.end(), [](char c){ return ::isalnum(c) || ::isspace(c) || c == '.' || c == ',' || c == '-' || c == '/'; });
    }

    cv::Mat preprocess_image(const cv::Mat& input_image) {
        cv::Mat gray_image;
        cv::cvtColor(input_image, gray_image, cv::COLOR_BGR2GRAY);

        cv::Mat processed_image = gray_image.clone();

        cv::medianBlur(processed_image, processed_image, 3);

        cv::adaptiveThreshold(processed_image, processed_image, 255,
                              cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 21, 5);

        cv::bitwise_not(processed_image, processed_image);

        cv::Mat kernel_dilate = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2,2));
        cv::dilate(processed_image, processed_image, kernel_dilate, cv::Point(-1,-1), 1);

        cv::imshow("Imagem Pre-Processada (Tesseract)", processed_image);

        return processed_image;
    }

    std::vector<DetectedText> detect_text_and_boxes(const cv::Mat& image) {
        std::vector<DetectedText> detected_texts;
        if(image.empty()) {
            std::cerr << "Erro: Imagem vazia para deteccao de texto." << std::endl;
            return detected_texts;
        }

        cv::Mat preprocessed_img = preprocess_image(image);

        std::unique_ptr<tesseract::TessBaseAPI> ocr(new tesseract::TessBaseAPI());
        if(ocr->Init("/usr/local/share/tessdata/", "eng", tesseract::OEM_LSTM_ONLY)) {
            std::cerr << "Falha ao inicializar Tesseract. Verifique se os arquivos .traineddata estao instalados." << std::endl;
            return detected_texts;
        }

        ocr->SetPageSegMode(tesseract::PSM_AUTO); 

       
        ocr->SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ");

        ocr->SetImage(preprocessed_img.data, preprocessed_img.cols, preprocessed_img.rows,
                      preprocessed_img.channels(), preprocessed_img.step);

        std::cout << "\n--- Tesseract Output (Full Text) ---\n";
        std::unique_ptr<char[]> fullText(ocr->GetUTF8Text());
        if (fullText) {
            std::cout << fullText.get() << std::endl;
        } else {
            std::cout << "Nenhum texto detectado por GetUTF8Text().\n";
        }
        std::cout << "-------------------------------------\n";

        tesseract::ResultIterator* ri = ocr->GetIterator();
        if (ri != nullptr) {
            do {
                const char* word = ri->GetUTF8Text(tesseract::RIL_WORD);
                if (word != nullptr && strlen(word) > 0) {
                    float conf = ri->Confidence(tesseract::RIL_WORD);

                    // --- AJUSTAR AQUI: LIMIAR DE CONFIANCA MAIS BAIXO 
                    if (conf > 60 && word[0] != ' ' && is_alphanumeric(std::string(word)) ) {
                        int x1, y1, x2, y2;
                        if (ri->BoundingBox(tesseract::RIL_WORD, &x1, &y1, &x2, &y2)) {
                            cv::Rect bbox(x1, y1, x2 - x1, y2 - y1);

                            // Ajustei a filtragem de tamanho da caixa
                            int min_width = 10; // Reduzi minimo para pegar letras menores
                            int min_height = 10; // Reduzi minimo
                            int max_width = preprocessed_img.cols * 0.9; // Max 90% da largura
                            int max_height = preprocessed_img.rows * 0.9; // Max 90% da altura

                            if (bbox.width > min_width && bbox.height > min_height &&
                                bbox.width < max_width && bbox.height < max_height)
                            {
                                DetectedText dt;
                                dt.text = std::string(word);
                                dt.bbox = bbox;
                                detected_texts.push_back(dt);
                                // DEBUG: Imprimir o que foi detectado para o terminal
                                std::cout << "DEBUG Tesseract - Detectado: '" << dt.text << "' Confianca: " << conf << " BBox: (" << x1 << "," << y1 << ")-(" << x2 << "," << y2 << ")" << std::endl;
                            }
                        }
                    }
                    delete[] word;
                }
            } while (ri->Next(tesseract::RIL_WORD));
            delete ri;
        }
        return detected_texts;
    }

    void draw_text_boxes(cv::Mat& frame, const std::vector<DetectedText>& detected_texts) {
        int fontFace = cv::FONT_HERSHEY_SIMPLEX;
        double fontScale = 0.5;
        int thickness = 1;

        for (const auto& dt : detected_texts) {
            cv::rectangle(frame, dt.bbox, cv::Scalar(0, 255, 0), 2);

            cv::putText(frame, dt.text, cv::Point(dt.bbox.x, dt.bbox.y - 5),
                        fontFace, fontScale, cv::Scalar(255, 255, 255), thickness, cv::LINE_AA);
        }
    }

}
