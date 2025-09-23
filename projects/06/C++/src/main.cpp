#include <filesystem>
#include <iostream>

#include "assembler.hpp"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " {filename}.asm";
        return 1;
    }

    std::filesystem::path inFile(std::filesystem::absolute(argv[1]));
    if (!inFile.has_extension()) {
        inFile.replace_extension("asm");
    }

    if (!std::filesystem::exists(inFile)) {
        std::cerr << "File: '" << inFile << "' does not exist";
        return 1;
    }

    std::filesystem::path outFile(inFile);
    outFile.replace_extension("hack");

    Assembler assembler(inFile, outFile);
    assembler.assemble();
}
