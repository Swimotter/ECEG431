#pragma once

#include <string>
#include <filesystem>
#include <fstream>

#include "compilationEngine.hpp"
#include "jackTokenizer.hpp"


class JackAnalyzer {
    public:
        JackAnalyzer(const std::filesystem::path& inFile, const std::filesystem::path& outFile);

        void analyze();

    private:
        JackTokenizer tokenizer;
        CompilationEngine engine;

        void handleToken();
        void handleKeyword();
};
