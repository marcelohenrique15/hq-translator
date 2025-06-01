#include <translation/translator.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace std;
using namespace cpr;

string ask_ia(const string& prompt)
{
    const string api_key = "AIzaSyCJCkN5Ep0mxtPysCkxCaedw5BuOFJJX38";
    const string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + api_key;

    // Esse é o formato do JSON enviado para requisição, contents é a conversa com a Gemini, 
    // Parts é o bloco da mensagem enviada e Text é o texto enviado (prompt).
    json body = 
    {
        {"contents", {{
            {"parts", {{
                {"text", prompt}
            }}}
        }}}
    };

    // Essa é a requisição que envia o JSON para a Gemini nos devolver uma resposta.
    Response r = Post
    (
        Url{url},
        Header{{"Content-Type", "application/json"}},
        Body{body.dump()}
    );

    //Se a requisição for um sucesso, o texto da resposta será retornado como string.
    if (r.status_code == 200)
    {
        json res = json::parse(r.text);

        string reply = res["candidates"][0]["content"]["parts"][0]["text"];
        return reply;
    }
    
    else
    {
        return "Erro HTTP ao consultar Gemini: status code = " + to_string(r.status_code);
    }
}