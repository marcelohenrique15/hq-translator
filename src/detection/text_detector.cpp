#include "detection/text_detector.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>

namespace detection {
    std::string extract_text_from_image(const std::string& image_path) {
        cv::Mat img = cv::imread(image_path);
        if(img.empty()) {
            throw std::runtime_error("Falha ao carregar imagem: " + image_path);
        }

        tesseract::TessBaseAPI ocr;
        if(ocr.Init(nullptr, "por+eng")) {  // Português + Inglês
            throw std::runtime_error("Falha ao inicializar Tesseract");
        }

        ocr.SetImage(img.data, img.cols, img.rows, 3, img.step);
        std::unique_ptr<char[]> result(ocr.GetUTF8Text());
        return result ? std::string(result.get()) : "";
    }
}