#pragma once

#include <filesystem>
#include <fstream>
#include <string>


class VMWriter
{
    public:
        enum class Segment {
            NONE,
            CONSTANT,
            ARGUMENT,
            LOCAL,
            STATIC,
            THIS,
            THAT,
            POINTER,
            TEMP
        };

        enum class Op {
            NONE,
            ADD,
            SUB,
            NEG,
            EQ,
            GT,
            LT,
            AND,
            OR,
            NOT
        };

        VMWriter(const std::filesystem::path& outFile);

        void writePush(Segment segment, int index);
        void writePop(Segment segment, int index);
        void writeArithmetic(Op op);
        void writeLabel(const std::string& label);
        void writeGoto(const std::string& label);
        void writeIf(const std::string& label);
        void writeCall(const std::string& name, int nArgs);
        void writeFunction(const std::string& name, int nVars);
        void writeReturn();

    private:
        std::ofstream ofs;

        friend std::ostream& operator<<(std::ostream& os, const VMWriter::Segment segment);
        friend std::ostream& operator<<(std::ostream& os, const VMWriter::Op op);
};
