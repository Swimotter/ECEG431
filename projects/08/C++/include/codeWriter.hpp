#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include "parser.hpp"


class CodeWriter {
    public:
        CodeWriter(const std::filesystem::path file);

        void writeArithmetic(const std::string command);
        void writePushPop(const Parser::CommandType command, const std::string segment, const int index);
        void close();

    private:
        std::ofstream ofs;
        
        int badCodeCounter = 0;

        void writeBinaryArithmetic();
        void writeUnaryArithmetic();
        void writeCompare(std::string comparison);

        void writePush(const std::string segment, const int index);
        void writePushDynamicSegment(const std::string segment, const int index);
        void writePushFixedSegment(const std::string segment, const int index);
        void writePushConstant(const int index);
        void writePushStack();

        void writePop(const std::string segment, const int index);
        void writePopDynamicSegment(const std::string segment, const int index);
        void writePopFixedSegment(const std::string segment, const int index);
        void writePopStack();
};
