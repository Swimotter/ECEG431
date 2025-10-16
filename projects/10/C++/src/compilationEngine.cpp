#include "compilationEngine.hpp"

#include <cassert>
#include <iostream>
#include <unordered_map>

CompilationEngine::CompilationEngine(const std::filesystem::path& inFile, const std::filesystem::path& outFile) : tokenizer(inFile)
{
    ofs.open(outFile);
}

void CompilationEngine::compileClass()
{
    compileOpenTag("class", true);

    // 'class'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::KEYWORD || tokenizer.keyword() != JackTokenizer::Keyword::CLASS) {
        throw JackCompilationException("class", token, tokenizer.line(), tokenizer.col());
    }
    compileKeyword();

    // className
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
        throw JackCompilationException("identifier", token, tokenizer.line(), tokenizer.col(), false);
    }
    className = tokenizer.identifier();
    compileIdentifier();

    // '{'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "{") {
        throw JackCompilationException("{", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    // classVarDec*
    for (std::string token; token = tokenizer.peek(), tokenizer.tokenType(token) == JackTokenizer::TokenType::KEYWORD &&
        (tokenizer.keyword(token) == JackTokenizer::Keyword::STATIC ||
         tokenizer.keyword(token) == JackTokenizer::Keyword::FIELD);) {
        compileClassVarDec();
    }
    
    // subroutineDec*
    for (std::string token; token = tokenizer.peek(), isSubroutineDec(token);) {
        compileSubroutine();
    }

    // '}'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "}") {
        throw JackCompilationException("}", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();
    
    compileCloseTag("class");
}

void CompilationEngine::compileClassVarDec()
{
    compileOpenTag("classVarDec", true);

    // 'static' | 'field'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::KEYWORD || (tokenizer.keyword() != JackTokenizer::Keyword::STATIC && tokenizer.keyword() != JackTokenizer::Keyword::FIELD)) {
        throw JackCompilationException(std::vector<std::string>{"static", "field"}, token, tokenizer.line(), tokenizer.col());
    }
    compileKeyword();

    // type
    if (std::string token = tokenizer.advance(); isType()) {
        compileKeyword();
    }
    else if (tokenizer.tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
        compileIdentifier();
    }
    else {
        throw JackCompilationException({"int", "char", "boolean", "identifier"}, tokenizer.token(), tokenizer.line(), tokenizer.col());
    }

    // varName
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
        throw JackCompilationException("identifier", token, tokenizer.line(), tokenizer.col(), false);
    }
    compileIdentifier();

    // (',' varName)*
    for (std::string token; token = tokenizer.peek(), tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == ",";) {
        // ','
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ",") {
            throw JackCompilationException(",", token, tokenizer.line(), tokenizer.col());
        }
        compileSymbol();

        // varName
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
            throw JackCompilationException("identifier", token, tokenizer.line(), tokenizer.col(), false);
        }
        compileIdentifier();
    }

    // ';'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ";") {
        throw JackCompilationException(";", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    compileCloseTag("classVarDec");
}

void CompilationEngine::compileSubroutine()
{
    compileOpenTag("subroutineDec", true);

    // 'constructor' | 'function' | 'method'
    if (std::string token = tokenizer.advance(); !isSubroutineDec()) {
        throw JackCompilationException({"constructor", "function", "method"}, token, tokenizer.line(), tokenizer.col());
    }
    compileKeyword();

    // 'void' | type
    if (std::string token = tokenizer.advance(); (tokenizer.tokenType() == JackTokenizer::TokenType::KEYWORD && tokenizer.keyword() == JackTokenizer::Keyword::VOID) || isType()) {
        compileKeyword();
    }
    else if (tokenizer.tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
        compileIdentifier();
    }
    else {
        throw JackCompilationException({"void", "int", "char", "boolean", "identifier"}, token, tokenizer.line(), tokenizer.col());
    }

    // suburoutineName
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
        throw JackCompilationException("identifier", token, tokenizer.line(), tokenizer.col(), false);
    }
    compileIdentifier();

    // '('
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "(") {
        throw JackCompilationException("(", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    // parameterList
    compileParameterList();

    // ')'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ")") {
        throw JackCompilationException(")", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    // subroutineBody
    compileSubroutineBody();

    compileCloseTag("subroutineDec");
}

void CompilationEngine::compileParameterList()
{
    compileOpenTag("parameterList", true);

    // ((type varName)(, type varName)*)?
    for (std::string token; token = tokenizer.peek(), isType(token);) {
        // type
        if (token = tokenizer.advance(); isType(token)) {
            compileKeyword();
        }
        else if (tokenizer.tokenType(token) == JackTokenizer::TokenType::IDENTIFIER) {
            compileIdentifier();
        }
        else {
            throw JackCompilationException({"int", "char", "boolean", "identifier"}, token, tokenizer.line(), tokenizer.col(), false);
        }

        // varName
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
            throw JackCompilationException("identifier", token, tokenizer.line(), tokenizer.col(), false);
        }
        compileIdentifier();

        // (, type varName)*
        for (; token = tokenizer.peek(), tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == ",";) {
            // ,
            if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ",") {
                throw JackCompilationException(",", token, tokenizer.line(), tokenizer.col());
            }
            compileSymbol();

            // type
            if (token = tokenizer.advance(); isType(token)) {
                compileKeyword();
            }
            else if (tokenizer.tokenType(token) == JackTokenizer::TokenType::IDENTIFIER) {
                compileIdentifier();
            }
            else {
                throw JackCompilationException({"int", "char", "boolean", "identifier"}, token, tokenizer.line(), tokenizer.col(), false);
            }

            // varName
            if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
                throw JackCompilationException("identifier", token, tokenizer.line(), tokenizer.col(), false);
            }
            compileIdentifier();
        }
    }

    compileCloseTag("parameterList");
}

void CompilationEngine::compileSubroutineBody()
{
    compileOpenTag("subroutineBody", true);

    // '{'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "{") {
        throw JackCompilationException("{", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    // varDec*
    for (std::string token; token = tokenizer.peek(), tokenizer.tokenType(token) == JackTokenizer::TokenType::KEYWORD && tokenizer.keyword(token) == JackTokenizer::Keyword::VAR;) {
        compileVarDec();
    }

    // statements
    compileStatements();

    // '}'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "}") {
        throw JackCompilationException("}", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    compileCloseTag("subroutineBody");
}

void CompilationEngine::compileVarDec()
{
    compileOpenTag("varDec", true);

    // 'var'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::KEYWORD || tokenizer.keyword() != JackTokenizer::Keyword::VAR) {
        throw JackCompilationException("var", token, tokenizer.line(), tokenizer.col());
    }
    compileKeyword();

    // type
    if (std::string token = tokenizer.advance(); isType()) {
        compileKeyword();
    }
    else if (tokenizer.tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
        compileIdentifier();
    }
    else {
        throw JackCompilationException({"int", "char", "boolean", "identifier"}, tokenizer.token(), tokenizer.line(), tokenizer.col());
    }

    // varName
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
        throw JackCompilationException("identifier", token, tokenizer.line(), tokenizer.col(), false);
    }
    compileIdentifier();

    // (',' type varName)*
    for (std::string token; token = tokenizer.peek(), tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == ",";) {
        // ','
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ",") {
            throw JackCompilationException(",", token, tokenizer.line(), tokenizer.col());
        }
        compileSymbol();

        // varName
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
            throw JackCompilationException("identifier", token, tokenizer.line(), tokenizer.col(), false);
        }
        compileIdentifier();
    }

    // ';'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ";") {
        throw JackCompilationException(";", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();
    
    compileCloseTag("varDec");
}

void CompilationEngine::compileStatements()
{
    compileOpenTag("statements", true);
    
    // statement*
    for (std::string token; token = tokenizer.peek(), isStatement(token);) {
        // letStatement | ifStatement | whileStatement | doStatement | returnStatement
        switch (tokenizer.keyword(token)) {
            case JackTokenizer::Keyword::LET:
                compileLet();
                break;

            case JackTokenizer::Keyword::IF:
                compileIf();
                break;

            case JackTokenizer::Keyword::WHILE:
                compileWhile();
                break;

            case JackTokenizer::Keyword::DO:
                compileDo();
                break;

            case JackTokenizer::Keyword::RETURN:
                compileReturn();
                break;

            default:
                return;
        }
    }

    compileCloseTag("statements");
}

void CompilationEngine::compileLet()
{
    compileOpenTag("letStatement", true);

    // 'let'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::KEYWORD || tokenizer.keyword() != JackTokenizer::Keyword::LET) {
        throw JackCompilationException("let", token, tokenizer.line(), tokenizer.col());
    }
    compileKeyword();

    // varName
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
        throw JackCompilationException("identifier", token, tokenizer.line(), tokenizer.col(), false);
    }
    compileIdentifier();

    // ('[' expression ']')?
    for (std::string token; token = tokenizer.peek(), tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "[";) {
        // '['
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "[") {
            throw JackCompilationException("[", token, tokenizer.line(), tokenizer.col());
        }
        compileSymbol();

        // expression
        compileExpression();

        // ']'
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "]") {
            throw JackCompilationException("]", token, tokenizer.line(), tokenizer.col());
        }
        compileSymbol();
    }

    // '='
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "=") {
        throw JackCompilationException("=", tokenizer.token(), tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    // expression
    compileExpression();
    
    // ';'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ";") {
        throw JackCompilationException(";", tokenizer.token(), tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    compileCloseTag("letStatement");
}

void CompilationEngine::compileIf()
{
    compileOpenTag("ifStatement", true);

    // 'if'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::KEYWORD || tokenizer.keyword() != JackTokenizer::Keyword::IF) {
        throw JackCompilationException("if", token, tokenizer.line(), tokenizer.col());
    }
    compileKeyword();

    // '('
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "(") {
        throw JackCompilationException("(", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    // expression
    compileExpression();

    // ')'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ")") {
        throw JackCompilationException(")", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    // '{'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "{") {
        throw JackCompilationException("{", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    // expression
    compileStatements();

    // '}'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "}") {
        throw JackCompilationException("}", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    // ('else' '{' statements '}')?
    for (std::string token; token = tokenizer.peek(), tokenizer.tokenType(token) == JackTokenizer::TokenType::KEYWORD && tokenizer.keyword(token) == JackTokenizer::Keyword::ELSE;) {
        // 'else'
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::KEYWORD || tokenizer.keyword() != JackTokenizer::Keyword::ELSE) {
            throw JackCompilationException("else", token, tokenizer.line(), tokenizer.col());
        }
        compileKeyword();

        // '{'
        if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "{") {
            throw JackCompilationException("{", token, tokenizer.line(), tokenizer.col());
        }
        compileSymbol();

        // expression
        compileStatements();

        // '}'
        if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "}") {
            throw JackCompilationException("}", token, tokenizer.line(), tokenizer.col());
        }
        compileSymbol();
    }

    compileCloseTag("ifStatement");
}

void CompilationEngine::compileWhile()
{
    compileOpenTag("whileStatement", true);

    // 'while'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::KEYWORD || tokenizer.keyword() != JackTokenizer::Keyword::WHILE) {
        throw JackCompilationException("while", token, tokenizer.line(), tokenizer.col());
    }
    compileKeyword();

    // '('
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "(") {
        throw JackCompilationException("(", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    // expression
    compileExpression();

    // ')'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ")") {
        throw JackCompilationException(")", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();
    
    // '{'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "{") {
        throw JackCompilationException("{", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    // expression
    compileStatements();

    // '}'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "}") {
        throw JackCompilationException("}", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    compileCloseTag("whileStatement");
}

void CompilationEngine::compileDo()
{
    compileOpenTag("doStatement", true);

    // 'do'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::KEYWORD || tokenizer.keyword() != JackTokenizer::Keyword::DO) {
        throw JackCompilationException("do", token, tokenizer.line(), tokenizer.col());
    }
    compileKeyword();
    
    // NOTE: compile expression handles the weird subroutineCall as one of it's cases
    // subroutineName | (className | varName)
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
        throw JackCompilationException("identifier", tokenizer.token(), tokenizer.line(), tokenizer.col(), false);
    }
    compileIdentifier();

    // ('(' expressionList ')') | ('.' subroutineName '(' expressionList ')')
    if (std::string token = tokenizer.peek(); tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "(") {
        // '('
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "(") {
            throw JackCompilationException("(", token, tokenizer.line(), tokenizer.col());
        }
        compileSymbol();

        // expressionList
        compileExpressionList();

        // ')'
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ")") {
            throw JackCompilationException(")", token, tokenizer.line(), tokenizer.col());
        }
        compileSymbol();
    }
    else if (tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == ".") {
        // '.'
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ".") {
            throw JackCompilationException(".", token, tokenizer.line(), tokenizer.col());
        }
        compileSymbol();

        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
            throw JackCompilationException("identifier", token, tokenizer.line(), tokenizer.col());
        }
        compileIdentifier();
        
        // '('
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "(") {
            throw JackCompilationException("(", token, tokenizer.line(), tokenizer.col());
        }
        compileSymbol();

        // expressionList
        compileExpressionList();

        // ')'
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ")") {
            throw JackCompilationException(")", token, tokenizer.line(), tokenizer.col());
        }
        compileSymbol();
    }

    // ';'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ";") {
        throw JackCompilationException(";", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();

    compileCloseTag("doStatement");
}

void CompilationEngine::compileReturn()
{
    compileOpenTag("returnStatement", true);

    // 'return'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::KEYWORD || tokenizer.keyword() != JackTokenizer::Keyword::RETURN) {
        throw JackCompilationException("return", token, tokenizer.line(), tokenizer.col());
    }
    compileKeyword();
    
    // expression?
    for (std::string token; token = tokenizer.peek(), isExpression(token);) {
        compileExpression();
    }

    // ';'
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ";") {
        throw JackCompilationException(";", token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();
    
    compileCloseTag("returnStatement");
}

void CompilationEngine::compileExpression()
{
    compileOpenTag("expression", true);

    // term
    compileTerm();

    // (op term)*
    for (std::string token; token = tokenizer.peek(), isOp(token);) {
        // op
        if (token = tokenizer.advance(); !isOp()) {
            throw JackCompilationException({"+", "-", "*", "/", "&", "|", "<", ">", "="}, token, tokenizer.line(), tokenizer.col());
        }
        compileSymbol();

        // term
        compileTerm();
    }

    compileCloseTag("expression");
}

void CompilationEngine::compileTerm()
{
    compileOpenTag("term", true);

    // integerConstant | stringConstant | keywordConstant | varName | varname '[' expression ']' | '(' expression ')' | unaryOp term | subroutineCall
    if (std::string token = tokenizer.peek(); tokenizer.tokenType(token) == JackTokenizer::TokenType::INT_CONST) {
        // integerConstant
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::INT_CONST) {
            throw JackCompilationException("integerConstant", tokenizer.token(), tokenizer.line(), tokenizer.col(), false);
        }
        compileIntVal();
    }
    else if (tokenizer.tokenType(token) == JackTokenizer::TokenType::STRING_CONST) {
        // stringConstant
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::STRING_CONST) {
            throw JackCompilationException("stringConstant", tokenizer.token(), tokenizer.line(), tokenizer.col(), false);
        }
        compileStringVal();
    }
    else if (isKeywordConstant(token)) {
        // keywordConstant
        if (token = tokenizer.advance(); !isKeywordConstant()) {
            throw JackCompilationException("keywordConstant", tokenizer.token(), tokenizer.line(), tokenizer.col(), false);
        }
        compileKeyword();
    }
    else if (tokenizer.tokenType(token) == JackTokenizer::TokenType::IDENTIFIER) {
        // varName | subroutineName | (className | varName)
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
            throw JackCompilationException("identifier", tokenizer.token(), tokenizer.line(), tokenizer.col(), false);
        }
        compileIdentifier();

        // ('[' expression ']') | ('(' expressionList ')') | ('.' subroutineName '(' expressionList ')')
        if (token = tokenizer.peek(); tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "[") {
            // '['
            if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "[") {
                throw JackCompilationException("[", token, tokenizer.line(), tokenizer.col());
            }
            compileSymbol();

            // expression
            compileExpression();

            // ']'
            if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "]") {
                throw JackCompilationException("]", token, tokenizer.line(), tokenizer.col());
            }
            compileSymbol();
        }
        else if (tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "(") {
            // '('
            if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "(") {
                throw JackCompilationException("(", token, tokenizer.line(), tokenizer.col());
            }
            compileSymbol();

            // expressionList
            compileExpression();

            // ')'
            if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ")") {
                throw JackCompilationException(")", token, tokenizer.line(), tokenizer.col());
            }
            compileSymbol();
        }
        else if (tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == ".") {
            // '.'
            if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ".") {
                throw JackCompilationException(".", token, tokenizer.line(), tokenizer.col());
            }
            compileSymbol();

            if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
                throw JackCompilationException("identifier", token, tokenizer.line(), tokenizer.col());
            }
            compileIdentifier();
            
            // '('
            if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "(") {
                throw JackCompilationException("(", token, tokenizer.line(), tokenizer.col());
            }
            compileSymbol();

            // expressionList
            compileExpressionList();

            // ')'
            if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ")") {
                throw JackCompilationException(")", token, tokenizer.line(), tokenizer.col());
            }
            compileSymbol();
        }
    }
    else if (tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "(") {
        // '('
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != "(") {
            throw JackCompilationException("(", token, tokenizer.line(), tokenizer.col());
        }
        compileSymbol();

        // expression
        compileExpression();

        // ')'
        if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ")") {
            throw JackCompilationException(")", tokenizer.token(), tokenizer.line(), tokenizer.col());
        }
        compileSymbol();
    }
    else if (isUnaryOp(token)) {
        // unaryOp
        if (token = tokenizer.advance(); !isUnaryOp()) {
            throw JackCompilationException(std::vector<std::string>{"-", "~"}, tokenizer.token(), tokenizer.line(), tokenizer.col());
        }
        compileSymbol();

        // term
        compileTerm();
    }
    else {
        throw JackCompilationException({"integer", "string", "true", "false", "null", "this", "varName", "(", "-", "~", "subroutineCall"}, tokenizer.token(), tokenizer.line(), tokenizer.col());
    }

    compileCloseTag("term");
}

int CompilationEngine::compileExpressionList()
{
    int expressions = 0;

    compileOpenTag("expressionList", true);

    // (expression (',' expression)*)?
    for (std::string token; token = tokenizer.peek(), isExpression(token);) {
        // expression
        compileExpression();

        expressions++;

        // (',' expression)*
        for (;token = tokenizer.peek(), tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == ",";) {
            // ','
            if (token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || token != ",") {
                throw JackCompilationException(",", token, tokenizer.line(), tokenizer.col());
            }
            compileSymbol();

            // expression
            compileExpression();

            expressions++;
        }
    }

    compileCloseTag("expressionList");

    return expressions;
}

void CompilationEngine::writeIndent()
{
    for (int i = 0; i < indentLevel; i++) {
        // NOTE: We use 2 spaces per .NET defaults
        ofs << "  ";
    }
}

void CompilationEngine::compileOpenTag(const std::string &tag, bool trailingNewLine)
{
    writeIndent();
    ofs << "<" << tag << ">";
    if (trailingNewLine) {
        ofs << std::endl;
    }
    indentLevel++;
}

void CompilationEngine::compileCloseTag(const std::string &tag, bool indent)
{
    indentLevel--;
    if (indent) {
        writeIndent();
    }
    ofs << "</" << tag << ">" << std::endl;
}

void CompilationEngine::compileKeyword()
{
    std::string keyword;
    switch (tokenizer.keyword())
    {
        case JackTokenizer::Keyword::CLASS:
            keyword = "class";
            break;

        case JackTokenizer::Keyword::METHOD:
            keyword = "method";
            break;

        case JackTokenizer::Keyword::FUNCTION:
            keyword = "function";
            break;

        case JackTokenizer::Keyword::CONSTRUCTOR:
            keyword = "constructor";
            break;

        case JackTokenizer::Keyword::INT:
            keyword = "int";
            break;

        case JackTokenizer::Keyword::BOOLEAN:
            keyword = "boolean";
            break;

        case JackTokenizer::Keyword::CHAR:
            keyword = "char";
            break;

        case JackTokenizer::Keyword::VOID:
            keyword = "void";
            break;

        case JackTokenizer::Keyword::VAR:
            keyword = "var";
            break;

        case JackTokenizer::Keyword::STATIC:
            keyword = "static";
            break;

        case JackTokenizer::Keyword::FIELD:
            keyword = "field";
            break;

        case JackTokenizer::Keyword::LET:
            keyword = "let";
            break;

        case JackTokenizer::Keyword::DO:
            keyword = "do";
            break;

        case JackTokenizer::Keyword::IF:
            keyword = "if";
            break;

        case JackTokenizer::Keyword::ELSE:
            keyword = "else";
            break;

        case JackTokenizer::Keyword::WHILE:
            keyword = "while";
            break;

        case JackTokenizer::Keyword::RETURN:
            keyword = "return";
            break;

        case JackTokenizer::Keyword::TRUE:
            keyword = "true";
            break;

        case JackTokenizer::Keyword::FALSE:
            keyword = "false";
            break;

        case JackTokenizer::Keyword::NULL_TOKEN:
            keyword = "null";
            break;

        case JackTokenizer::Keyword::THIS:
            keyword = "this";
            break;
        
        default:
            return;
    }
    
    compileOpenTag("keyword");
    ofs << " " << keyword << " ";
    compileCloseTag("keyword", false);
}

void CompilationEngine::compileSymbol()
{
    std::string symbol;
    // XML escape characters
    switch (tokenizer.symbol()) {
        case '<':
            symbol = "&lt;";
            break;

        case '>':
            symbol = "&gt;";
            break;

        case '&':
            symbol = "&amp;";
            break;

        case '"':
            symbol = "&quot;";
            break;

        case '\'':
            symbol = "&apos;";
            break;

        default:
            symbol = std::string(1, tokenizer.symbol());
            break;
    }

    compileOpenTag("symbol");
    ofs << " " << symbol << " ";
    compileCloseTag("symbol", false);
}

void CompilationEngine::compileIdentifier()
{
    compileOpenTag("identifier");
    ofs << " " << tokenizer.identifier() << " ";
    compileCloseTag("identifier", false);
}

void CompilationEngine::compileIntVal()
{
    compileOpenTag("integerConstant");
    ofs << " " << tokenizer.intVal() << " ";
    compileCloseTag("integerConstant", false);
}

void CompilationEngine::compileStringVal()
{
    compileOpenTag("stringConstant");
    ofs << " " << tokenizer.stringVal() << " ";
    compileCloseTag("stringConstant", false);
}

bool CompilationEngine::isSubroutineDec() const
{
    return isSubroutineDec(tokenizer.token());
}

bool CompilationEngine::isSubroutineDec(const std::string &token) const
{
    return tokenizer.tokenType(token) == JackTokenizer::TokenType::KEYWORD &&
        (tokenizer.keyword(token) == JackTokenizer::Keyword::CONSTRUCTOR ||
         tokenizer.keyword(token) == JackTokenizer::Keyword::FUNCTION ||
         tokenizer.keyword(token) == JackTokenizer::Keyword::METHOD);
}

// NOTE: This only checks for predefined types
bool CompilationEngine::isType() const
{
    return isType(tokenizer.token());
}

bool CompilationEngine::isType(const std::string& token) const
{
    return tokenizer.tokenType(token) == JackTokenizer::TokenType::KEYWORD &&
        (tokenizer.keyword(token) == JackTokenizer::Keyword::INT ||
        tokenizer.keyword(token) == JackTokenizer::Keyword::CHAR ||
        tokenizer.keyword(token) == JackTokenizer::Keyword::BOOLEAN);
}

bool CompilationEngine::isStatement() const
{
    return isStatement(tokenizer.token());
}

bool CompilationEngine::isStatement(const std::string &token) const
{
    return tokenizer.tokenType(token) == JackTokenizer::TokenType::KEYWORD &&
        (tokenizer.keyword(token) == JackTokenizer::Keyword::LET ||
        tokenizer.keyword(token) == JackTokenizer::Keyword::IF ||
        tokenizer.keyword(token) == JackTokenizer::Keyword::WHILE ||
        tokenizer.keyword(token) == JackTokenizer::Keyword::DO ||
        tokenizer.keyword(token) == JackTokenizer::Keyword::RETURN);
}

bool CompilationEngine::isKeywordConstant() const
{
    return isKeywordConstant(tokenizer.token());
}

bool CompilationEngine::isKeywordConstant(const std::string &token) const
{
    return tokenizer.tokenType(token) == JackTokenizer::TokenType::KEYWORD &&
        (tokenizer.keyword(token) == JackTokenizer::Keyword::TRUE ||
        tokenizer.keyword(token) == JackTokenizer::Keyword::FALSE ||
        tokenizer.keyword(token) == JackTokenizer::Keyword::NULL_TOKEN ||
        tokenizer.keyword(token) == JackTokenizer::Keyword::THIS);
}

bool CompilationEngine::isOp() const
{
    return isOp(tokenizer.token());
}

bool CompilationEngine::isOp(const std::string &token) const
{
    return tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL &&
        (tokenizer.symbol(token) == '+' ||
        tokenizer.symbol(token) == '-' ||
        tokenizer.symbol(token) == '*' ||
        tokenizer.symbol(token) == '/' ||
        tokenizer.symbol(token) == '&' ||
        tokenizer.symbol(token) == '|' ||
        tokenizer.symbol(token) == '<' ||
        tokenizer.symbol(token) == '>' ||
        tokenizer.symbol(token) == '=');
}

bool CompilationEngine::isUnaryOp() const
{
    return isUnaryOp(tokenizer.token());
}

bool CompilationEngine::isUnaryOp(const std::string &token) const
{
    return tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL &&
        (tokenizer.symbol(token) == '-' ||
        tokenizer.symbol(token) == '~');
}

bool CompilationEngine::isExpression() const
{
    return isExpression(tokenizer.token());
}

bool CompilationEngine::isExpression(const std::string &token) const
{
    return tokenizer.tokenType(token) == JackTokenizer::TokenType::INT_CONST ||
        tokenizer.tokenType(token) == JackTokenizer::TokenType::STRING_CONST ||
        isKeywordConstant(token) ||
        tokenizer.tokenType(token) == JackTokenizer::TokenType::IDENTIFIER ||
        (tokenizer.tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "(") ||
        isUnaryOp(token);
}
