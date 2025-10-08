#include "codeWriter.hpp"

#include <unordered_map>


CodeWriter::CodeWriter(const std::filesystem::path file)
{
    ofs.open(file);
}

void CodeWriter::writeArithmetic(const std::string command)
{
    if (command == "add") {
        writeBinaryArithmetic();
        /**
         * M=M+D
         */
        ofs << "M=M+D" << std::endl;
    }
    else if (command == "sub") {
        writeBinaryArithmetic();
        /**
         * M=M+D
         */
        ofs << "M=M-D" << std::endl;
    }
    else if (command == "neg") {
        writeUnaryArithmetic();
        /**
         * M=-M
         */
        ofs << "M=-M" << std::endl;
    }
    else if (command == "eq") {
        writeBinaryArithmetic();
        writeCompare("JEQ");
    }
    else if (command == "gt") {
        writeBinaryArithmetic();
        writeCompare("JGT");
    }
    else if (command == "lt") {
        writeBinaryArithmetic();
        writeCompare("JLT");
    }
    else if (command == "and") {
        writeBinaryArithmetic();
        /**
         * M=D&M
         */
        ofs << "M=D&M" << std::endl;
    }
    else if (command == "or") {
        writeBinaryArithmetic();
        /**
         * M=D|M
         */
        ofs << "M=D|M" << std::endl;
    }
    else if (command == "not") {
        writeUnaryArithmetic();
        /**
         * M=!M
         */
        ofs << "M=!M" << std::endl;
    }
}

// Note: This expects a new value to be pushed to the stack after
void CodeWriter::writeUnaryArithmetic()
{
    /**
     * @SP
     * A=M-1
     */
    ofs << "@SP" << std::endl;
    ofs << "A=M-1" << std::endl;
}

void CodeWriter::writeCompare(std::string comparison)
{
    /**
     * I'm not even gunna bother writing this out, it's trash and doesn't
     * deserve getting a clean comment until it doesn't just keep adding
     * a psycopathic amount of jumps
     */
    ofs << "D=M-D" << std::endl;
    ofs << "@TRUE." << badCodeCounter << std::endl;
    ofs << "D;" << comparison << std::endl;
    ofs << "@FALSE." << badCodeCounter << std::endl;
    ofs << "D=0;JMP" << std::endl;
    ofs << "(TRUE." << badCodeCounter << ")" << std::endl;
    ofs << "D=-1" << std::endl;
    ofs << "(FALSE." << badCodeCounter << ")" << std::endl;
    ofs << "@SP" << std::endl;
    ofs << "A=M-1" << std::endl;
    ofs << "M=D" << std::endl;

    badCodeCounter++;
}

// Note: This expects a new value to be pushed to the stack after
void CodeWriter::writeBinaryArithmetic()
{
    writePopStack();
    /**
     * @SP
     * A=M-1
     */
    ofs << "@SP" << std::endl;
    ofs << "A=M-1" << std::endl;
}

void CodeWriter::writePushPop(const Parser::CommandType command, const std::string segment, const int index)
{
    switch (command)
    {
        case Parser::CommandType::C_PUSH:
            writePush(segment, index);
            break;
        
        case Parser::CommandType::C_POP:
            writePop(segment, index);
            break;
        
        default:
            break;
    }
}

void CodeWriter::close()
{
    ofs << "(END)" << std::endl;
    ofs << "@END" << std::endl;
    ofs << "0;JMP" << std::endl;
    ofs.close();
}

void CodeWriter::writePush(const std::string segment, const int index)
{
    if (segment == "local" ||
        segment == "argument" ||
        segment == "this" ||
        segment == "that"
    ) {
        static const std::unordered_map<std::string, std::string> segmentMap = {
            {"local", "LCL"},
            {"argument", "ARG"},
            {"this", "THIS"},
            {"that", "THAT"},
        };

        writePushDynamicSegment(segmentMap.at(segment), index);
    }
    else if (segment == "pointer" ||
        segment == "temp" ||
        segment == "static"
    ) {
        writePushFixedSegment(segment, index);
    }
    else if (segment == "constant") {
        writePushConstant(index);
    }
}

void CodeWriter::writePushDynamicSegment(const std::string segment, const int index)
{
    /**
     * @{segment}
     * D=M
     * writePushStack();
     * @{index}
     * A=D+A
     * D=M
     */
    ofs << "@" << segment << std::endl;
    ofs << "D=M" << std::endl;
    ofs << "@" << index << std::endl;
    ofs << "A=D+A" << std::endl;
    ofs << "D=M" << std::endl;
    writePushStack();
}

void CodeWriter::writePushFixedSegment(const std::string segment, const int index)
{
    static constexpr int TEMP_IDX = 5;
    static constexpr int STATIC_IDX = 16;

    int address;
    if (segment == "pointer") {
        switch (index) {
            case 0:
                // @THIS
                address = 3;
                break;

            case 1:
                // @THAT
                address = 4;
                break;

            default:
                return;
        }
    }
    else if (segment == "temp") {
        address = TEMP_IDX + index;
    }
    else if (segment == "static") {
        address = STATIC_IDX + index;
    }
    else {
        return;
    }

    /**
     * @{address}
     * D=M
     */
    ofs << "@" << address << std::endl;
    ofs << "D=M" << std::endl;
    writePushStack();
}

void CodeWriter::writePushConstant(const int index)
{
    /**
     * @{index}
     * D=A
     */
    ofs << "@" << index << std::endl;
    ofs << "D=A" << std::endl;
    writePushStack();
}

void CodeWriter::writePushStack()
{
    /**
     * @SP
     * M=M+1
     * A=M-1
     * M=D
     */
    ofs << "@SP" << std::endl;
    ofs << "M=M+1" << std::endl;
    ofs << "A=M-1" << std::endl;
    ofs << "M=D" << std::endl;
}

void CodeWriter::writePop(const std::string segment, const int index)
{
    if (segment == "local" ||
        segment == "argument" ||
        segment == "this" ||
        segment == "that"
    ) {
        static const std::unordered_map<std::string, std::string> segmentMap = {
            {"local", "LCL"},
            {"argument", "ARG"},
            {"this", "THIS"},
            {"that", "THAT"},
        };

        writePopDynamicSegment(segmentMap.at(segment), index);
    }
    else if (segment == "pointer" ||
        segment == "temp" ||
        segment == "static"
    ) {
        writePopFixedSegment(segment, index);
    }
}

void CodeWriter::writePopDynamicSegment(const std::string segment, const int index)
{
    /**
     * @{segment}
     * D=M
     * @{index}
     * D=D+A
     * @13
     * M=D
     * popStack()
     * @13
     * A=M
     * M=D
     */
    ofs << "@" << segment << std::endl;
    ofs << "D=M" << std::endl;
    ofs << "@" << index << std::endl;
    ofs << "D=D+A" << std::endl;
    ofs << "@13" << std::endl;
    ofs << "M=D" << std::endl;
    writePopStack();
    ofs << "@13" << std::endl;
    ofs << "A=M" << std::endl;
    ofs << "M=D" << std::endl;
}

void CodeWriter::writePopFixedSegment(const std::string segment, const int index)
{
    static constexpr int TEMP_IDX = 5;
    static constexpr int STATIC_IDX = 16;

    int address;
    if (segment == "pointer") {
        switch (index) {
            case 0:
                // @THIS
                address = 3;
                break;

            case 1:
                // @THAT
                address = 4;
                break;

            default:
                return;
        }
    }
    else if (segment == "temp") {
        address = TEMP_IDX + index;
    }
    else if (segment == "static") {
        address = STATIC_IDX + index;
    }
    else {
        return;
    }

    writePopStack();
    /**
     * @{address}
     * M=D
     */
    ofs << "@" << address << std::endl;
    ofs << "M=D" << std::endl;
}

void CodeWriter::writePopStack()
{
    /**
     * @SP
     * AM=M-1
     * D=M
     */
    ofs << "@SP" << std::endl;
    ofs << "AM=M-1" << std::endl;
    ofs << "D=M" << std::endl;
}
