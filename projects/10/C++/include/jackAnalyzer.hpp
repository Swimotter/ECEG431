#pragma once

#include <string>
#include <filesystem>
#include <fstream>

#include "compilationEngine.hpp"


class JackAnalyzer
{
    public:
        JackAnalyzer(const std::filesystem::path& inFile, const std::filesystem::path& outFile, const bool tokenize = false);

        void analyze();

    private:
        CompilationEngine engine;

        std::filesystem::path inFile;
        std::filesystem::path outFile;
};
