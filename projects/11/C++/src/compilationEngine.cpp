#include "compilationEngine.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_map>

constexpr std::initializer_list<char> op = {'+', '-', '*', '/', '&', '|', '<', '>', '='};
constexpr std::initializer_list<char> unaryOp = {'-', '~'};
constexpr std::initializer_list<JackTokenizer::Keyword> keywordConstant = {JackTokenizer::Keyword::TRUE, JackTokenizer::Keyword::FALSE, JackTokenizer::Keyword::NULL_TOKEN, JackTokenizer::Keyword::THIS};

CompilationEngine::CompilationEngine(const std::filesystem::path& inFile, const std::filesystem::path& outFile) : tokenizer(inFile), writer(outFile), classScope(), subroutineScope()
{
    numIf = 0;
    numWhile = 0;
}

void CompilationEngine::compileClass()
{
    // 'class'
    expectKeyword(JackTokenizer::Keyword::CLASS);

    // className
    className = expectIdentifier();

    // '{'
    expectSymbol('{');

    // classVarDec*
    while (peekKeyword({JackTokenizer::Keyword::STATIC, JackTokenizer::Keyword::FIELD})) {
        compileClassVarDec();
    }

    // subroutineDec*
    while (peekKeyword({JackTokenizer::Keyword::CONSTRUCTOR, JackTokenizer::Keyword::FUNCTION, JackTokenizer::Keyword::METHOD})) {
        compileSubroutine();
        subroutineScope.reset();
        numIf = 0;
        numWhile = 0;
    }

    // '}'
    expectSymbol('}');
}

void CompilationEngine::compileClassVarDec()
{
    // 'static' | 'field'
    JackTokenizer::Keyword kind = expectKeyword({JackTokenizer::Keyword::STATIC, JackTokenizer::Keyword::FIELD});

    // type
    std::string type = expectTypeOrIdentifier();

    // varName
    std::string name = expectIdentifier();

    defineVar(classScope, name, type, kind);

    // (',' varName)*
    while (peekSymbol(',')) {
        // ','
        expectSymbol(',');

        // varName
        name = expectIdentifier();

        defineVar(classScope, name, type, kind);
    }

    // ';'
    expectSymbol(';');
}

void CompilationEngine::compileSubroutine()
{
    // 'constructor' | 'function' | 'method'
    JackTokenizer::Keyword type = expectKeyword({JackTokenizer::Keyword::CONSTRUCTOR, JackTokenizer::Keyword::FUNCTION, JackTokenizer::Keyword::METHOD});

    // 'void' | type
    if (std::string token = tokenizer.advance(); (tokenizer.tokenType() == JackTokenizer::TokenType::KEYWORD && tokenizer.keyword() == JackTokenizer::Keyword::VOID) || (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::KEYWORD &&
        (JackTokenizer::keyword(token) == JackTokenizer::Keyword::INT ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::CHAR ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::BOOLEAN))) {
    }
    else if (tokenizer.tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
    }
    else {
        throw JackCompilationException({"void", "int", "char", "boolean", "identifier"}, token, tokenizer.line(), tokenizer.col());
    }

    // suburoutineName
    std::string name = expectIdentifier();

    // '('
    expectSymbol('(');

    // parameterList
    compileParameterList(type == JackTokenizer::Keyword::METHOD);

    // ')'
    expectSymbol(')');

    // subroutineBody
    compileSubroutineBody(type, name);
}

int CompilationEngine::compileParameterList(bool method)
{
    // ((type varName)(, type varName)*)?
    int count = 0;
    if (method) {
        defineVar(subroutineScope, "this", className, JackSymbolTable::Kind::ARG);
        count++;
    }

    std::string token;
    if (peekKeyword({JackTokenizer::Keyword::INT, JackTokenizer::Keyword::CHAR, JackTokenizer::Keyword::BOOLEAN}, &token) || JackTokenizer::tokenType(token) == JackTokenizer::TokenType::IDENTIFIER) {
        // type
        std::string type = expectTypeOrIdentifier();

        // varName
        std::string name = expectIdentifier();

        defineVar(subroutineScope, name, type, JackSymbolTable::Kind::ARG);
        count++;

        // (, type varName)*
        while (peekSymbol(',')) {
            // ,
            expectSymbol(',');

            // type
            type = expectTypeOrIdentifier();

            // varName
            name = expectIdentifier();

            defineVar(subroutineScope, name, type, JackSymbolTable::Kind::ARG);
            count++;
        }
    }

    return count;
}

void CompilationEngine::compileSubroutineBody(JackTokenizer::Keyword type, const std::string& name)
{
    // '{'
    expectSymbol('{');

    int count = 0;
    // varDec*
    while (peekKeyword(JackTokenizer::Keyword::VAR)) {
        count += compileVarDec();
    }

    writer.writeFunction(className + "." + name, count);

    if (type == JackTokenizer::Keyword::CONSTRUCTOR) {
        writer.writePush(VMWriter::Segment::CONSTANT, classScope.varCount(JackSymbolTable::Kind::FIELD));
        writer.writeCall("Memory.alloc", 1);
        writer.writePop(VMWriter::Segment::POINTER, 0);
    }
    else if (type == JackTokenizer::Keyword::METHOD) {
        writer.writePush(VMWriter::Segment::ARGUMENT, 0);
        writer.writePop(VMWriter::Segment::POINTER, 0);
    }

    // statements
    compileStatements();

    // '}'
    expectSymbol('}');
}

int CompilationEngine::compileVarDec()
{
    int count = 0;

    // 'var'
    expectKeyword(JackTokenizer::Keyword::VAR);

    // type
    std::string type = expectTypeOrIdentifier();

    // varName
    std::string name = expectIdentifier();

    defineVar(subroutineScope, name, type, JackSymbolTable::Kind::VAR);
    count++;

    // (',' type varName)*
    while (peekSymbol(',')) {
        // ','
        expectSymbol(',');

        // varName
        name = expectIdentifier();

        defineVar(subroutineScope, name, type, JackSymbolTable::Kind::VAR);
        count++;
    }

    // ';'
    expectSymbol(';');

    return count;
}

void CompilationEngine::compileStatements()
{
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
}

void CompilationEngine::compileLet()
{
    // 'let'
    expectKeyword(JackTokenizer::Keyword::LET);

    // varName
    std::string name = expectIdentifier();

    // ('[' expression ']')?
    bool array = false;
    if (peekSymbol('[')) {
        array = true;

        // '['
        expectSymbol('[');
        
        // expression
        compileExpression();
        
        // ']'
        expectSymbol(']');

        if (subroutineScope.contains(name)) {
            writer.writePush(kindToSegment(subroutineScope.kindOf(name)), subroutineScope.indexOf(name));
        }
        else if (classScope.contains(name)) {
            writer.writePush(kindToSegment(classScope.kindOf(name)), classScope.indexOf(name));
        }
        writer.writeArithmetic(VMWriter::Op::ADD);
    }

    // '='
    expectSymbol('=');

    // expression
    compileExpression();

    if (array) {
        writer.writePop(VMWriter::Segment::TEMP, 0);
        writer.writePop(VMWriter::Segment::POINTER, 1);
        writer.writePush(VMWriter::Segment::TEMP, 0);
        writer.writePop(VMWriter::Segment::THAT, 0);
    }
    else if (subroutineScope.contains(name)) {
        writer.writePop(kindToSegment(subroutineScope.kindOf(name)), subroutineScope.indexOf(name));
    }
    else if (classScope.contains(name)) {
        writer.writePop(kindToSegment(classScope.kindOf(name)), classScope.indexOf(name));
    }

    // ';'
    expectSymbol(';');
}

void CompilationEngine::compileIf()
{
    std::string trueLabel = "IF_TRUE" + std::to_string(numIf);
    std::string falseLabel = "IF_FALSE" + std::to_string(numIf);
    std::string endLabel = "IF_END" + std::to_string(numIf);
    numIf++;

    // 'if'
    expectKeyword(JackTokenizer::Keyword::IF);

    // '('
    expectSymbol('(');

    // expression
    compileExpression();

    // ')'
    expectSymbol(')');

    writer.writeIf(trueLabel);
    writer.writeGoto(falseLabel);
    writer.writeLabel(trueLabel);

    // '{'
    expectSymbol('{');

    // expression
    compileStatements();

    // '}'
    expectSymbol('}');

    // ('else' '{' statements '}')?
    if (peekKeyword(JackTokenizer::Keyword::ELSE)) {
        // 'else'
        expectKeyword(JackTokenizer::Keyword::ELSE);

        writer.writeGoto(endLabel);
        writer.writeLabel(falseLabel);

        // '{'
        expectSymbol('{');

        // expression
        compileStatements();

        // '}'
        expectSymbol('}');

        writer.writeLabel(endLabel);
    }
    else {
        writer.writeLabel(falseLabel);
    }
}

void CompilationEngine::compileWhile()
{
    std::string whileLabel = "WHILE_EXP" + std::to_string(numWhile);
    std::string endLabel = "WHILE_END" + std::to_string(numWhile);
    numWhile++;

    // 'while'
    expectKeyword(JackTokenizer::Keyword::WHILE);

    writer.writeLabel(whileLabel);

    // '('
    expectSymbol('(');

    // expression
    compileExpression();

    // ')'
    expectSymbol(')');

    writer.writeArithmetic(VMWriter::Op::NOT);
    writer.writeIf(endLabel);

    // '{'
    expectSymbol('{');

    // expression
    compileStatements();

    // '}'
    expectSymbol('}');

    writer.writeGoto(whileLabel);
    writer.writeLabel(endLabel);
}

void CompilationEngine::compileDo()
{
    // 'do'
    expectKeyword(JackTokenizer::Keyword::DO);
    
    // NOTE: compile expression handles the weird subroutineCall as one of it's cases
    // subroutineName | (className | varName)
    std::string name1 = expectIdentifier();
    std::string name2;

    int count = 0;

    // ('(' expressionList ')') | ('.' subroutineName '(' expressionList ')')
    if (std::string token = tokenizer.peek(); JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "(") {
        name2 = name1;
        name1 = className;

        writer.writePush(VMWriter::Segment::POINTER, 0);
        count++;
    }
    else if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == ".") {
        // '.'
        expectSymbol('.');

        // subroutineName
        name2 = expectIdentifier();

        if (subroutineScope.contains(name1)) {
        std::cout << name1 << std::endl;
            writer.writePush(kindToSegment(subroutineScope.kindOf(name1)), subroutineScope.indexOf(name1));
            name1 = subroutineScope.typeOf(name1);
            count++;
        }
        else if (classScope.contains(name1)) {
            writer.writePush(kindToSegment(classScope.kindOf(name1)), classScope.indexOf(name1));
            name1 = classScope.typeOf(name1);
            count++;
        }
    }
    else {
        throw JackCompilationException(std::vector{std::string("."), std::string("(expressionList)")}, token, tokenizer.line(), tokenizer.col());
    }

    // '('
    expectSymbol('(');

    // expressionList
    count += compileExpressionList();

    // ')'
    expectSymbol(')');

    writer.writeCall(name1 + "." + name2, count);
    writer.writePop(VMWriter::Segment::TEMP, 0);

    // ';'
    expectSymbol(';');
}

void CompilationEngine::compileReturn()
{
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
    else {
        writer.writePush(VMWriter::Segment::CONSTANT, 0);
    }

    writer.writeReturn();

    // ';'
    expectSymbol(';');
}

void CompilationEngine::compileExpression()
{
    // term
    compileTerm();

    // (op term)*
    while (peekSymbol(op)) {
        // op
        char symbol = expectSymbol(op);

        // term
        compileTerm();

        opType(symbol);
    }
}

void CompilationEngine::compileTerm()
{
    // integerConstant | stringConstant | keywordConstant | varName | varname '[' expression ']' | '(' expression ')' | unaryOp term | subroutineCall
    if (std::string token = tokenizer.peek(); JackTokenizer::tokenType(token) == JackTokenizer::TokenType::INT_CONST) {
        int val = expectIntConst();
        // integerConstant
        writer.writePush(VMWriter::Segment::CONSTANT, val);
    }
    else if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::STRING_CONST) {
        // stringConstant
        std::string string = expectStringConst();
        writer.writePush(VMWriter::Segment::CONSTANT, string.length());
        writer.writeCall("String.new", 1);
        for (char c : string) {
            writer.writePush(VMWriter::Segment::CONSTANT, c);
            writer.writeCall("String.appendChar", 2);
        }
    }
    else if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::KEYWORD &&
        (JackTokenizer::keyword(token) == JackTokenizer::Keyword::TRUE ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::FALSE ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::NULL_TOKEN ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::THIS)) {
        // keywordConstant
        JackTokenizer::Keyword keyword = expectKeyword(keywordConstant);
        switch (keyword)
        {
            case JackTokenizer::Keyword::NULL_TOKEN:
            case JackTokenizer::Keyword::FALSE:
                writer.writePush(VMWriter::Segment::CONSTANT, 0);
                break;

            case JackTokenizer::Keyword::TRUE:
                writer.writePush(VMWriter::Segment::CONSTANT, 0);
                writer.writeArithmetic(VMWriter::Op::NOT);
                break;

            case JackTokenizer::Keyword::THIS:
                writer.writePush(VMWriter::Segment::POINTER, 0);

            default:
                break;
        }
    }
    else if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::IDENTIFIER) {
        // varName | subroutineName | (className | varName)
        std::string name1 = expectIdentifier();

        // ('[' expression ']') | ('(' expressionList ')') | ('.' subroutineName '(' expressionList ')')
        if (token = tokenizer.peek(); JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "[") {
            // '['
            expectSymbol('[');

            // expression
            compileExpression();

            // ']'
            expectSymbol(']');

            if (subroutineScope.contains(name1)) {
                writer.writePush(kindToSegment(subroutineScope.kindOf(name1)), subroutineScope.indexOf(name1));
            }
            else if (classScope.contains(name1)) {
                writer.writePush(kindToSegment(classScope.kindOf(name1)), classScope.indexOf(name1));
            }
            writer.writeArithmetic(VMWriter::Op::ADD);
            writer.writePop(VMWriter::Segment::POINTER, 1);
            writer.writePush(VMWriter::Segment::THAT, 0);
        }
        else if ((JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "(") || (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == ".")){
            int count = 0;
            std::string name2;

            if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == "(") {
                name2 = name1;
                name1 =  className;
                writer.writePush(VMWriter::Segment::POINTER, 0);
                count++;
            }
            else if (JackTokenizer::tokenType(token) == JackTokenizer::TokenType::SYMBOL && token == ".") {
                // '.'
                expectSymbol('.');

                name2 = expectIdentifier();

                if (subroutineScope.contains(name1)) {
                    writer.writePush(kindToSegment(subroutineScope.kindOf(name1)), subroutineScope.indexOf(name1));
                    name1 = subroutineScope.typeOf(name1);
                    count++;
                }
                else if (classScope.contains(name1)) {
                    writer.writePush(kindToSegment(classScope.kindOf(name1)), classScope.indexOf(name1));
                    name1 = classScope.typeOf(name1);
                    count++;
                }
            }

            // '('
            expectSymbol('(');

            // expressionList
            count += compileExpressionList();

            // ')'
            expectSymbol(')');

            writer.writeCall(name1 + "." + name2, count);
        }
        else {
            if (subroutineScope.contains(name1)) {
                writer.writePush(kindToSegment(subroutineScope.kindOf(name1)), subroutineScope.indexOf(name1));
            }
            else if (classScope.contains(name1)) {
                writer.writePush(kindToSegment(classScope.kindOf(name1)), classScope.indexOf(name1));
            }
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
        char op = expectSymbol(unaryOp);

        // term
        compileTerm();

        unaryOpType(op);
    }
    else {
        throw JackCompilationException({"integerConstant", "stringConstant", "keywordConstant", "varName", "varname '[' expression ']'", "'(' expression ')'", "unaryOp term", "subroutineCall"}, token, tokenizer.line(), tokenizer.col());
    }
}

int CompilationEngine::compileExpressionList()
{
    int expressions = 0;

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

    return expressions;
}

JackTokenizer::Keyword CompilationEngine::expectKeyword(JackTokenizer::Keyword expected)
{
    return expectKeyword({expected});
}

JackTokenizer::Keyword CompilationEngine::expectKeyword(std::initializer_list<JackTokenizer::Keyword> expected)
{
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::KEYWORD || std::find(expected.begin(), expected.end(), tokenizer.keyword()) == expected.end()) {
        std::vector<std::string> expectedStrs;
        for (JackTokenizer::Keyword keyword : expected) {
            expectedStrs.push_back(JackTokenizer::keywordToString(keyword));
        }

        throw JackCompilationException(expectedStrs, token, tokenizer.line(), tokenizer.col());
    }
    return tokenizer.keyword();
}

char CompilationEngine::expectSymbol(char expected)
{
    return expectSymbol({expected});
}

char CompilationEngine::expectSymbol(std::initializer_list<char> expected)
{
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::SYMBOL || std::find(expected.begin(), expected.end(), tokenizer.symbol()) == expected.end()) {
        std::vector<std::string> expectedStrs;
        for (char symbol : expected) {
            expectedStrs.push_back(std::string(1, symbol));
        }

        throw JackCompilationException(expectedStrs, token, tokenizer.line(), tokenizer.col());
    }
    return tokenizer.symbol();
}

const std::string& CompilationEngine::expectIdentifier()
{
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
        throw JackCompilationException("identifier", token, tokenizer.line(), tokenizer.col(), false);
    }
    return tokenizer.identifier();
}

int CompilationEngine::expectIntConst()
{
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::INT_CONST) {
        throw JackCompilationException("integerConstant", token, tokenizer.line(), tokenizer.col(), false);
    }
    return tokenizer.intVal();
}

std::string CompilationEngine::expectStringConst()
{
    if (std::string token = tokenizer.advance(); tokenizer.tokenType() != JackTokenizer::TokenType::STRING_CONST) {
        throw JackCompilationException("stringConstant", token, tokenizer.line(), tokenizer.col(), false);
    }
    return tokenizer.stringVal();
}

std::string CompilationEngine::expectTypeOrIdentifier()
{
    if (std::string token = tokenizer.advance(); JackTokenizer::tokenType(token) == JackTokenizer::TokenType::KEYWORD &&
        (JackTokenizer::keyword(token) == JackTokenizer::Keyword::INT ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::CHAR ||
        JackTokenizer::keyword(token) == JackTokenizer::Keyword::BOOLEAN)) {
            switch (tokenizer.keyword())
            {
                case JackTokenizer::Keyword::INT:
                    return "int";

                case JackTokenizer::Keyword::BOOLEAN:
                    return "boolean";

                case JackTokenizer::Keyword::CHAR:
                    return "char";

                default:
                    return "";
            }
    }
    else if (tokenizer.tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
        return tokenizer.identifier();
    }
    else {
        throw JackCompilationException({"int", "char", "boolean", "identifier"}, token, tokenizer.line(), tokenizer.col());
    }
    return "";
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

void CompilationEngine::defineVar(JackSymbolTable& scope, const std::string& name, const std::string& type, JackTokenizer::Keyword kind)
{
    switch (kind)
    {
        case JackTokenizer::Keyword::STATIC:
            return defineVar(scope, name, type, JackSymbolTable::Kind::STATIC);

        case JackTokenizer::Keyword::FIELD:
            return defineVar(scope, name, type, JackSymbolTable::Kind::FIELD);

        case JackTokenizer::Keyword::VAR:
            return defineVar(scope, name, type, JackSymbolTable::Kind::VAR);

        default:
            return;
    }
}

void CompilationEngine::defineVar(JackSymbolTable& scope, const std::string& name, const std::string& type, JackSymbolTable::Kind kind)
{
    scope.define(name, type, kind);
}

void CompilationEngine::opType(char op)
{
    switch (op)
    {
        case '+':
            writer.writeArithmetic(VMWriter::Op::ADD);
            break;

        case '-':
            writer.writeArithmetic(VMWriter::Op::SUB);
            break;

        case '*':
            writer.writeCall("Math.multiply", 2);
            break;

        case '/':
            writer.writeCall("Math.divide", 2);
            break;

        case '&':
            writer.writeArithmetic(VMWriter::Op::AND);
            break;

        case '|':
            writer.writeArithmetic(VMWriter::Op::OR);
            break;

        case '<':
            writer.writeArithmetic(VMWriter::Op::LT);
            break;

        case '>':
            writer.writeArithmetic(VMWriter::Op::GT);
            break;

        case '=':
            writer.writeArithmetic(VMWriter::Op::EQ);
            break;

        default:
            break;
    }
}

void CompilationEngine::unaryOpType(char unaryOp)
{
    switch (unaryOp)
    {
        case '-':
            writer.writeArithmetic(VMWriter::Op::NEG);
            break;

        case '~':
            writer.writeArithmetic(VMWriter::Op::NOT);
            break;

        default:
            break;
    }
}

VMWriter::Segment CompilationEngine::kindToSegment(JackSymbolTable::Kind kind)
{
    switch (kind)
    {
        case JackSymbolTable::Kind::STATIC:
            return VMWriter::Segment::STATIC;

        case JackSymbolTable::Kind::FIELD:
            return VMWriter::Segment::THIS;

        case JackSymbolTable::Kind::ARG:
            return VMWriter::Segment::ARGUMENT;

        case JackSymbolTable::Kind::VAR:
            return VMWriter::Segment::LOCAL;

        default:
            return VMWriter::Segment::NONE;
    }
}
