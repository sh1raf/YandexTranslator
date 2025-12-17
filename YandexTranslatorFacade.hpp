#pragma once YANDEX_TRANSLATOR_FACADE_HPP
#define YANDEX_TRANSLATOR_FACADE_HPP

#include <string>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <iostream>
#include <map>

using nlohmann::json;

class YandexTranslatorFacade {
public:
    YandexTranslatorFacade(const std::string& url_, 
                          const std::string& token_, 
                          const std::string& folderId_)
        : url(url_), apiKey(token_), folderId(folderId_) {
        // Инициализация поддерживаемых языков
        supportedLanguages = {
            {"en", "English"},
            {"ru", "Russian"},
            {"fr", "French"},
            {"de", "German"},
            {"es", "Spanish"},
            {"it", "Italian"},
            {"zh", "Chinese"},
            {"ja", "Japanese"},
            {"ko", "Korean"}
        };
    }

    std::string translate(const std::string& text, 
                         const std::string& sourceLanguage = "ru",
                         const std::string& targetLanguage = "en") {
        
        json response = makeRequest(text, sourceLanguage, targetLanguage);
        
        if (response.contains("translations") && 
            response["translations"].is_array() && 
            !response["translations"].empty()) {
            
            return response["translations"][0]["text"];
        }
        
        return "[Ошибка перевода: " + text + "]";
    }

    bool isLanguageSupported(const std::string& langCode) const {
        return supportedLanguages.find(langCode) != supportedLanguages.end();
    }

    std::string getLanguageName(const std::string& langCode) const {
        auto it = supportedLanguages.find(langCode);
        if (it != supportedLanguages.end()) {
            return it->second;
        }
        return "Unknown";
    }

    void printSupportedLanguages() const {
        std::cout << "\nПоддерживаемые языки:\n";
        std::cout << "Код | Язык\n";
        std::cout << "----------\n";
        for (const auto& [code, name] : supportedLanguages) {
            std::cout << code << "  | " << name << "\n";
        }
        std::cout << std::endl;
    }

private:
    std::string url;
    std::string apiKey;
    std::string folderId;
    std::map<std::string, std::string> supportedLanguages;

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
        size_t totalSize = size * nmemb;
        output->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    json makeRequest(const std::string& text, 
                    const std::string& sourceLanguage,
                    const std::string& targetLanguage) {
        
        CURL* curl;
        CURLcode res;
        std::string response;
        json jsonRequest;
        
        jsonRequest["sourceLanguageCode"] = sourceLanguage;
        jsonRequest["targetLanguageCode"] = targetLanguage;
        jsonRequest["folderId"] = folderId;
        jsonRequest["texts"] = json::array({text});
        
        std::string jsonData = jsonRequest.dump();

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        
        if (curl) {
            struct curl_slist* headers = nullptr;

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, "Accept: application/json");

            std::string auth_header = "Authorization: Api-Key " + apiKey;
            headers = curl_slist_append(headers, auth_header.c_str());
            
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonData.length());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            res = curl_easy_perform(curl);
            
            if (res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            }
            
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
        
        curl_global_cleanup();

        try {
            return json::parse(response);
        } catch (const json::parse_error& e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
            return json::object();
        }
    }
};