#pragma once

#include <filesystem>
#include <fstream>
#include <string>


class JackTokenizer
{
    public:
        enum class TokenType
        {
            NONE,
            KEYWORD,
            SYMBOL,
            IDENTIFIER,
            INT_CONST,
            STRING_CONST,
        };

        enum class Keyword
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

        static std::string keywordToString(JackTokenizer::Keyword keyword);
        static JackTokenizer::TokenType tokenType(const std::string& token);
        static JackTokenizer::Keyword keyword(const std::string& token);
        static char symbol(const std::string& token);
        static const std::string& identifier(const std::string& token);
        static int intVal(const std::string& token);
        static std::string stringVal(const std::string& token);

        JackTokenizer(const std::filesystem::path& inFile);

        bool hasMoreTokens();
        const std::string& peek();
        const std::string& advance();
        JackTokenizer::TokenType tokenType() const;
        JackTokenizer::Keyword keyword() const;
        char symbol() const;
        const std::string& identifier() const;
        int intVal() const;
        std::string stringVal() const;

        int line() const;
        int col() const;
        const std::string& token() const;

    private:
        static bool isIdentifier(const std::string& token);
        static bool isInteger(const std::string& token);
        static bool isString(const std::string& token);

        std::ifstream ifs;
        std::string curToken;
        std::string nextToken;

        int lineNum = 1;
        int colNum = 1;

        bool peeked = false;

        bool isIdentifier() const;
        bool isInteger() const;
        bool isString() const;
        
        std::string readNextToken();
};
