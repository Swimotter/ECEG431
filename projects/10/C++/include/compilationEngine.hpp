#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include "jackTokenizer.hpp"


class CompilationEngine {
    public:
        CompilationEngine(const std::filesystem::path& outFile);

        void compileClass();
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

    private:
        std::ofstream ofs;

        unsigned short indentLevel = 0;

        void indent();
        void compileOpenTag(const std::string& tag);
        void compileCloseTag(const std::string& tag);
};
