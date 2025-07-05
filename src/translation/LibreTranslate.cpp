#include "translation/LibreTranslate.h" // Inclui o cabeçalho da classe LibreTranslateAPI.

#include <vector> // Para o uso de vetores dinâmicos.

#include <curl/curl.h> // Inclui a biblioteca cURL para realizar requisições HTTP.

// --- Funções Globais Auxiliares ---
// As funções a seguir não são métodos de classe, mas apoiam a classe
// para encapsular a complexidade da comunicação HTTP e manipulação de JSON.

// --- Função de callback para escrita de dados do cURL ---
// Esta função é chamada pelo cURL para cada pedaço de dados recebido da requisição HTTP.
// Ele anexa os 'contents' (dados recebidos) à string 's'.
size_t curl_writeback_fun(void* contents, size_t size, size_t nmemb, std::string *s) {
  size_t len = size * nmemb; // Calcula o tamanho total dos dados recebidos.
  s->append((char*)contents, len); // Anexa os dados ao buffer de string.
  return len;  // Retorna o número de bytes processados.
}

// --- Função para realizar uma requisição HTTP POST usando cURL ---
// - url: A URL para onde a requisição será enviada.
// - headers: Um vetor de strings contendo os cabeçalhos HTTP (ex: "Content-Type: application/json").
// - post_fields: A string contendo os dados a serem enviados no corpo da requisição POST.
// Retorna a resposta do servidor como uma string.
std::string curl_post(std::string url, std::vector<std::string> headers, std::string post_fields){
  // Baseado no exemplo oficial: https://curl.se/libcurl/c/http-post.html
  // Daniel Stenberg, <daniel@haxx.se>, et al.

  std::string to_return; // String para armazenar a resposta do servidor.
  CURL *curl; // Ponteiro para o manipulador de cURL.
  curl_global_init(CURL_GLOBAL_ALL); // Inicializa a libcurl (necessário uma vez por aplicação).
  curl = curl_easy_init(); // Cria um manipulador de easy cURL.
  if(curl) { // Verifica se o manipulador foi criado com sucesso.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // Define a URL da requisição.
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str()); // Define os dados para o POST.

    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // Habilita SSL para todas as conexões.
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);  // Verifica o certificado do servidor (importante para segurança).

    struct curl_slist *headers_slist = NULL; // Lista encadeada para armazenar os cabeçalhos.
    if(headers.size() > 0){ // Se houver cabeçalhos, adiciona-os à lista.
      for(std::string header : headers){
        headers_slist = curl_slist_append(headers_slist, header.c_str()); // Adiciona cada cabeçalho.
      }
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers_slist); // Define os cabeçalhos na requisição cURL.
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writeback_fun); // Define a função de callback para escrita da resposta.
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &to_return); // Passa o buffer para a função de callback.

    CURLcode res;
    res = curl_easy_perform(curl); // Executa a requisição HTTP.

    if(res != CURLE_OK){ // Verifica se houve algum erro na requisição.
      throw curl_easy_strerror(res); // Lança uma exceção com a descrição do erro.
    }

    if(headers_slist){ // Se os cabeçalhos foram definidos, libera a memória da lista.
      curl_slist_free_all(headers_slist);
    }
 
    curl_easy_cleanup(curl); // Libera o manipulador de easy cURL.
  }
  curl_global_cleanup(); // Libera os recursos globais da libcurl.

  return to_return; // Retorna a string da resposta.
}

// --- Função auxiliar para realizar uma requisição HTTP POST com corpo JSON ---
// - url: A URL para onde a requisição será enviada.
// - data: Um objeto JSON (nlohmann::json) a ser enviado no corpo da requisição.
// Retorna a resposta do servidor como um objeto JSON.
json json_post(std::string url, json data){
  std::vector<std::string> headers;
  // Define o cabeçalho essencial para informar ao servidor que estamos enviando JSON.
  headers.push_back("Content-Type: application/json");

  // Serializa o objeto JSON para uma string formatada.
  std::string serialized_json = data.dump();
  
  // Chama a função de baixo nível para realizar a requisição POST.
  std::string res = curl_post(url, headers, serialized_json);

  // Analisa (parse) a string de resposta de volta para um objeto JSON.
  return json::parse(res);
}

// --- Construtor da Classe LibreTranslateAPI ---
// Inicializa o objeto da API, configurando a URL base do serviço de tradução.
LibreTranslateAPI::LibreTranslateAPI(std::string base_url)
  : base_url(base_url){}

// --- Traduz um texto ---
// Constrói e envia uma requisição para o endpoint de tradução da API.
json LibreTranslateAPI::translate(std::string q, std::string source, std::string target){
  std::string url = base_url + "translate"; // Constrói a URL completa do endpoint.
  
  // Cria o corpo (payload) da requisição em formato JSON.
  json req;
  req["q"] = q;
  req["source"] = source;
  req["target"] = target;

  // Realiza a chamada e retorna a resposta JSON completa.
  json res = json_post(url, req);
  return res;
}

// --- Lista os idiomas disponíveis ---
// Constrói e envia uma requisição para o endpoint que lista os idiomas suportados.
json LibreTranslateAPI::languages(){
  std::string url = base_url + "languages";
  
  json req; // O corpo da requisição é vazio para este endpoint.

  json res = json_post(url, req);
  return res;
}

// --- Detecta o idioma de um texto ---
// Constrói e envia uma requisição para o endpoint de detecção de idioma.
json LibreTranslateAPI::detect(std::string q){
  std::string url = base_url + "detect";
  
  json req;
  req["q"] = q; // O corpo da requisição contém apenas o texto a ser analisado.

  json res = json_post(url, req);
  return res;
}

// --- Método de conveniência para traduzir e limpar o texto ---
// Retorna diretamente a string traduzida, em vez do objeto JSON completo,
// e aplica uma limpeza para remover caracteres indesejados.
std::string LibreTranslateAPI::translateText(std::string q, std::string source, std::string target) {
    // Primeiro, obtém a resposta JSON completa.
    auto res = translate(q, source, target);
    
    // Verifica se a resposta contém o campo "translatedText".
    if (res.contains("translatedText")) {
      std::string result = res["translatedText"];
      std::string cleaned_result;

      // Itera sobre a string de resultado para filtrar apenas caracteres permitidos.
      // Isso garante que o resultado final seja "limpo" e seguro para exibição.
      for (char c : result) 
      {
          if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ' || c == '.' || c == ',' || c == '!') 
          {
              cleaned_result += c;
          }
      }
      return cleaned_result;
    }
    // Se a tradução falhar ou a resposta não tiver o formato esperado, retorna uma string de erro.
    return "[erro]";
}