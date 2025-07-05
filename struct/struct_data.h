#pragma once  // Garante que este arquivo de cabeçalho seja incluído apenas uma vez na compilação.

#include <opencv2/opencv.hpp> // Inclui a biblioteca OpenCV para usar cv::Rect.
#include <iostream> // Inclui a biblioteca padrão C++ para operações de entrada/saída (não diretamente usada aqui, mas comum para debug).

// --- ESTRUTURA DE DADOS (AGREGAÇÃO) / ENCAPSULAMENTO BÁSICO ---
// 'textData' é uma **estrutura de dados** (struct). Em C++, uma 'struct' é uma forma
// simplificada de uma classe, onde, por padrão, todos os membros são públicos.
//
// Ela exemplifica a **agregação** de dados relacionados. Em vez de ter três variáveis
// separadas (`string original_text`, `string translated_text`, `cv::Rect text_box`),
// esta `struct` as agrupa em uma única unidade lógica. Isso permite que você trate
// "um pedaço de texto detectado com sua tradução e posição" como um **único conceito**.
//
// Embora não tenha métodos ou encapsulamento de acesso (como membros privados e públicos),
// o simples ato de agrupar esses dados já é um passo fundamental da POO:
// criar tipos de dados complexos que representam entidades do mundo real ou do domínio do problema.
// Essa agregação facilita a passagem de informações de forma organizada entre diferentes
// partes do seu código (como entre as threads de detecção e tradução), reduzindo a
// quantidade de parâmetros e melhorando a legibilidade.

struct textData 
    {
      std::string original; // Armazena o texto original detectado pela OCR.
      std::string translated; // Armazena o texto traduzido.
      cv::Rect box; // Armazena a caixa delimitadora (bounding box) do texto no frame.
                    // cv::Rect é uma estrutura do OpenCV que define um retângulo (x, y, largura, altura).
    };