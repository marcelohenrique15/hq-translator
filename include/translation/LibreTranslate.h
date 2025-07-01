#include <optional>

#include "translation/json.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class LibreTranslateAPI {
  private:
    std::string base_url;

  public:
    LibreTranslateAPI(std::string base_url="http://localhost:5001/");
    json translate(std::string q, std::string source, std::string target);
    json languages();
    json detect(std::string q);
    std::string translateText(std::string q, std::string source, std::string target);

};

