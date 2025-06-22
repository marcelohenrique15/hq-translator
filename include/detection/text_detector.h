#pragma once
#include <string>
#include <stdexcept>  // Para tratamento de exceções

/**
 * @namespace detection
 * @brief Namespace para funções de detecção de texto
 */
namespace detection {

    /**
     * @brief Extrai texto de uma imagem usando OCR
     * @param image_path Caminho para o arquivo de imagem
     * @return String com o texto detectado
     * @throws std::runtime_error Se a imagem não puder ser processada
     */
    std::string extract_text_from_image(const std::string& image_path);

} // namespace detection