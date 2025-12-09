#include "jackCompiler.hpp"

#include <iostream>


JackCompiler::JackCompiler(const std::filesystem::path& inFile, const std::filesystem::path& outFile) : engine(inFile, outFile), inFile(inFile), outFile(outFile) {}

void JackCompiler::compile()
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
