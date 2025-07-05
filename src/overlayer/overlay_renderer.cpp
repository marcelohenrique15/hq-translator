#include "overlayer/overlay_renderer.h" // Inclui o cabeçalho para a classe Overlay.

using namespace std; // Usa o namespace padrão C++.
 
// --- MÉTODO ESTÁTICO (Implementação) ---
// Esta é a implementação do método `drawTextOverlay`. Como ele foi declarado como `static`
// no cabeçalho da classe `Overlay`, ele é implementado aqui sem a necessidade de prefixar
// com `Overlay::` para acessar membros de instância (pois não há membros de instância
// sendo usados por este método estático). Ele opera diretamente nos parâmetros que recebe.
//
// --- ENCAPSULAMENTO ---
// Este método encapsula toda a complexidade de desenhar um texto sobreposto em uma imagem.
// Ele lida com a remoção do texto original (via inpainting), o cálculo do tamanho do novo texto,
// o posicionamento para centralização e, finalmente, o desenho do texto.
// A lógica detalhada de como isso é feito (chamadas a `cv::inpaint`, `cv::getTextSize`, `cv::putText`)
// é contida dentro deste método. O usuário da classe `Overlay` simplesmente chama `Overlay::drawTextOverlay()`
// e não precisa se preocupar com os passos internos, demonstrando o **encapsulamento** da funcionalidade.

void Overlay::drawTextOverlay(cv::Mat& frame, const string& translated_text, const cv::Rect& origin)
{
    // Define propriedades da fonte para o texto traduzido.

    int fontFace = cv::FONT_HERSHEY_PLAIN; // Tipo de fonte (simples).
    double fontScale = 2.0; // Escala da fonte.
    int thickness = 2; // Espessura das linhas da fonte.
    int baseline = 0; // Variável para obter a linha de base do texto (usada por getTextSize).

    // PASSO 1: Usar inpainting para remover o texto original.
    // Cria uma máscara em preto do mesmo tamanho do frame.

    cv::Mat mask = cv::Mat::zeros(frame.size(), CV_8UC1);

    // Desenha um retângulo preenchido na máscara na posição do texto original,
    // marcando a área a ser inpainted. O Scalar(255) significa branco total na máscara.

    rectangle(mask, origin, cv::Scalar(255), cv::FILLED);
    
    cv::Mat inpainted; // Matriz para armazenar o resultado do inpainting.

     // Realiza o inpainting (preenchimento da área marcada) no frame.
    // - frame: Imagem original de entrada.
    // - mask: Máscara que indica a área a ser preenchida.
    // - inpainted: Imagem de saída com a área preenchida.
    // - 3: Raio da vizinhança usada para inpainting.
    // - cv::INPAINT_TELEA: Algoritmo de inpainting (baseado em Fast Marching Method).

    inpaint(frame, mask, inpainted, 3, cv::INPAINT_TELEA);
    // Copia o resultado do inpainting de volta para o frame original.

    inpainted.copyTo(frame);

    // PASSO 2: Desenhar o texto traduzido.
    // Calcula o tamanho que o texto traduzido ocupará na imagem.
    // Isso é importante para centralizar o texto dentro da caixa original.

    cv::Size textSize = cv::getTextSize(translated_text, fontFace, fontScale, thickness, &baseline);

    // Calcula o ponto de origem (canto inferior esquerdo) para centralizar o texto.
    // O texto é centralizado horizontalmente e verticalmente dentro da 'origin
    
    cv::Point textOrg(
        origin.x + (origin.width - textSize.width) / 2,
        origin.y + (origin.height + textSize.height) / 2
    );

    // Finalmente, desenha o texto traduzido sobre o frame já modificado.
    // - frame: Imagem de destino onde o texto será desenhado.
    // - translated_text: O texto que será exibido.
    // - textOrg: O ponto de origem (canto inferior esquerdo) calculado para centralização.
    // - fontFace, fontScale, thickness: Parâmetros da fonte definidos anteriormente.
    // - cv::Scalar(0, 0, 0): A cor do texto (preto, no formato BGR).
    
    putText(frame, translated_text, textOrg, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);
}