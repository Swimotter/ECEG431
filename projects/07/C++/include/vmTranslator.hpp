#pragma once

#include <string>
#include <filesystem>
#include <fstream>

#include "codeWriter.hpp"
#include "parser.hpp"


class VMTranslator {
    public:
        VMTranslator(const std::filesystem::path inFile, const std::filesystem::path outFile);
        ~VMTranslator();

        void translate();

    private:
        Parser parser;
        CodeWriter writer;
};
