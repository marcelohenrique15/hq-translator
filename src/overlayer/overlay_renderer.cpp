#include "overlayer/overlay_renderer.h"

using namespace std;

void Overlay::drawTextOverlay(cv::Mat& frame, const string& translated_text, const cv::Rect& origin)
{
    int fontFace = cv::FONT_HERSHEY_PLAIN;
    double fontScale = 2.0;
    int thickness = 2;
    int baseline = 0;

    // PASSO 1: Usar inpainting para remover o texto original
    cv::Mat mask = cv::Mat::zeros(frame.size(), CV_8UC1);
    rectangle(mask, origin, cv::Scalar(255), cv::FILLED);
    
    cv::Mat inpainted;
    inpaint(frame, mask, inpainted, 3, cv::INPAINT_TELEA);
    inpainted.copyTo(frame);

    // PASSO 2: Desenhar o texto traduzido
    cv::Size textSize = cv::getTextSize(translated_text, fontFace, fontScale, thickness, &baseline);
    
    cv::Point textOrg(
        origin.x + (origin.width - textSize.width) / 2,
        origin.y + (origin.height + textSize.height) / 2
    );
    
    putText(frame, translated_text, textOrg, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);
}