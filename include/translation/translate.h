#ifndef GEMINI_H
#define GEMINI_H

#include <string>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

using namespace std;

class Gemini {
public:
    Gemini(const string& api_key);
    string translate(const string& text_to_translate);
    string search(const string& query);

private:
    string api_key_;
    string make_gemini_request(const nlohmann::json& body);
};

#endif // GEMINI_H

