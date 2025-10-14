#pragma once

#include <string>
#include <filesystem>
#include <fstream>

#include "codeWriter.hpp"
#include "parser.hpp"


class VMTranslator {
    public:
        VMTranslator(const std::filesystem::path outFile, const bool needsBootstrap = true);
        ~VMTranslator();

        void translate(const std::filesystem::path& inFile);

    private:
        Parser parser;
        CodeWriter writer;
};
