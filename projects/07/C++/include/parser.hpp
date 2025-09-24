#pragma once

#include <string>
#include <filesystem>
#include <fstream>


class Parser {
    public:
        enum CommandType {
            NONE,
            C_ARITHMETIC,
            C_PUSH,
            C_POP,
            C_LABEL,
            C_GOTO,
            C_RETURN,
            C_CALL
        };

        Parser(std::filesystem::path file);

        bool hasMoreLines();
        void advance();
        const CommandType commandType();
        const std::string arg1() const;
        int arg2() const;

    private:
        std::ifstream ifs;
        std::string line;
        CommandType type;

        bool isWhiteSpace() const;
        bool isComment() const;
        bool isArithmetic() const;
};
