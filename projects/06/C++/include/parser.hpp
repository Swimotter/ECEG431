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
        const InstructionType instructionType();
        const std::string symbol() const;
        const std::string dest() const;
        const std::string comp() const;
        const std::string jump() const;

    private:
        std::ifstream ifs;
        std::string line;
        InstructionType type;

        bool isWhiteSpace() const;
        bool isComment() const;
};
