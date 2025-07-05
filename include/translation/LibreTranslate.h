#pragma once // Garante que este arquivo de cabeçalho seja incluído apenas uma vez.

#include <nlohmann/json.hpp> // Inclui a biblioteca Nlohmann JSON para manipulação de dados JSON.

using json = nlohmann::json; // Cria um alias 'json' para nlohmann::json para facilitar o uso.

// --- CLASSE ---
// A classe 'LibreTranslateAPI' é um modelo para criar objetos que encapsulam toda
// a funcionalidade e os dados necessários para interagir com um serviço de tradução
// LibreTranslate. Ela agrupa a URL da API e os métodos para realizar operações
// como traduzir texto, listar idiomas e detectar idiomas.

class LibreTranslateAPI {
  private:

    // --- ENCAPSULAMENTO (Atributo Privado) ---
    // 'base_url' é um atributo **privado**. Isso significa que ele só pode ser acessado
    // e modificado por métodos dentro da própria classe 'LibreTranslateAPI'.
    // Esse encapsulamento protege a URL base de ser alterada acidentalmente por código
    // externo, garantindo que as requisições à API sempre utilizem o endereço correto.

    std::string base_url; // Armazena a URL base da API LibreTranslate.

  public:
 // --- MÉTODOS PÚBLICOS ---
    // 'translate()', 'languages()', 'detect()' e 'translateText()' são os **métodos públicos**
    // da classe. Eles definem a interface através da qual o "mundo exterior" interage
    // com um objeto 'LibreTranslateAPI'. Eles abstraem a complexidade das requisições
    // de rede (HTTP) e da manipulação de JSON, oferecendo uma maneira simples e direta
    // de usar o serviço de tradução.

    LibreTranslateAPI(std::string base_url="http://libretranslate_api:5000/");

    // --- MÉTODOS PÚBLICOS ---
    // 'translate()', 'languages()', 'detect()' e 'translateText()' são os **métodos públicos**
    // da classe. Eles definem a interface através da qual o "mundo exterior" interage
    // com um objeto 'LibreTranslateAPI'. Eles abstraem a complexidade das requisições
    // de rede (HTTP) e da manipulação de JSON, oferecendo uma maneira simples e direta
    // de usar o serviço de tradução.

    // @brief Traduz um texto e retorna a resposta completa da API em formato JSON.
    // @param q O texto a ser traduzido.
    // @param source O código do idioma de origem (ex: "en" para inglês).
    // @param target O código do idioma de destino (ex: "pt" para português).
    // @return Um objeto JSON contendo a resposta completa da API.

    json translate(std::string q, std::string source, std::string target);
 
    // @brief Obtém a lista de idiomas suportados pela API.
    // @return Um objeto JSON contendo a lista de idiomas.

    json languages();

    // @brief Detecta o idioma de um determinado texto.
    // @param q O texto a ser analisado.
    // @return Um objeto JSON contendo o resultado da detecção.

    json detect(std::string q);

    
     // @brief Método de conveniência que traduz um texto e retorna apenas o texto traduzido.
     // @details Este método chama 'translate', analisa o JSON resultante e extrai
     // apenas a string do texto traduzido, simplificando o uso comum.
     // @param q O texto a ser traduzido.
     // @param source O código do idioma de origem.
     // @param target O código do idioma de destino.
     // @return Uma std::string contendo apenas o texto final traduzido.
     
    std::string translateText(std::string q, std::string source, std::string target);
};

