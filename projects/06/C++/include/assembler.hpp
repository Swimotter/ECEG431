#pragma once

#include <string>
#include <filesystem>
#include <fstream>

#include "symbolTable.hpp"
#include "parser.hpp"


class Assembler {
    public:
        Assembler(std::filesystem::path file);

        void assemble();

    private:
        SymbolTable table;
        Parser parser;
        std::ofstream ofs;

        void initializeSymbolTable();
};
