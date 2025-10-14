#include "codeWriter.hpp"

#ifdef DEBUG
#include <cassert>
#endif
#include <unordered_map>


CodeWriter::CodeWriter(const std::filesystem::path file, const bool needsBootstrap)
{
    ofs.open(file);
    if (needsBootstrap) {
        writeBootstrap();
    }
}

void CodeWriter::writeBootstrap()
{
#ifdef DEBUG
    ofs << "// Start bootstrap" << std::endl;
#endif
    ofs << "@256" << std::endl;
    ofs << "D=A" << std::endl;
    ofs << "@SP" << std::endl;
    ofs << "M=D" << std::endl;
    writeCall("Sys.init");
#ifdef DEBUG
    ofs << "// End bootstrap" << std::endl << std::endl;
#endif
}

void CodeWriter::setFilename(const std::string& filename)
{
    parsingFile = filename;
}

void CodeWriter::writeArithmetic(const std::string command)
{
#ifdef DEBUG
    ofs << "// Start arithmetic" << std::endl;
#endif
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
#ifdef DEBUG
    ofs << "// End arithmetic" << std::endl << std::endl;
#endif
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

void CodeWriter::writeLabel(const std::string& label)
{
#ifdef DEBUG
    ofs << "// Start label" << std::endl;
#endif
    if (curFunc.empty()) {
        ofs << "(" << label << ")" << std::endl;
    }
    else {
        ofs << "(" << curFunc << "$" << label << ")" << std::endl;
    }
#ifdef DEBUG
    ofs << "// End label" << std::endl << std::endl;
#endif
}

void CodeWriter::writeGoto(const std::string& label)
{
#ifdef DEBUG
    ofs << "// Start goto" << std::endl;
#endif
    if (curFunc.empty()) {
        ofs << "@" << label << std::endl;
    }
    else {
        ofs << "@" << curFunc << "$" << label << std::endl;
    }
    ofs << "0;JMP" << std::endl;
#ifdef DEBUG
    ofs << "// End goto" << std::endl << std::endl;
#endif
}

void CodeWriter::writeIf(const std::string& label)
{
#ifdef DEBUG
    ofs << "// Start if-goto" << std::endl;
#endif
    writePopStack();
    if (curFunc.empty()) {
        ofs << "@" << label << std::endl;
    }
    else {
        ofs << "@" << curFunc << "$" << label << std::endl;
    }
    ofs << "D;JNE" << std::endl;
#ifdef DEBUG
    ofs << "// End if-goto" << std::endl << std::endl;
#endif
}

void CodeWriter::writeFunction(const std::string& functionName, const int nVars)
{
    curFunc = functionName;

#ifdef DEBUG
    ofs << "// Start function" << std::endl;
#endif
    ofs << "(" << functionName << ")" << std::endl;
    for (int i = 0; i < nVars; i++) {
        writePushStack(0);
    }
#ifdef DEBUG
    ofs << "// End function" << std::endl << std::endl;
#endif
}

void CodeWriter::writeCall(const std::string& functionName, const int nVars)
{
#ifdef DEBUG
    ofs << "// Start call" << std::endl;
#endif
    // RA
    ofs << "@" << "ret" << returnCount << std::endl;
    ofs << "D=A" << std::endl;
    writePushStack();
    writeCallHelper("LCL");
    writeCallHelper("ARG");
    writeCallHelper("THIS");
    writeCallHelper("THAT");
    // Set ARG and LCL
    ofs << "@SP" << std::endl;
    ofs << "D=M" << std::endl;
    ofs << "@LCL" << std::endl;
    ofs << "M=D" << std::endl;
    ofs << "@" << nVars + 5 << std::endl;
    ofs << "D=D-A" << std::endl;
    ofs << "@ARG" << std::endl;
    ofs << "M=D" << std::endl;
    // GOTO
    ofs << "@" << functionName << std::endl;
    ofs << "0;JMP" << std::endl;
    ofs << "(" << "ret" << returnCount << ")" << std::endl;
#ifdef DEBUG
    ofs << "// End call" << std::endl << std::endl;
#endif

    returnCount++;
}

void CodeWriter::writeCallHelper(const std::string& segment)
{
#ifdef DEBUG
    ofs << "// Start call helper" << std::endl;
#endif
    ofs << "@" << segment << std::endl;
    ofs << "D=M" << std::endl;
    writePushStack();
#ifdef DEBUG
    ofs << "// End call helper" << std::endl;
#endif
}

void CodeWriter::writeReturn()
{
#ifdef DEBUG
    ofs << "// Start return" << std::endl;
#endif
    ofs << "@5" << std::endl;
    ofs << "D=A" << std::endl;
    ofs << "@LCL" << std::endl;
    ofs << "A=M-D" << std::endl;
    ofs << "D=M" << std::endl;
    ofs << "@R13" << std::endl;
    ofs << "M=D" << std::endl;

    writePopStack();

    ofs << "@ARG" << std::endl;
    ofs << "A=M" << std::endl;
    ofs << "M=D" << std::endl;
    ofs << "D=A+1" << std::endl;
    ofs << "@SP" << std::endl;
    ofs << "M=D" << std::endl;
    
    writeReturnHelper("THAT");
    writeReturnHelper("THIS");
    writeReturnHelper("ARG");
    writeReturnHelper("LCL");

    ofs << "@R13" << std::endl;
    ofs << "A=M" << std::endl;
    ofs << "0;JMP" << std::endl;
#ifdef DEBUG
    ofs << "// End return" << std::endl << std::endl;
#endif
}

void CodeWriter::writeReturnHelper(const std::string& segment)
{
#ifdef DEBUG
    ofs << "// Start return helper" << std::endl;
#endif
    ofs << "@LCL" << std::endl;
    ofs << "AM=M-1" << std::endl;
    ofs << "D=M" << std::endl;
    ofs << "@" << segment << std::endl;
    ofs << "M=D" << std::endl;
#ifdef DEBUG
    ofs << "// End return helper" << std::endl;
#endif
}

void CodeWriter::close()
{
#ifdef DEBUG
    ofs << "// Start close" << std::endl;
#endif
    ofs << "(END)" << std::endl;
    ofs << "@END" << std::endl;
    ofs << "0;JMP" << std::endl;
#ifdef DEBUG
    ofs << "// End close" << std::endl;
#endif
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
#ifdef DEBUG
    ofs << "// Start push dynamic segment" << std::endl;
#endif
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
#ifdef DEBUG
    ofs << "// End push dynamic segment" << std::endl << std::endl;
#endif
}

void CodeWriter::writePushFixedSegment(const std::string segment, const int index)
{
    static constexpr int TEMP_IDX = 5;

    std::string address;
    if (segment == "pointer") {
        switch (index) {
            case 0:
                address = "THIS";
                break;

            case 1:
                address = "THAT";
                break;

            default:
                return;
        }
    }
    else if (segment == "temp") {
        address = std::to_string(TEMP_IDX + index);
    }
    else if (segment == "static") {
        address = parsingFile + "." + std::to_string(index);
    }
    else {
        return;
    }

#ifdef DEBUG
    ofs << "// Start push fixed segment" << std::endl;
#endif
    /**
     * @{address}
     * D=M
     */
    ofs << "@" << address << std::endl;
    ofs << "D=M" << std::endl;
    writePushStack();
#ifdef DEBUG
    ofs << "// End push fixed segment" << std::endl << std::endl;
#endif
}

void CodeWriter::writePushConstant(const int index)
{
#ifdef DEBUG
    ofs << "// Start push constant" << std::endl;
#endif
    // 0, 1, -1 all can be used directly
    if (index == 0 || index == 1 || index == -1) {
        writePushStack(index);
    }
    else {
        /**
         * @{index}
         * D=A
         */
        ofs << "@" << index << std::endl;
        ofs << "D=A" << std::endl;
        writePushStack();
    }
#ifdef DEBUG
    ofs << "// End push constant" << std::endl << std::endl;
#endif
}

void CodeWriter::writePushStack()
{
#ifdef DEBUG
    ofs << "// Start push stack" << std::endl;
#endif
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
#ifdef DEBUG
    ofs << "// End push stack" << std::endl;
#endif
}

// NOTE: this should only be called if val is 0, 1, or -1
void CodeWriter::writePushStack(const int val)
{
#ifdef DEBUG
    assert(val == 0 || val == 1 || val == -1);
    ofs << "// Start push stack" << std::endl;
#endif
    /**
     * @SP
     * M=M+1
     * A=M-1
     * M=D
     */
    ofs << "@SP" << std::endl;
    ofs << "M=M+1" << std::endl;
    ofs << "A=M-1" << std::endl;
    ofs << "M=" << val << std::endl;
#ifdef DEBUG
    ofs << "// End push stack" << std::endl;
#endif
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
#ifdef DEBUG
    ofs << "// Start pop dynamic segment" << std::endl;
#endif
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
#ifdef DEBUG
    ofs << "// End pop dynamic segment" << std::endl << std::endl;
#endif
}

void CodeWriter::writePopFixedSegment(const std::string segment, const int index)
{
    static constexpr int TEMP_IDX = 5;

    std::string address;
    if (segment == "pointer") {
        switch (index) {
            case 0:
                address = "THIS";
                break;

            case 1:
                address = "THAT";
                break;

            default:
                return;
        }
    }
    else if (segment == "temp") {
        address = std::to_string(TEMP_IDX + index);
    }
    else if (segment == "static") {
        address = parsingFile + "." + std::to_string(index);;
    }
    else {
        return;
    }

#ifdef DEBUG
    ofs << "// Start pop fixed segment" << std::endl;
#endif
    writePopStack();
    /**
     * @{address}
     * M=D
     */
    ofs << "@" << address << std::endl;
    ofs << "M=D" << std::endl;
#ifdef DEBUG
    ofs << "// End pop fixed segment" << std::endl << std::endl;
#endif
}

void CodeWriter::writePopStack()
{
#ifdef DEBUG
    ofs << "// Start pop stack" << std::endl;
#endif
    /**
     * @SP
     * AM=M-1
     * D=M
     */
    ofs << "@SP" << std::endl;
    ofs << "AM=M-1" << std::endl;
    ofs << "D=M" << std::endl;
#ifdef DEBUG
    ofs << "// End pop stack" << std::endl;
#endif
}

// Note: This expects a new value to be pushed to the stack after
void CodeWriter::writeUnaryArithmetic()
{
#ifdef DEBUG
    ofs << "// Start unary arithmetic" << std::endl;
#endif
    /**
     * @SP
     * A=M-1
     */
    ofs << "@SP" << std::endl;
    ofs << "A=M-1" << std::endl;
#ifdef DEBUG
    ofs << "// End unary arithmetic" << std::endl;
#endif
}

void CodeWriter::writeCompare(std::string comparison)
{
#ifdef DEBUG
    ofs << "// Start compare" << std::endl;
#endif
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
#ifdef DEBUG
    ofs << "// End compare" << std::endl;
#endif

    badCodeCounter++;
}

// Note: This expects a new value to be pushed to the stack after
void CodeWriter::writeBinaryArithmetic()
{
#ifdef DEBUG
    ofs << "// Start binary arithmetic" << std::endl;
#endif
    writePopStack();
    /**
     * @SP
     * A=M-1
     */
    ofs << "@SP" << std::endl;
    ofs << "A=M-1" << std::endl;
#ifdef DEBUG
    ofs << "// End binary arithmetic" << std::endl;
#endif
}
