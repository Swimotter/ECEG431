#include <filesystem>
#include <iostream>
#include <vector>

#include "vmTranslator.hpp"


int main(int argc, char *argv[]) {
    // if (argc != 2) {
    //     std::cerr << "Usage: " << argv[0] << " {filename}.vm";
    //     return 1;
    // }

    std::filesystem::path path(std::filesystem::absolute(argv[1]));
    if (std::filesystem::is_directory(path))
    {   
        std::vector<std::filesystem::path> files;
        for (const auto& p : std::filesystem::directory_iterator(path))
        {
            std::filesystem::path inFile(std::filesystem::absolute(p.path()));
            if (inFile.extension() == ".vm")
            {
                files.push_back(inFile);
            }
        }

        std::filesystem::path outFile(path);
        outFile /= path.filename();
        outFile.replace_extension("asm");
        VMTranslator translator(outFile, files.size() > 1);

        for (const auto& file : files) {
            translator.translate(file);
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

        VMTranslator translator(outFile);
        translator.translate(inFile);
    }
}
