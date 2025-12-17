#pragma once ENCRYPTION_DECORATOR_HPP
#define ENCRYPTION_DECORATOR_HPP

#include "TextDocumentDecorator.hpp"

class EncryptionDecorator : public TextDocumentDecorator {
public:
    explicit EncryptionDecorator(std::unique_ptr<TextDocument> document)
        : TextDocumentDecorator(std::move(document)) {}

    std::string getContent() const override {
        std::string content = document_->getContent();
        return encryptText(content);
    }

    void setContent(const std::string& content) override {
        document_->setContent(decryptText(content));
    }

private:
    std::string encryptText(const std::string& text) const {
        std::string encrypted = "[ENCRYPTED: " + text + " :ENCRYPTED]";
        return encrypted;
    }

    std::string decryptText(const std::string& text) const {
        if (text.find("[ENCRYPTED: ") == 0 && text.rfind(" :ENCRYPTED]") != std::string::npos) {
            return text.substr(11, text.length() - 24);
        }
        return text;
    }
};