#include "jackAnalyzer.hpp"

#include <iostream>


JackAnalyzer::JackAnalyzer(const std::filesystem::path& inFile, const std::filesystem::path& outFile, const bool tokenize) : engine(inFile, outFile, tokenize), inFile(inFile), outFile(outFile) {}

void JackAnalyzer::analyze()
{
    try {
        engine.compileClass();
    }
    catch (const JackCompilationException& e) {
        // We need to manually call the destructor so we can delete the file
        engine.~CompilationEngine();
        std::cerr << "Error in " << inFile.string() << ":" << e.line() << ":" << e.col() << " - " << e.what() << std::endl;
        std::filesystem::remove(outFile);
    }
}
