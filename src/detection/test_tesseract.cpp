#include "detection/text_detector.h"
#include <iostream>

int main() {
    try {
        std::string path = "/home/gustavo/Área de Trabalho/test.png";
        std::string text = detection::extract_text_from_image(path);
        std::cout << "Texto detectado:\n" << text << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}