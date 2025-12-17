#include <iostream>
#include <memory>
#include <string>
#include <limits>

#include "TextDocument.hpp"
#include "DocumentFactory.hpp"
#include "YandexTranslatorFacade.hpp"

// Функция для безопасного ввода языка с проверкой
std::string inputLanguage(const std::string& prompt, 
                         const YandexTranslatorFacade& translator, 
                         const std::string& defaultLang = "") {
    
    std::string langCode;
    while (true) {
        std::cout << prompt;
        if (!defaultLang.empty()) {
            std::cout << " (по умолчанию: " << defaultLang << " - " 
                     << translator.getLanguageName(defaultLang) << ")";
        }
        std::cout << ": ";
        
        std::getline(std::cin, langCode);
        
        // Если ввод пустой и есть язык по умолчанию
        if (langCode.empty() && !defaultLang.empty()) {
            return defaultLang;
        }
        
        // Проверка поддержки языка
        if (translator.isLanguageSupported(langCode)) {
            return langCode;
        } else {
            std::cout << "Ошибка: язык '" << langCode << "' не поддерживается.\n";
            translator.printSupportedLanguages();
        }
    }
}

int main() {
    // Конфигурация Yandex API
    std::string url = "https://translate.api.cloud.yandex.net/translate/v2/translate";
    std::string token = "Ваш_Токен";
    std::string folderID = "Ваша_папка";
    
    auto translatorFacade = std::make_shared<YandexTranslatorFacade>(url, token, folderID);
    
    std::cout << "=== Программа 'Переводчик' с Yandex API ===\n";
    
    translatorFacade->printSupportedLanguages();
    
    // Получаем языки от пользователя
    std::string sourceLang = inputLanguage("Введите код исходного языка", *translatorFacade, "ru");
    std::string targetLang = inputLanguage("Введите код целевого языка", *translatorFacade, "en");
    
    // Получаем текст от пользователя
    std::string text;
    std::cout << "\nВведите текст для перевода (" 
              << translatorFacade->getLanguageName(sourceLang) 
              << " -> " 
              << translatorFacade->getLanguageName(targetLang) 
              << "):\n> ";
    std::getline(std::cin, text);
    
    if (text.empty()) {
        text = "Пример текста для перевода";
        std::cout << "Используем текст по умолчанию: " << text << "\n";
    }
    
    std::cout << "\n=== Результаты перевода ===\n\n";
    
    // 1. Простой документ без перевода
    std::cout << "1. Оригинальный документ:\n";
    std::cout << "   Язык: " << translatorFacade->getLanguageName(sourceLang) << "\n";
    auto originalDoc = std::make_unique<PlainTextDocument>(text);
    std::cout << "   Содержимое: " << originalDoc->getContent() << "\n\n";
    
    // 2. Документ с переводом через Yandex API
    std::cout << "2. Документ с переводом:\n";
    std::cout << "   Направление: " << translatorFacade->getLanguageName(sourceLang) 
              << " -> " << translatorFacade->getLanguageName(targetLang) << "\n";
    
    auto translatedDoc = DocumentFactory::createTranslatedDocument(
        text, translatorFacade, sourceLang, targetLang);
    std::cout << "   Содержимое: " << translatedDoc->getContent() << "\n\n";
    
    // 3. Меню дополнительных опций
    std::cout << "=== Дополнительные опции ===\n";
    
    while (true) 
    {
        std::cout << "\nВыберите действие:\n";
        std::cout << "1. Изменить языки перевода\n";
        std::cout << "2. Перевести другой текст\n";
        std::cout << "3. Добавить шифрование к переводу\n";
        std::cout << "4. Обратный перевод\n";
        std::cout << "5. Выход\n";
        std::cout << "> ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") 
        {
            // Изменить языки
            sourceLang = inputLanguage("Введите новый исходный язык", *translatorFacade, sourceLang);
            targetLang = inputLanguage("Введите новый целевой язык", *translatorFacade, targetLang);
            
            // Обновить перевод с новыми языками
            auto updatedDoc = DocumentFactory::createTranslatedDocument(
                text, translatorFacade, sourceLang, targetLang);
            
            std::cout << "\nНовый перевод (" 
                      << translatorFacade->getLanguageName(sourceLang) 
                      << " -> " 
                      << translatorFacade->getLanguageName(targetLang) 
                      << "):\n";
            std::cout << "Результат: " << updatedDoc->getContent() << "\n";
            
        } 
        else if (choice == "2") 
        {
            // Новый текст
            std::cout << "\nВведите новый текст для перевода: ";
            std::getline(std::cin, text);
            
            if (!text.empty()) {
                auto newDoc = DocumentFactory::createTranslatedDocument(
                    text, translatorFacade, sourceLang, targetLang);
                
                std::cout << "Перевод: " << newDoc->getContent() << "\n";
            }
            
        } 
        else if (choice == "3")
        {
            // Добавить шифрование
            std::cout << "\nДокумент с переводом и шифрованием:\n";
            auto encryptedDoc = DocumentFactory::createEncryptedTranslatedDocument(
                text, translatorFacade, sourceLang, targetLang);
            std::cout << "Результат: " << encryptedDoc->getContent() << "\n";
            
        } 
        else if (choice == "4") 
        {
            // Обратный перевод
            std::string currentTranslation = translatedDoc->getContent();
            
            std::cout << "\nОбратный перевод (" 
                      << translatorFacade->getLanguageName(targetLang) 
                      << " -> " 
                      << translatorFacade->getLanguageName(sourceLang) 
                      << "):\n";
            
            auto backTranslatedDoc = DocumentFactory::createTranslatedDocument(
                currentTranslation, translatorFacade, targetLang, sourceLang);
            
            std::cout << "Оригинал: " << text << "\n";
            std::cout << "Перевод: " << currentTranslation << "\n";
            std::cout << "Обратно: " << backTranslatedDoc->getContent() << "\n";
            
        } 
        else if (choice == "5") 
        {
            break;
            
        } else {
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
    return 0;
}