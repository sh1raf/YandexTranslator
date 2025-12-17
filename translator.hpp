#include <iostream>
#include <string>
#include <memory>

#include "DocumentFactory.hpp"

int main() {
    std::cout << "=== Программа 'Переводчик' с использованием паттерна Декоратор ===\n\n";

    // 1. Простой документ без перевода
    std::cout << "1. Оригинальный документ:\n";
    auto originalDoc = std::make_unique<PlainTextDocument>("Hello, world!");
    std::cout << "   Содержимое: " << originalDoc->getContent() << "\n\n";

    // 2. Документ с переводом
    std::cout << "2. Документ с переводом (EN -> RU):\n";
    auto translatedDoc = DocumentFactory::createTranslatedDocument("Hello, world!");
    std::cout << "   Содержимое: " << translatedDoc->getContent() << "\n\n";

    // 3. Документ с переводом и шифрованием
    std::cout << "3. Документ с переводом и шифрованием:\n";
    auto encryptedTranslatedDoc = DocumentFactory::createEncryptedTranslatedDocument(
        "Hello, world!");
    std::cout << "   Содержимое: " << encryptedTranslatedDoc->getContent() << "\n\n";

    // 4. Динамическое изменение языка перевода
    std::cout << "4. Изменение языка перевода:\n";
    auto dynamicDoc = std::make_unique<PlainTextDocument>("Bonjour tout le monde!");
    auto translator = std::make_unique<TranslatorDecorator>(
        std::move(dynamicDoc), "fr", "ru");
    
    std::cout << "   Французский -> Русский: " << translator->getContent() << "\n";
    
    translator->setLanguages("fr", "en");
    // Для получения перевода на новый язык нужно создать новый декоратор
    // или изменить подход (в данном случае просто демонстрация)
    std::cout << "   (После смены языка на EN)\n\n";

    // 5. Цепочка декораторов
    std::cout << "5. Цепочка декораторов (шифрование -> перевод):\n";
    auto chainedDoc = std::make_unique<PlainTextDocument>("Secret message");
    auto encrypted = std::make_unique<EncryptionDecorator>(std::move(chainedDoc));
    auto translated = std::make_unique<TranslatorDecorator>(
        std::move(encrypted), "en", "ru");
    
    std::cout << "   Результат: " << translated->getContent() << "\n\n";

    std::cout << "=== Обоснование использования паттерна Декоратор ===\n";
    std::cout << "1. Гибкость: можно динамически добавлять функциональность перевода\n";
    std::cout << "2. Расширяемость: легко добавить новые декораторы (шифрование, форматирование)\n";
    std::cout << "3. Отделение ответственности: каждый декоратор выполняет одну задачу\n";
    std::cout << "4. Совместимость с API: можно обернуть любой текстовый документ\n";
    std::cout << "5. Соответствует OCP: новая функциональность без изменения существующего кода\n";

    return 0;
}