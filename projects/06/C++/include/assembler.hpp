#pragma once

#include <string>
#include <filesystem>
#include <fstream>

#include "symbolTable.hpp"
#include "parser.hpp"


class Assembler {
    public:
        Assembler(const std::filesystem::path inFile, const std::filesystem::path outFile);

        void assemble();

    private:
        SymbolTable table;
        Parser parser;
        std::ofstream ofs;

        void initializeSymbolTable();
        void firstPass();
        void secondPass();
};
