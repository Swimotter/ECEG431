#include <filesystem>
#include <iostream>
#include <vector>

#include "jackAnalyzer.hpp"


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
            if (inFile.extension() == ".jack")
            {
                files.push_back(inFile);
            }
        }
        
        for (const auto& inFile : files) {
            std::filesystem::path outFile = inFile.parent_path() / (inFile.stem().string() + "-C++.xml");
            JackAnalyzer analyzer(inFile, outFile);
            analyzer.analyze();
        }
    }
    else
    {
        std::filesystem::path inFile(std::filesystem::absolute(path));

        if (!std::filesystem::exists(inFile)) {
            std::cerr << "File: '" << inFile << "' does not exist";
            return 1;
        }
        
        if (inFile.extension() != ".jack") {
            std::cerr << "File: '" << inFile << "' is not a Jack file";
            return 1;
        }

        std::filesystem::path outFile = inFile.parent_path() / inFile.stem() / "-C++.xml";
        JackAnalyzer analyzer(inFile, outFile);
        analyzer.analyze();
    }
}
