#pragma once

#include <string>
#include <filesystem>
#include <fstream>

#include "compilationEngine.hpp"


class JackCompiler
{
    public:
        JackCompiler(const std::filesystem::path& inFile, const std::filesystem::path& outFile);

        void compile();

    private:
        CompilationEngine engine;

        std::filesystem::path inFile;
        std::filesystem::path outFile;
};
