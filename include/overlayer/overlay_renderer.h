#pragma once // Garante que este arquivo de cabeçalho seja incluído apenas uma vez na compilação.
#include <opencv2/opencv.hpp> // Inclui a biblioteca OpenCV para funcionalidades de visão computacional.
#include <string> // Inclui a biblioteca padrão C++ para manipulação de strings.

using namespace std; // Usa o namespace padrão C++.

// --- CLASSE ---
// A classe 'Overlay' é um molde ou projeto para organizar a funcionalidade
// relacionada a desenhar elementos (neste caso, texto) sobre frames de vídeo.
// Ela agrupa um método que executa essa ação, tornando o código mais modular e claro.

class Overlay
{
    public:

        // --- MÉTODO ESTÁTICO (e Implicação de Encapsulamento) ---
        // 'drawTextOverlay' é um método público e **estático**.
        //
        // **Método Estático:** Indica que este método pertence à classe 'Overlay' em si,
        // e não a uma instância específica (objeto) da classe. Você pode chamá-lo
        // diretamente usando `Overlay::drawTextOverlay(...)` sem precisar criar um objeto `Overlay`.
        // Isso é útil para funcionalidades que não dependem do estado de um objeto,
        // mas que estão logicamente associadas ao propósito da classe.
        //
        // **Encapsulamento (Implícito):** Embora não existam atributos privados aqui,
        // a classe 'Overlay' **encapsula** a lógica de como o texto é desenhado.
        // Isso significa que todos os detalhes internos de como o `cv::putText` ou
        // qualquer outro processamento de desenho é feito estão contidos neste método,
        // protegendo o código externo da complexidade interna e permitindo que as
        // mudanças na forma de desenhar sejam feitas apenas aqui.
        //
        // Parâmetros:
        // - cv::Mat& frame: O frame da imagem onde o texto será desenhado (passado por referência para modificação).
        // - const std::string& translated_text: O texto já traduzido a ser exibido.
        // - const cv::Rect& box: A caixa delimitadora (retângulo) onde o texto original foi detectado.
        //                        Este parâmetro é usado para posicionar o texto traduzido.
        
        static void drawTextOverlay(cv::Mat& frame, const std::string& translated_text, const cv::Rect& box);
};