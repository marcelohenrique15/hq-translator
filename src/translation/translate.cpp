#include <translation/translate.h>
#include <iostream>

Gemini::Gemini(const string& api_key) : api_key(api_key) {}

string Gemini::make_gemini_request(const nlohmann::json& body) 
{
    const string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + api_key;

    cpr::Response r = Post(
        Url{url},
        Header{{"Content-Type", "application/json"}},
        Body{body.dump()}
    );

    if (r.status_code == 200)
    {
        json res = json::parse(r.text);
        if (res.contains("candidates") && !res["candidates"].empty()) 
        {
            return res["candidates"][0]["content"]["parts"][0]["text"];
        } 
        
        else 
        {
            return "Erro: Resposta da Gemini nao contem \'candidates\' ou esta vazia.";
        }
    } 
    
    else
    {
        return "Erro HTTP ao consultar Gemini: status code = " + to_string(r.status_code) + ", Mensagem: " + r.text;
    }
}

string Gemini::translate(const string& text_to_translate) {
    json body = {
        {"contents", {{
            {"parts", {{
                {"text", "Traduza APENAS o seguinte texto para o portugues brasileiro, usando SOMENTE letras basicas do alfabeto (a-z, A-Z), numeros (0-9) e espacos. NAO use acentos, cedilha, til ou qualquer caractere especial. Responda APENAS com a traducao, sem explicacoes: " + text_to_translate}
            }}}
        }}}
    };
    
    string result = make_gemini_request(body);
    
    // Limpar caracteres especiais que possam ter sobrado
    string cleaned_result;
    for (char c : result) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
            (c >= '0' && c <= '9') || c == ' ' || c == '.' || c == ',' || c == '!') {
            cleaned_result += c;
        }
    }
    
    return cleaned_result;
}

string Gemini::search(const string& query) {
    json body = {
        {"contents", {{
            {"parts", {{
                {"text", "Pesquise sobre \'" + query + "\' e forneca APENAS o nome do anime e uma brevissima sinopse. Use somente caracteres ASCII basicos (sem acentos ou caracteres especiais como 'c'). Formate a resposta EXATAMENTE assim: ANIME: <Nome do Anime>\nSINOPSE: <Sinopse Brevissima>"}
            }}}
        }}}
    };
    return make_gemini_request(body);
}