#pragma once TEXT_DOCUMENT_DECORATOR_HPP
#define TEXT_DOCUMENT_DECORATOR_HPP

#include "TextDocument.hpp"

class TextDocumentDecorator : public TextDocument {
protected:
    std::unique_ptr<TextDocument> document_;

public:
    explicit TextDocumentDecorator(std::unique_ptr<TextDocument> document)
        : document_(std::move(document)) {}

    std::string getContent() const override {
        return document_->getContent();
    }

    void setContent(const std::string& content) override {
        document_->setContent(content);
    }
};