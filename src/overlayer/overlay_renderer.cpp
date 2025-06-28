#include "overlayer/overlay_renderer.h"

using namespace cv;
using namespace std;

void Overlay::drawTextOverlay(Mat& frame, const string& translated_text, const Rect& origin)
{
    int fontFace = FONT_HERSHEY_PLAIN;
    double fontScale = 2.0;
    int thickness = 2;
    int baseline = 0;

    // PASSO 1: Usar inpainting para remover o texto original
    Mat mask = Mat::zeros(frame.size(), CV_8UC1);
    rectangle(mask, origin, Scalar(255), FILLED);
    
    Mat inpainted;
    inpaint(frame, mask, inpainted, 3, INPAINT_TELEA);
    inpainted.copyTo(frame);

    // PASSO 2: Desenhar o texto traduzido
    Size textSize = getTextSize(translated_text, fontFace, fontScale, thickness, &baseline);
    
    Point textOrg(
        origin.x + (origin.width - textSize.width) / 2,
        origin.y + (origin.height + textSize.height) / 2
    );
    
    putText(frame, translated_text, textOrg, fontFace, fontScale, Scalar(0, 0, 0), thickness);
}