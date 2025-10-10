#include <filesystem>
#include <iostream>

#include "vmTranslator.hpp"


int main(int argc, char *argv[]) {
    // if (argc != 2) {
    //     std::cerr << "Usage: " << argv[0] << " {filename}.vm";
    //     return 1;
    // }

    std::filesystem::path path(std::filesystem::absolute(argv[1]));
    if (std::filesystem::is_directory(path))
    {
        for (const auto& p : std::filesystem::directory_iterator(path))
        {
            std::filesystem::path inFile(std::filesystem::absolute(p.path()));
            if (inFile.extension() == ".vm")
            {
                std::filesystem::path outFile(inFile);
                outFile.replace_extension("asm");

                VMTranslator translator(inFile, outFile);
                translator.translate();
            }
        }
    }
    else
    {
        std::filesystem::path inFile(std::filesystem::absolute(path));
        if (!inFile.has_extension()) {
            inFile.replace_extension("vm");
        }

        if (!std::filesystem::exists(inFile)) {
            std::cerr << "File: '" << inFile << "' does not exist";
            return 1;
        }

        std::filesystem::path outFile(inFile);
        outFile.replace_extension("asm");

        VMTranslator translator(inFile, outFile);
        translator.translate();
    }
}
