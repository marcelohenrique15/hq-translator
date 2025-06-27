#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

using namespace nlohmann;
using namespace cpr;
using namespace std;

class Gemini 
{
    private:
        string api_key;
        string make_gemini_request(const json& body);
        
    public:
        Gemini(const string& api_key = "AIzaSyCJCkN5Ep0mxtPysCkxCaedw5BuOFJJX38");
        string translate(const string& text_to_translate);
        string search(const string& query);


};


