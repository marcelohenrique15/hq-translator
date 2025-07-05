// Garante que esse cabeçalho só será incluído uma vez durante a compilação
#pragma once

// Inclusões de bibliotecas necessárias

#include <opencv2/opencv.hpp> // OpenCV principal (abrange video, imagem, etc)
#include <tesseract/baseapi.h> // API principal do Tesseract OCR
#include <leptonica/allheaders.h> // Leptonica (biblioteca auxiliar usada internamente pelo Tesseract)
#include <opencv2/imgproc.hpp>  // Processamento de imagem com OpenCV (ex: binarização, redimensionamento, etc)

#include <string>  // Para uso de std::string
#include <vector>  // Para uso de std::vector
#include <iostream> // Para possíveis prints/debugs com std::cout

#include "struct/struct_data.h" // Estrutura personalizada `textData`, definida em outro lugar do projeto

// --- CLASSE ---
// A classe `Detector` é um projeto ou modelo para criar objetos que
// encapsulam a funcionalidade de detecção de texto em imagens usando OCR (Tesseract).
// Ela agrupa os métodos e, se houvesse, os atributos (dados) relacionados a essa tarefa,
// promovendo modularidade e reutilização.

class Detector
{
private:


    // --- ENCAPSULAMENTO (Métodos Privados) ---
    // `is_alphanumeric` e `preprocess_image` são **métodos privados**.
    // Isso significa que eles só podem ser chamados por outros métodos dentro da própria
    // classe `Detector`. Eles são auxiliares internos que servem para apoiar a lógica
    // do método público `detect_text_box`, mas não são expostos diretamente ao "mundo exterior".
    // Isso protege a implementação interna e permite que a classe gerencie seus próprios detalhes
    // de como o pré-processamento ou a validação de texto são feitos.

    // Método auxiliar que verifica se um texto contém apenas caracteres alfanuméricos

    bool is_alphanumeric(const std::string& text);

    // Método de pré-processamento da imagem antes de enviar para o OCR
    // (pode incluir redimensionamento, conversão para escala de cinza, binarização, etc)

    cv::Mat preprocess_image(const cv::Mat& frame);

public:

    // --- MÉTODOS PÚBLICOS ---
    // `detect_text_box` é o **método público** principal da classe.
    // Ele define a interface através da qual outras partes do programa interagem com um
    // objeto `Detector`. Este método expõe a funcionalidade central de detecção de texto,
    // enquanto os detalhes de como ele realiza essa detecção (por exemplo, usando
    // `preprocess_image` e `is_alphanumeric` internamente) são escondidos (encapsulados).
    
    // Recebe um frame da câmera (ou imagem), processa e retorna um vetor de `textData`
    // contendo o texto detectado, as posições dos bounding boxes e outros metadados

    std::vector<textData> detect_text_box(const cv::Mat& frame);
};
