#pragma once

#include <string>
#include <filesystem>
#include <fstream>


class Parser {
    public:
        enum InstructionType {
            A_INSTRUCTION,
            C_INSTRUCTION,
            L_INSTRUCTION
        };

        Parser(std::filesystem::path file);

        bool hasMoreLines();
        void advance();
        InstructionType instructionType();
        std::string symbol() const;
        std::string dest() const;
        std::string comp() const;
        std::string jump() const;

    private:
        std::ifstream ifs;
        std::string line;
        InstructionType type;

        bool isWhiteSpace() const;
        bool isComment() const;
};
