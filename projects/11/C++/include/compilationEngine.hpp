#pragma once

#include <exception>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

#include "jackTokenizer.hpp"
#include "jackSymbolTable.hpp"
#include "VMWriter.hpp"


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
        CompilationEngine(const std::filesystem::path& inFile, const std::filesystem::path& outFile);

        void compileClass();

    private:
        JackTokenizer tokenizer;
        VMWriter writer;
        JackSymbolTable classScope;
        JackSymbolTable subroutineScope;

        std::string className;

        int numIf;
        int numWhile;

        JackTokenizer::Keyword expectKeyword(JackTokenizer::Keyword expected);
        JackTokenizer::Keyword expectKeyword(std::initializer_list<JackTokenizer::Keyword> expected);
        char expectSymbol(char expected);
        char expectSymbol(std::initializer_list<char> expected);
        const std::string& expectIdentifier();
        int expectIntConst();
        std::string expectStringConst();
        std::string expectTypeOrIdentifier();

        bool peekKeyword(JackTokenizer::Keyword expected, std::string* const tokenOut = nullptr);
        bool peekKeyword(std::initializer_list<JackTokenizer::Keyword> expected, std::string* const tokenOut = nullptr);
        bool peekSymbol(char expected, std::string* const tokenOut = nullptr);
        bool peekSymbol(std::initializer_list<char> expected, std::string* const tokenOut = nullptr);

        void compileClassVarDec();
        void compileSubroutine();
        int compileParameterList(bool method);
        void compileSubroutineBody(JackTokenizer::Keyword type, const std::string& name);
        int compileVarDec();
        void compileStatements();
        void compileLet();
        void compileIf();
        void compileWhile();
        void compileDo();
        void compileReturn();
        void compileExpression();
        void compileTerm();
        int compileExpressionList();

        void defineVar(JackSymbolTable& scope, const std::string& name, const std::string& type, JackTokenizer::Keyword kind);
        void defineVar(JackSymbolTable& scope, const std::string& name, const std::string& type, JackSymbolTable::Kind kind);
        void opType(char op);
        void unaryOpType(char unaryOp);
        VMWriter::Segment kindToSegment(JackSymbolTable::Kind kind);
};
