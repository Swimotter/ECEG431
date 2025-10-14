#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include "parser.hpp"


class CodeWriter {
    public:
        CodeWriter(const std::filesystem::path file, const bool needsBootstrap = true);

        void writeBootstrap();

        void setFilename(const std::string& filename);
        void writeArithmetic(const std::string command);
        void writePushPop(const Parser::CommandType command, const std::string segment, const int index);
        void writeLabel(const std::string& label);
        void writeGoto(const std::string& label);
        void writeIf(const std::string& label);
        void writeFunction(const std::string& functionName, const int nVars = 0);
        void writeCall(const std::string& functionName, const int nVars = 0);
        void writeReturn();
        void close();

    private:
        std::ofstream ofs;

        std::string parsingFile;
        std::string curFunc;

        int returnCount = 0;
        int badCodeCounter = 0;

        void writeBinaryArithmetic();
        void writeUnaryArithmetic();
        void writeCompare(std::string comparison);

        void writePush(const std::string segment, const int index);
        void writePushDynamicSegment(const std::string segment, const int index);
        void writePushFixedSegment(const std::string segment, const int index);
        void writePushConstant(const int index);
        void writePushStack();
        void writePushStack(const int val);

        void writePop(const std::string segment, const int index);
        void writePopDynamicSegment(const std::string segment, const int index);
        void writePopFixedSegment(const std::string segment, const int index);
        void writePopStack();

        void writeCallHelper(const std::string& segment);
        void writeReturnHelper(const std::string& segment);
};
