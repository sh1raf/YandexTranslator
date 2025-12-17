#pragma once TRANSLATOR_DECORATOR_HPP
#define TRANSLATOR_DECORATOR_HPP

#include "TextDocumentDecorator.hpp"
#include "YandexTranslatorFacade.hpp"

class TranslatorDecorator : public TextDocumentDecorator {
private:
    std::string sourceLanguage_;
    std::string targetLanguage_;
    std::shared_ptr<YandexTranslatorFacade> translator_;

public:
    TranslatorDecorator(std::unique_ptr<TextDocument> document,
                       std::shared_ptr<YandexTranslatorFacade> translator,
                       const std::string& sourceLang = "ru",
                       const std::string& targetLang = "en")
        : TextDocumentDecorator(std::move(document)),
          sourceLanguage_(sourceLang),
          targetLanguage_(targetLang),
          translator_(translator) {}

    std::string getContent() const override {
        std::string originalContent = document_->getContent();
        if (translator_) {
            return translator_->translate(originalContent, sourceLanguage_, targetLanguage_);
        }
        return originalContent;
    }

    void setLanguages(const std::string& sourceLang, const std::string& targetLang) {
        sourceLanguage_ = sourceLang;
        targetLanguage_ = targetLang;
    }

    std::string getSourceLanguage() const { return sourceLanguage_; }
    std::string getTargetLanguage() const { return targetLanguage_; }
    
    // Новые методы для получения информации о языках
    std::string getSourceLanguageName() const {
        if (translator_) {
            return translator_->getLanguageName(sourceLanguage_);
        }
        return sourceLanguage_;
    }
    
    std::string getTargetLanguageName() const {
        if (translator_) {
            return translator_->getLanguageName(targetLanguage_);
        }
        return targetLanguage_;
    }
};