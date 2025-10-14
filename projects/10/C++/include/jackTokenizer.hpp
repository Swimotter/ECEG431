#pragma once

#include <filesystem>
#include <fstream>
#include <string>


class JackTokenizer
{
    public:
        enum TokenType
        {
            NONE,
            KEYWORD,
            SYMBOL,
            IDENTIFIER,
            INT_CONST,
            STRING_CONST,
        };

        enum Keyword
        {
            NONE,
            CLASS,
            METHOD,
            FUNCTION,
            CONSTRUCTOR,
            INT,
            BOOLEAN,
            CHAR,
            VOID,
            VAR,
            STATIC,
            FIELD,
            LET,
            DO,
            IF,
            ELSE,
            WHILE,
            RETURN,
            TRUE,
            FALSE,
            NULL_TOKEN,
            THIS,
        };

        JackTokenizer(const std::filesystem::path& inFile);

        bool hasMoreTokens();
        void advance();
        const TokenType tokenType() const;
        const Keyword keyword() const;
        char symbol() const;
        const std::string& identifier() const;
        int intVal() const;
        const std::string& stringVal() const;

    private:
        std::ifstream ifs;
        std::string token;

        bool isIdentifier() const;
        bool isInteger() const;
        bool isString() const;
};
