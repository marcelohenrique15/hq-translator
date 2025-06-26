#include <translation/translate.h>
#include <iostream>

using namespace std;

Gemini::Gemini(const string& api_key) : api_key_(api_key) {}

string Gemini::make_gemini_request(const nlohmann::json& body) {
    const string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + api_key_;

    cpr::Response r = cpr::Post(
        cpr::Url{url},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body{body.dump()}
    );

    if (r.status_code == 200)
    {
        nlohmann::json res = nlohmann::json::parse(r.text);
        if (res.contains("candidates") && !res["candidates"].empty()) {
            return res["candidates"][0]["content"]["parts"][0]["text"];
        } else {
            return "Erro: Resposta da Gemini nao contem \'candidates\' ou esta vazia.";
        }
    } else
    {
        return "Erro HTTP ao consultar Gemini: status code = " + to_string(r.status_code) + ", Mensagem: " + r.text;
    }
}

string Gemini::translate(const string& text_to_translate) {
    nlohmann::json body = {
        {"contents", {{
            {"parts", {{
                {"text", "Traduza APENAS o seguinte texto para o portugues, usando somente caracteres ASCII basicos (sem acentos ou caracteres especiais como 'c'), sem nenhuma explicacao adicional ou contexto, e SEM PONTUACAO FINAL: " + text_to_translate}
            }}}
        }}}
    };
    return make_gemini_request(body);
}

string Gemini::search(const string& query) {
    nlohmann::json body = {
        {"contents", {{
            {"parts", {{
                {"text", "Pesquise sobre \'" + query + "\' e forneca APENAS o nome do anime e uma brevissima sinopse. Use somente caracteres ASCII basicos (sem acentos ou caracteres especiais como 'c'). Formate a resposta EXATAMENTE assim: ANIME: <Nome do Anime>\nSINOPSE: <Sinopse Brevissima>"}
            }}}
        }}}
    };
    return make_gemini_request(body);
}