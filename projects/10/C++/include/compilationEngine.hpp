#pragma once

#include <exception>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

#include "jackTokenizer.hpp"


class JackCompilationException : public std::exception
{
    public:
        JackCompilationException(const std::string& expected, const std::string& actual, int lineNum, int colNum, bool literal = true) : expected({expected}), actual(actual), lineNum(lineNum), colNum(colNum), literal(literal) {}
        JackCompilationException(const std::vector<std::string>& expected, const std::string& actual, int lineNum, int colNum, bool literal = true) : expected(expected), actual(actual), lineNum(lineNum), colNum(colNum), literal(literal) {}

        const char* what() const noexcept override {
            std::string out = "";
            if (expected.size() > 1) {
                out += "Expected token to be one of ";
                for (std::vector<std::basic_string<char>>::size_type i = 0; i < expected.size(); i++) {
                    if (literal) {
                        out += "\"";
                    }
                    out += expected.at(i);
                    if (literal) {
                        out += "\"";
                    }

                    if (i == expected.size() - 1) {
                        continue;
                    }
                    else if (i == expected.size() - 2) {
                        out += ", or ";
                    }
                    else {
                        out += ", ";
                    }
                }
                out += " but found \"" + actual + "\" instead";
            }
            else {
                out += "Expected token ";
                if (literal) {
                    out += "\"";
                }
                out += expected.at(0);
                if (literal) {
                    out += "\"";
                }
                out += " but found \"" + actual + "\" instead";
            }
            message = out;
            return message.c_str();
        }

        int line() const {
            return lineNum;
        }

        int col() const {
            return colNum;
        }
    
    private:
        std::vector<std::string> expected;
        std::string actual;
        int lineNum;
        int colNum;

        bool literal;
        mutable std::string message;
};

class CompilationEngine
{
    public:
        CompilationEngine(const std::filesystem::path& inFile, const std::filesystem::path& outFile, const bool tokenize = false);

        void compileClass();

    private:
        JackTokenizer tokenizer;

        std::ofstream ofs;

        unsigned short indentLevel = 0;

        bool tokenize;

        void expectKeyword(JackTokenizer::Keyword expected);
        void expectKeyword(std::initializer_list<JackTokenizer::Keyword> expected);
        void expectSymbol(char expected);
        void expectSymbol(std::initializer_list<char> expected);
        void expectIdentifier();
        void expectIntConst();
        void expectStringConst();
        void expectTypeOrIdentifier();

        bool peekKeyword(JackTokenizer::Keyword expected, std::string* const tokenOut = nullptr);
        bool peekKeyword(std::initializer_list<JackTokenizer::Keyword> expected, std::string* const tokenOut = nullptr);
        bool peekSymbol(char expected, std::string* const tokenOut = nullptr);
        bool peekSymbol(std::initializer_list<char> expected, std::string* const tokenOut = nullptr);

        void writeIndent();
        
        void compileClassVarDec();
        void compileSubroutine();
        void compileParameterList();
        void compileSubroutineBody();
        void compileVarDec();
        void compileStatements();
        void compileLet();
        void compileIf();
        void compileWhile();
        void compileDo();
        void compileReturn();
        void compileExpression();
        void compileTerm();
        int compileExpressionList();
        void compileOpenTag(const std::string& tag, bool trailingNewLine = false);
        void compileCloseTag(const std::string& tag, bool indent = true);
        template <typename T>
        void compileInlineTag(const std::string &tag, const T &token);
        void compileKeyword();
        void compileSymbol();
        void compileIdentifier();
        void compileIntVal();
        void compileStringVal();
};
