#pragma once TEXT_DOCUMENT_HPP
#define TEXT_DOCUMENT_HPP

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

class TextDocument {
public:
    virtual ~TextDocument() = default;
    virtual std::string getContent() const = 0;
    virtual void setContent(const std::string& content) = 0;
};

// Конкретный текстовый документ
class PlainTextDocument : public TextDocument {
private:
    std::string content;

public:
    PlainTextDocument(const std::string& content_ = "") : content(content_) {}

    std::string getContent() const override {
        return content;
    }

    void setContent(const std::string& content_) override {
        content = content_;
    }
};