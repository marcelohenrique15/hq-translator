#include "overlayer/overlay_translator.h"

using namespace cv;
using namespace std;

void Overlay::drawTextOverlay(Mat& frame, const string& translated_text, const Rect& origin)
{
    int fontFace = FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.5;
    int thickness = 2;
    int baseline = 0;
    int margin = 10;

    // Medir tamanho do texto
    Size textSize = getTextSize(translated_text, fontFace, fontScale, thickness, &baseline);

    // Definir a caixa adaptada ao texto com margem
    Rect box(origin.x, origin.y, textSize.width + 2 * margin, textSize.height + 2 * margin);

    // Desenhar caixa preta opaca
    rectangle(frame, box, Scalar(0, 0, 0), FILLED);

    // Desenhar texto centralizado dentro da caixa
    Point textOrg(box.x + margin, box.y + textSize.height + margin / 2);
    putText(frame, translated_text, textOrg, fontFace, fontScale, Scalar(255, 255, 255), thickness);
}