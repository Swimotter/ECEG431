#include "jackTokenizer.hpp"

#include <algorithm>
#include <cassert>
#include <charconv>
#include <unordered_map>


static const std::unordered_map<std::string, JackTokenizer::TokenType> tokens = {
    {"class", JackTokenizer::TokenType::KEYWORD},
    {"method", JackTokenizer::TokenType::KEYWORD},
    {"function", JackTokenizer::TokenType::KEYWORD},
    {"constructor", JackTokenizer::TokenType::KEYWORD},
    {"int", JackTokenizer::TokenType::KEYWORD},
    {"boolean", JackTokenizer::TokenType::KEYWORD},
    {"char", JackTokenizer::TokenType::KEYWORD},
    {"void", JackTokenizer::TokenType::KEYWORD},
    {"var", JackTokenizer::TokenType::KEYWORD},
    {"static", JackTokenizer::TokenType::KEYWORD},
    {"field", JackTokenizer::TokenType::KEYWORD},
    {"let", JackTokenizer::TokenType::KEYWORD},
    {"do", JackTokenizer::TokenType::KEYWORD},
    {"if", JackTokenizer::TokenType::KEYWORD},
    {"else", JackTokenizer::TokenType::KEYWORD},
    {"while", JackTokenizer::TokenType::KEYWORD},
    {"return", JackTokenizer::TokenType::KEYWORD},
    {"true", JackTokenizer::TokenType::KEYWORD},
    {"false", JackTokenizer::TokenType::KEYWORD},
    {"null", JackTokenizer::TokenType::KEYWORD},
    {"this", JackTokenizer::TokenType::KEYWORD},

    {"{", JackTokenizer::TokenType::SYMBOL},
    {"}", JackTokenizer::TokenType::SYMBOL},
    {"(", JackTokenizer::TokenType::SYMBOL},
    {")", JackTokenizer::TokenType::SYMBOL},
    {"[", JackTokenizer::TokenType::SYMBOL},
    {"]", JackTokenizer::TokenType::SYMBOL},
    {".", JackTokenizer::TokenType::SYMBOL},
    {",", JackTokenizer::TokenType::SYMBOL},
    {";", JackTokenizer::TokenType::SYMBOL},
    {"+", JackTokenizer::TokenType::SYMBOL},
    {"-", JackTokenizer::TokenType::SYMBOL},
    {"*", JackTokenizer::TokenType::SYMBOL},
    {"/", JackTokenizer::TokenType::SYMBOL},
    {"&", JackTokenizer::TokenType::SYMBOL},
    {"|", JackTokenizer::TokenType::SYMBOL},
    {"<", JackTokenizer::TokenType::SYMBOL},
    {">", JackTokenizer::TokenType::SYMBOL},
    {"=", JackTokenizer::TokenType::SYMBOL},
    {"~", JackTokenizer::TokenType::SYMBOL},
};

static const std::unordered_map<std::string, JackTokenizer::Keyword> keywords = {
    {"class", JackTokenizer::Keyword::CLASS},
    {"method", JackTokenizer::Keyword::METHOD},
    {"function", JackTokenizer::Keyword::FUNCTION},
    {"constructor", JackTokenizer::Keyword::CONSTRUCTOR},
    {"int", JackTokenizer::Keyword::INT},
    {"boolean", JackTokenizer::Keyword::BOOLEAN},
    {"char", JackTokenizer::Keyword::CHAR},
    {"void", JackTokenizer::Keyword::VOID},
    {"var", JackTokenizer::Keyword::VAR},
    {"static", JackTokenizer::Keyword::STATIC},
    {"field", JackTokenizer::Keyword::FIELD},
    {"let", JackTokenizer::Keyword::LET},
    {"do", JackTokenizer::Keyword::DO},
    {"if", JackTokenizer::Keyword::IF},
    {"else", JackTokenizer::Keyword::ELSE},
    {"while", JackTokenizer::Keyword::WHILE},
    {"return", JackTokenizer::Keyword::RETURN},
    {"true", JackTokenizer::Keyword::TRUE},
    {"false", JackTokenizer::Keyword::FALSE},
    {"null", JackTokenizer::Keyword::NULL_TOKEN},
    {"this", JackTokenizer::Keyword::THIS},
};

static const std::unordered_map<JackTokenizer::Keyword, std::string> reverseKeywords = {
    {JackTokenizer::Keyword::CLASS, "class"},
    {JackTokenizer::Keyword::METHOD, "method"},
    {JackTokenizer::Keyword::FUNCTION, "function"},
    {JackTokenizer::Keyword::CONSTRUCTOR, "constructor"},
    {JackTokenizer::Keyword::INT, "int"},
    {JackTokenizer::Keyword::BOOLEAN, "boolean"},
    {JackTokenizer::Keyword::CHAR, "char"},
    {JackTokenizer::Keyword::VOID, "void"},
    {JackTokenizer::Keyword::VAR, "var"},
    {JackTokenizer::Keyword::STATIC, "static"},
    {JackTokenizer::Keyword::FIELD, "field"},
    {JackTokenizer::Keyword::LET, "let"},
    {JackTokenizer::Keyword::DO, "do"},
    {JackTokenizer::Keyword::IF, "if"},
    {JackTokenizer::Keyword::ELSE, "else"},
    {JackTokenizer::Keyword::WHILE, "while"},
    {JackTokenizer::Keyword::RETURN, "return"},
    {JackTokenizer::Keyword::TRUE, "true"},
    {JackTokenizer::Keyword::FALSE, "false"},
    {JackTokenizer::Keyword::NULL_TOKEN, "null"},
    {JackTokenizer::Keyword::THIS, "this"},
};

std::string JackTokenizer::keywordToString(JackTokenizer::Keyword keyword)
{
    if (reverseKeywords.contains(keyword)) {
        return reverseKeywords.at(keyword);
    }
    else {
        return "";
    }
}

JackTokenizer::TokenType JackTokenizer::tokenType(const std::string& token)
{
    // Keywords and symbols
    if (tokens.contains(token)) {
        return tokens.at(token);
    }
    else {
        if (isIdentifier(token)) {
            return JackTokenizer::TokenType::IDENTIFIER;
        }
        else if (isInteger(token)) {
            return JackTokenizer::TokenType::INT_CONST;
        }
        else if (isString(token)) {
            return JackTokenizer::TokenType::STRING_CONST;
        }
        else {
            return JackTokenizer::TokenType::NONE;
        }
    }
}

JackTokenizer::Keyword JackTokenizer::keyword(const std::string &token)
{
    assert(tokenType(token) == JackTokenizer::TokenType::KEYWORD);

    if (keywords.contains(token)) {
        return keywords.at(token);
    }
    else {
        return JackTokenizer::Keyword::NONE;
    }
}

char JackTokenizer::symbol(const std::string &token)
{
    assert(tokenType(token) == JackTokenizer::TokenType::SYMBOL);
    assert(token.size() == 1);

    return token[0];
}

const std::string &JackTokenizer::identifier(const std::string &token)
{
    assert(tokenType(token) == JackTokenizer::TokenType::IDENTIFIER);

    return token;
}

int JackTokenizer::intVal(const std::string &token)
{
    assert(tokenType(token) == JackTokenizer::TokenType::INT_CONST);

    return std::stoi(token);
}

std::string JackTokenizer::stringVal(const std::string &token)
{
    assert(tokenType(token) == JackTokenizer::TokenType::STRING_CONST);

    // Remove double quotes
    return token.substr(1, token.size() - 2);
}

JackTokenizer::JackTokenizer(const std::filesystem::path &inFile)
{
    ifs.open(inFile);
}

bool JackTokenizer::hasMoreTokens()
{
    return ifs.peek() != EOF;
}

const std::string &JackTokenizer::peek()
{
    if (!peeked) {
        nextToken = readNextToken();
        peeked = true;
    }

    return nextToken;
}

const std::string& JackTokenizer::advance()
{
    if (peeked) {
        curToken = nextToken;
        peeked = false;
        return curToken;
    }

    curToken = readNextToken();
    return curToken;
}

JackTokenizer::TokenType JackTokenizer::tokenType() const
{
    return tokenType(curToken);
}

JackTokenizer::Keyword JackTokenizer::keyword() const
{
    return keyword(curToken);
}

char JackTokenizer::symbol() const
{
    return symbol(curToken);
}

const std::string& JackTokenizer::identifier() const
{
    return identifier(curToken);
}

int JackTokenizer::intVal() const
{
    return intVal(curToken);
}

std::string JackTokenizer::stringVal() const
{
    return stringVal(curToken);
}

int JackTokenizer::line() const
{
    return lineNum;
}

int JackTokenizer::col() const
{
    return colNum;
}

const std::string& JackTokenizer::token() const
{
    return curToken;
}

bool JackTokenizer::isIdentifier(const std::string& token)
{
    return !std::isdigit(static_cast<unsigned char>(token.front())) && std::ranges::all_of(token, [](unsigned char c) {
        return std::isalnum(c) || c == '_';
    });
}

bool JackTokenizer::isInteger(const std::string& token)
{
    static constexpr int MIN_VALUE = 0;
    static constexpr int MAX_VALUE = 32767;

    int value{};
    auto [ptr, ec] = std::from_chars(token.data(), token.data() + token.size(), value);

    // Check entire token is numeric and no error
    if (ec != std::errc() || ptr != token.data() + token.size()) {
        return false;
    }
    
    return value >= MIN_VALUE && value <= MAX_VALUE;
}

bool JackTokenizer::isString(const std::string& token)
{
    return token.front() == '"' && token.back() == '"';
}

bool JackTokenizer::isIdentifier() const
{
    return isIdentifier(curToken);
}

bool JackTokenizer::isInteger() const
{
    return isInteger(curToken);
}

bool JackTokenizer::isString() const
{
    return isString(curToken);
}

std::string JackTokenizer::readNextToken()
{
    assert(hasMoreTokens());

    std::string token;
    char c;
    // Ignore whitespace and comments
    while (ifs.get(c)) {
        // Update pos for exception handling
        if (c == '\n') {
            lineNum++;
            colNum = 1;
        }
        else {
            colNum++;
        }

        if (std::isspace(static_cast<unsigned char>(c))) {
            continue;
        }

        // Possible comment
        if (c == '/') {
            // End line comment
            if (ifs.peek() == '/') {
                ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                // Update pos for exception handling
                lineNum++;
                colNum = 1;
                continue;
            }
            
            // Multiline comment
            if (ifs.peek() == '*') {
                ifs.get();
                char prev = '\0';
                while (ifs.get(c)) {
                    // Update pos for exception handling
                    if (c == '\n') {
                        lineNum++;
                        colNum = 1;
                    }
                    else {
                        colNum++;
                    }

                    if (prev == '*' && c == '/') {
                        break;
                    }
                    prev = c;
                }
                continue;
            }
        }
        // Not whitespace or a comment
        break;
    }

    // String
    if (c == '"') {
        token += c;
        while (ifs.get(c)) {
            token += c;
            if (c == '\n') {
                lineNum++;
                colNum = 1;
            } else {
                colNum++;
            }
            
            if (c == '"') {
                break;
            }
        }
        return token;
    }

    // Symbol
    std::string str(1, c);
    if (tokens.contains(str) && tokens.at(str) == JackTokenizer::TokenType::SYMBOL) {
        token = str;
        return token;
    }

    token += c;
    while ((c = ifs.peek()) != EOF) {
        // Stop at whitespace or symbol
        if (std::isspace(static_cast<unsigned char>(c)) || tokens.contains(std::string(1, c))) {
            break;
        }

        ifs.get(c);
        token += c;
        colNum++;
    }

    return token;
}
