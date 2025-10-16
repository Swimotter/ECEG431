#include "compilationEngine.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_map>

constexpr std::initializer_list<char> op = {'+', '-', '*', '/', '&', '|', '<', '>', '='};
constexpr std::initializer_list<char> unaryOp = {'-', '~'};
constexpr std::initializer_list<JackTokenizer::Keyword> keywordConstant = {JackTokenizer::Keyword::TRUE, JackTokenizer::Keyword::FALSE, JackTokenizer::Keyword::NULL_TOKEN, JackTokenizer::Keyword::THIS};

CompilationEngine::CompilationEngine(const std::filesystem::path& inFile, const std::filesystem::path& outFile) : tokenizer(inFile)
{
    ofs.open(outFile);
}

void CompilationEngine::compileClass()
{
    compileOpenTag("class", true);

    // 'class'
    expectKeyword(JackTokenizer::Keyword::CLASS);

    // className
    expectIdentifier();

    // '{'
    expectSymbol('{');

    // classVarDec*
    while (peekKeyword({JackTokenizer::Keyword::STATIC, JackTokenizer::Keyword::FIELD})) {
        compileClassVarDec();
    }
    
    // subroutineDec*
    while (peekKeyword({JackTokenizer::Keyword::CONSTRUCTOR, JackTokenizer::Keyword::FUNCTION, JackTokenizer::Keyword::METHOD})) {
        compileSubroutine();
    }

    // '}'
    expectSymbol('}');
    
    compileCloseTag("class");
}

void CompilationEngine::compileClassVarDec()
{
    compileOpenTag("classVarDec", true);

    // 'static' | 'field'
    expectKeyword({JackTokenizer::Keyword::STATIC, JackTokenizer::Keyword::FIELD});

    // type
    expectTypeOrIdentifier();

    // varName
    expectIdentifier();

    // (',' varName)*
    while (peekSymbol(',')) {
        // ','
        expectSymbol(',');

        // varName
        expectIdentifier();
    }

    // ';'
    expectSymbol(';');

    compileCloseTag("classVarDec");
}

void CompilationEngine::compileSubroutine()
{
    compileOpenTag("subroutineDec", true);

    // 'constructor' | 'function' | 'method'
    expectKeyword({JackTokenizer::Keyword::CONSTRUCTOR, JackTokenizer::Keyword::FUNCTION, JackTokenizer::Keyword::METHOD});

    // 'void' | type
    if (std::string token = tokenizer.advance(); (tokenizer.tokenType() == JackTokenizer::TokenType::KEYWORD && tokenizer.keyword() == JackTokenizer::Keyword::VOID) || (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::KEYWORD &&
        (JackTokenizer::keyword(token) == JackTokenizer::Keyword::INT ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::CHAR ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::BOOLEAN))) {
        compileKeyword();
    }
    else if (tokenizer.tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
        compileIdentifier();
    }
    else {
        throw JackCompilationException({"void", "int", "char", "boolean", "identifier"}, token, tokenizer.line(), tokenizer.col());
    }

    // suburoutineName
    expectIdentifier();

    // '('
    expectSymbol('(');

    // parameterList
    compileParameterList();

    // ')'
    expectSymbol(')');

    // subroutineBody
    compileSubroutineBody();

    compileCloseTag("subroutineDec");
}

void CompilationEngine::compileParameterList()
{
    compileOpenTag("parameterList", true);

    // ((type varName)(, type varName)*)?
    std::string token;
    if (peekKeyword({JackTokenizer::Keyword::INT, JackTokenizer::Keyword::CHAR, JackTokenizer::Keyword::BOOLEAN}, &token) || JackTokenizer::tokenType(token) == JackTokenizer::TokenType::IDENTIFIER) {
        // type
        expectTypeOrIdentifier();

        // varName
        expectIdentifier();

        // (, type varName)*
        while (peekSymbol(',')) {
            // ,
            expectSymbol(',');

            // type
            expectTypeOrIdentifier();

            // varName
            expectIdentifier();
        }
    }

    compileCloseTag("parameterList");
}

void CompilationEngine::compileSubroutineBody()
{
    compileOpenTag("subroutineBody", true);

    // '{'
    expectSymbol('{');

    // varDec*
    while (peekKeyword(JackTokenizer::Keyword::VAR)) {
        compileVarDec();
    }

    // statements
    compileStatements();

    // '}'
    expectSymbol('}');

    compileCloseTag("subroutineBody");
}

void CompilationEngine::compileVarDec()
{
    compileOpenTag("varDec", true);

    // 'var'
    expectKeyword(JackTokenizer::Keyword::VAR);

    // type
    expectTypeOrIdentifier();

    // varName
    expectIdentifier();

    // (',' type varName)*
    while (peekSymbol(',')) {
        // ','
        expectSymbol(',');

        // varName
        expectIdentifier();
    }

    // ';'
    expectSymbol(';');
    
    compileCloseTag("varDec");
}

void CompilationEngine::compileStatements()
{
    compileOpenTag("statements", true);
    
    // statement*
    std::string token;
    while (peekKeyword({JackTokenizer::Keyword::LET, JackTokenizer::Keyword::IF, JackTokenizer::Keyword::WHILE, JackTokenizer::Keyword::DO, JackTokenizer::Keyword::RETURN}, &token)) {
        // letStatement | ifStatement | whileStatement | doStatement | returnStatement
        switch (JackTokenizer::keyword(token)) {
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
    expectKeyword(JackTokenizer::Keyword::LET);

    // varName
    expectIdentifier();

    // ('[' expression ']')?
    while (peekSymbol('[')) {
        // '['
        expectSymbol('[');

        // expression
        compileExpression();

        // ']'
        expectSymbol(']');
    }

    // '='
    expectSymbol('=');

    // expression
    compileExpression();
    
    // ';'
    expectSymbol(';');

    compileCloseTag("letStatement");
}

void CompilationEngine::compileIf()
{
    compileOpenTag("ifStatement", true);

    // 'if'
    expectKeyword(JackTokenizer::Keyword::IF);

    // '('
    expectSymbol('(');

    // expression
    compileExpression();

    // ')'
    expectSymbol(')');

    // '{'
    expectSymbol('{');

    // expression
    compileStatements();

    // '}'
    expectSymbol('}');

    // ('else' '{' statements '}')?
    while (peekKeyword(JackTokenizer::Keyword::ELSE)) {
        // 'else'
        expectKeyword(JackTokenizer::Keyword::ELSE);

        // '{'
        expectSymbol('{');

        // expression
        compileStatements();

        // '}'
        expectSymbol('}');
    }

    compileCloseTag("ifStatement");
}

void CompilationEngine::compileWhile()
{
    compileOpenTag("whileStatement", true);

    // 'while'
    expectKeyword(JackTokenizer::Keyword::WHILE);

    // '('
    expectSymbol('(');

    // expression
    compileExpression();

    // ')'
    expectSymbol(')');
    
    // '{'
    expectSymbol('{');

    // expression
    compileStatements();

    // '}'
    expectSymbol('}');

    compileCloseTag("whileStatement");
}

void CompilationEngine::compileDo()
{
    compileOpenTag("doStatement", true);

    // 'do'
    expectKeyword(JackTokenizer::Keyword::DO);
    
    // NOTE: compile expression handles the weird subroutineCall as one of it's cases
    // subroutineName | (className | varName)
    expectIdentifier();

    // ('(' expressionList ')') | ('.' subroutineName '(' expressionList ')')
    if (std::string token = tokenizer.peek(); JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "(") {
        // '('
        expectSymbol('(');

        // expressionList
        compileExpressionList();

        // ')'
        expectSymbol(')');
    }
    else if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == ".") {
        // '.'
        expectSymbol('.');

        // subroutineName
        expectIdentifier();
        
        // '('
        expectSymbol('(');

        // expressionList
        compileExpressionList();

        // ')'
        expectSymbol(')');
    }

    // ';'
    expectSymbol(';');

    compileCloseTag("doStatement");
}

void CompilationEngine::compileReturn()
{
    compileOpenTag("returnStatement", true);

    // 'return'
    expectKeyword(JackTokenizer::Keyword::RETURN);
    
    // expression?
    if (std::string token; token = tokenizer.peek(), JackTokenizer::tokenType(token) == JackTokenizer::TokenType::INT_CONST ||
        JackTokenizer::tokenType(token) == JackTokenizer::TokenType::STRING_CONST ||
        (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::KEYWORD &&
            (JackTokenizer::keyword(token) == JackTokenizer::Keyword::TRUE ||
            JackTokenizer::keyword(token) == JackTokenizer::Keyword::FALSE ||
            JackTokenizer::keyword(token) == JackTokenizer::Keyword::NULL_TOKEN ||
            JackTokenizer::keyword(token) == JackTokenizer::Keyword::THIS)) ||
        JackTokenizer::tokenType(token) == JackTokenizer::TokenType::IDENTIFIER ||
        (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "(") ||
        (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL &&
            (JackTokenizer::symbol(token) == '-' ||
            JackTokenizer::symbol(token) == '~'))) {
        compileExpression();
    }

    // ';'
    expectSymbol(';');
    
    compileCloseTag("returnStatement");
}

void CompilationEngine::compileExpression()
{
    compileOpenTag("expression", true);

    // term
    compileTerm();

    // (op term)*
    while (peekSymbol(op)) {
        // op
        expectSymbol(op);

        // term
        compileTerm();
    }

    compileCloseTag("expression");
}

void CompilationEngine::compileTerm()
{
    compileOpenTag("term", true);

    // integerConstant | stringConstant | keywordConstant | varName | varname '[' expression ']' | '(' expression ')' | unaryOp term | subroutineCall
    if (std::string token = tokenizer.peek(); JackTokenizer::tokenType(token) == JackTokenizer::TokenType::INT_CONST) {
        // integerConstant
        expectIntConst();
    }
    else if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::STRING_CONST) {
        // stringConstant
        expectStringConst();
    }
    else if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::KEYWORD &&
        (JackTokenizer::keyword(token) == JackTokenizer::Keyword::TRUE ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::FALSE ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::NULL_TOKEN ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::THIS)) {
        // keywordConstant
        expectKeyword(keywordConstant);
    }
    else if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::IDENTIFIER) {
        // varName | subroutineName | (className | varName)
        expectIdentifier();

        // ('[' expression ']') | ('(' expressionList ')') | ('.' subroutineName '(' expressionList ')')
        if (token = tokenizer.peek(); JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "[") {
            // '['
            expectSymbol('[');

            // expression
            compileExpression();

            // ']'
            expectSymbol(']');
        }
        else if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "(") {
            // '('
            expectSymbol('(');

            // expressionList
            compileExpression();

            // ')'
            expectSymbol(')');
        }
        else if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == ".") {
            // '.'
            expectSymbol('.');

            expectIdentifier();
            
            // '('
            expectSymbol('(');

            // expressionList
            compileExpressionList();

            // ')'
            expectSymbol(')');
        }
    }
    else if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "(") {
        // '('
        expectSymbol('(');

        // expression
        compileExpression();

        // ')'
        expectSymbol(')');
    }
    else if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL &&
        (JackTokenizer::symbol(token) == '-' ||
        JackTokenizer::symbol(token) == '~')) {
        // unaryOp
        expectSymbol(unaryOp);

        // term
        compileTerm();
    }
    else {
        throw JackCompilationException({"integerConstant", "stringConstant", "keywordConstant", "varName", "varname '[' expression ']'", "'(' expression ')'", "unaryOp term", "subroutineCall"}, token, tokenizer.line(), tokenizer.col());
    }

    compileCloseTag("term");
}

int CompilationEngine::compileExpressionList()
{
    int expressions = 0;

    compileOpenTag("expressionList", true);

    // (expression (',' expression)*)?
    if (std::string token; token = tokenizer.peek(), JackTokenizer::tokenType(token) == JackTokenizer::TokenType::INT_CONST ||
        JackTokenizer::tokenType(token) == JackTokenizer::TokenType::STRING_CONST ||
        (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::KEYWORD &&
            (JackTokenizer::keyword(token) == JackTokenizer::Keyword::TRUE ||
            JackTokenizer::keyword(token) == JackTokenizer::Keyword::FALSE ||
            JackTokenizer::keyword(token) == JackTokenizer::Keyword::NULL_TOKEN ||
            JackTokenizer::keyword(token) == JackTokenizer::Keyword::THIS)) ||
        JackTokenizer::tokenType(token) == JackTokenizer::TokenType::IDENTIFIER ||
        (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "(") ||
        (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL &&
            (JackTokenizer::symbol(token) == '-' ||
            JackTokenizer::symbol(token) == '~'))) {
        // expression
        compileExpression();

        expressions++;

        // (',' expression)*
        while (peekSymbol(',')) {
            // ','
            expectSymbol(',');

            // expression
            compileExpression();

            expressions++;
        }
    }

    compileCloseTag("expressionList");

    return expressions;
}

void CompilationEngine::expectKeyword(JackTokenizer::Keyword expected)
{
    return expectKeyword({expected});
}

void CompilationEngine::expectKeyword(std::initializer_list<JackTokenizer::Keyword> expected)
{
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::KEYWORD || std::find(expected.begin(), expected.end(), tokenizer.keyword()) == expected.end()) {
        std::vector<std::string> expectedStrs;
        for (JackTokenizer::Keyword keyword : expected) {
            expectedStrs.push_back(JackTokenizer::keywordToString(keyword));
        }

        throw JackCompilationException(expectedStrs, token, tokenizer.line(), tokenizer.col());
    }
    compileKeyword();
}

void CompilationEngine::expectSymbol(char expected)
{
    return expectSymbol({expected});
}

void CompilationEngine::expectSymbol(std::initializer_list<char> expected)
{
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || std::find(expected.begin(), expected.end(), tokenizer.symbol()) == expected.end()) {
        std::vector<std::string> expectedStrs;
        for (char symbol : expected) {
            expectedStrs.push_back(std::string(1, symbol));
        }

        throw JackCompilationException(expectedStrs, token, tokenizer.line(), tokenizer.col());
    }
    compileSymbol();
}

void CompilationEngine::expectIdentifier()
{
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
        throw JackCompilationException("identifier", token, tokenizer.line(), tokenizer.col(), false);
    }
    compileIdentifier();
}

void CompilationEngine::expectIntConst()
{
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::INT_CONST) {
        throw JackCompilationException("integerConstant", token, tokenizer.line(), tokenizer.col(), false);
    }
    compileIntVal();
}

void CompilationEngine::expectStringConst()
{
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::STRING_CONST) {
        throw JackCompilationException("stringConstant", token, tokenizer.line(), tokenizer.col(), false);
    }
    compileStringVal();
}

void CompilationEngine::expectTypeOrIdentifier()
{
    if (std::string token = tokenizer.advance(); JackTokenizer::tokenType(token) == JackTokenizer::TokenType::KEYWORD &&
        (JackTokenizer::keyword(token) == JackTokenizer::Keyword::INT ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::CHAR ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::BOOLEAN)) {
        compileKeyword();
    }
    else if (tokenizer.tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
        compileIdentifier();
    }
    else {
        throw JackCompilationException({"int", "char", "boolean", "identifier"}, token, tokenizer.line(), tokenizer.col());
    }
}

bool CompilationEngine::peekKeyword(JackTokenizer::Keyword expected, std::string* const token)
{
    return peekKeyword({expected}, token);
}

bool CompilationEngine::peekKeyword(std::initializer_list<JackTokenizer::Keyword> expected, std::string* const tokenOut)
{
    std::string token = tokenizer.peek();
    if (tokenOut) {
        *tokenOut = token;
    }
    return JackTokenizer::tokenType(token) == JackTokenizer::TokenType::KEYWORD && std::find(expected.begin(), expected.end(), JackTokenizer::keyword(token)) != expected.end();
}

bool CompilationEngine::peekSymbol(char expected, std::string* const token)
{
    return peekSymbol({expected}, token);
}

bool CompilationEngine::peekSymbol(std::initializer_list<char> expected, std::string* const tokenOut)
{
    std::string token = tokenizer.peek();
    if (tokenOut) {
        *tokenOut = token;
    }
    return JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && std::find(expected.begin(), expected.end(), JackTokenizer::symbol(token)) != expected.end();
}

void CompilationEngine::writeIndent()
{
    // NOTE: We use 2 spaces per .NET defaults
    ofs << std::string(indentLevel * 2, ' ');
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
