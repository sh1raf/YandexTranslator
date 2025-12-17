#pragma once DOCUMENT_FACTORY
#define DOCUMENT_FACTORY

#include "EncryptionDecorator.hpp"
#include "TranslatorDecorator.hpp"

class DocumentFactory {
public:
    static std::unique_ptr<TextDocument> createTranslatedDocument(
        const std::string& content,
        std::shared_ptr<YandexTranslatorFacade> translator,
        const std::string& sourceLang = "ru",
        const std::string& targetLang = "en") {
        
        auto document = std::make_unique<PlainTextDocument>(content);
        return std::make_unique<TranslatorDecorator>(
            std::move(document), translator, sourceLang, targetLang);
    }

    static std::unique_ptr<TextDocument> createEncryptedTranslatedDocument(
        const std::string& content,
        std::shared_ptr<YandexTranslatorFacade> translator,
        const std::string& sourceLang = "ru",
        const std::string& targetLang = "en") {
        
        auto document = std::make_unique<PlainTextDocument>(content);
        auto translated = std::make_unique<TranslatorDecorator>(
            std::move(document), translator, sourceLang, targetLang);
        return std::make_unique<EncryptionDecorator>(std::move(translated));
    }
};