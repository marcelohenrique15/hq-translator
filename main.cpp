#include "capture/webcam_capture.h"
#include "detection/text_detector.h"
#include "overlayer/overlay_translator.h"
#include <translation/translate.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm> // Para std::remove_if, std::isspace

using namespace std;
using namespace cv;

// Funcao auxiliar para remover espacos em branco do inicio e fim da string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (string::npos == first) {
        return str; // String vazia ou so com espacos
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

string extract_value(const string& line, const string& prefix) {
    size_t pos = line.find(prefix);
    if (pos != string::npos) {
        return trim(line.substr(pos + prefix.length())); // Aplica trim aqui tambem
    }
    return "";
}

int main() {
    WebcamCapture webcam;
    Overlay overlayer;

    if (!webcam.isOpened())
    {
        cerr << "Camera nao foi iniciada corretamente.\n";
        return -1;
    }

    Mat frame;
    string text_to_translate_fixed = "SHINZO WO SASAGEYO";
    string search_query = "Attack on Titan";

    string api_key = "AIzaSyCJCkN5Ep0mxtPysCkxCaedw5BuOFJJX38";

    Gemini ai(api_key);

    cout << "\n--- Testando traducao de 'SHINZO WO SASAGEYO' ---\n";
    string translated_raw = ai.translate(text_to_translate_fixed);
    string translated = trim(translated_raw); // Remove espacos/newlines do inicio/fim

    cout << "Texto traduzido: " << translated << endl;

    cout << "\n--- Testando pesquisa por 'Attack on Titan' ---\n";
    string search_result_raw = ai.search(search_query);

    string anime_name;
    string synopsis;

    stringstream ss(search_result_raw);
    string line;
    while (getline(ss, line)) {
        line = trim(line); // Remove espacos/newlines de cada linha lida

        anime_name = extract_value(line, "ANIME: ");
        if (!anime_name.empty()) continue;

        synopsis = extract_value(line, "SINOPSE: ");
        if (!synopsis.empty()) continue;
    }

    if (anime_name.empty() || synopsis.empty()) {
        stringstream ss_reparse(search_result_raw);
        vector<string> lines;
        while(getline(ss_reparse, line)) {
            lines.push_back(trim(line)); // Aplica trim ao armazenar
        }

        if (lines.size() >= 2) {
            anime_name = extract_value(lines[0], "ANIME: ");
            synopsis = extract_value(lines[1], "SINOPSE: ");
        }
    }

    cout << "ANIME: " << anime_name << endl;
    cout << "SINOPSE: " << synopsis << endl;

    while (true)
    {
        if (!webcam.readFrame(frame))
        {
            cerr << "Falha ao capturar frame.\n";
            break;
        }

        Rect textBox(100, 100, 300, 50);
        overlayer.drawTextOverlay(frame, translated, textBox);
        imshow("Webcam", frame);

        if (waitKey(1) == 27)
        {
            break;
        }
    }

    destroyAllWindows();
    return 0;
}