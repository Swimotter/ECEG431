#include <filesystem>
#include <fstream>
#include <string>

#include "VMWriter.hpp"


VMWriter::VMWriter(const std::filesystem::path& outFile)
{
    ofs.open(outFile);
}

void VMWriter::writePush(VMWriter::Segment segment, int index)
{
    ofs << "push " << segment << " " << index << std::endl;
}

void VMWriter::writePop(VMWriter::Segment segment, int index)
{
    if (segment == VMWriter::Segment::CONSTANT) {
        exit(1);
    }

    ofs << "pop " << segment << " " << index << std::endl;
}

void VMWriter::writeArithmetic(VMWriter::Op op)
{
    ofs << op << std::endl;
}

void VMWriter::writeLabel(const std::string& label)
{
    ofs << "label " << label << std::endl;
}

void VMWriter::writeGoto(const std::string& label)
{
    ofs << "goto " << label << std::endl;
}

void VMWriter::writeIf(const std::string& label)
{
    ofs << "if-goto " << label << std::endl;
}

void VMWriter::writeCall(const std::string& name, int nArgs)
{
    ofs << "call " << name << " " << nArgs << std::endl;
}

void VMWriter::writeFunction(const std::string& name, int nVars)
{
    ofs << "function " << name << " " << nVars << std::endl;
}

void VMWriter::writeReturn()
{
    ofs << "return" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const VMWriter::Segment segment)
{
    switch (segment) {
        case VMWriter::Segment::CONSTANT:
            os << "constant";
            break;
        case VMWriter::Segment::ARGUMENT:
            os << "argument";
            break;
        case VMWriter::Segment::LOCAL:
            os << "local";
            break;
        case VMWriter::Segment::STATIC:
            os << "static";
            break;
        case VMWriter::Segment::THIS:
            os << "this";
            break;
        case VMWriter::Segment::THAT:
            os << "that";
            break;
        case VMWriter::Segment::POINTER:
            os << "pointer";
            break;
        case VMWriter::Segment::TEMP:
            os << "temp";
            break;
        default:
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const VMWriter::Op op)
{
    switch (op) {
        case VMWriter::Op::ADD:
            os << "add";
            break;
        case VMWriter::Op::SUB:
            os << "sub";
            break;
        case VMWriter::Op::NEG:
            os << "neg";
            break;
        case VMWriter::Op::EQ:
            os << "eq";
            break;
        case VMWriter::Op::GT:
            os << "gt";
            break;
        case VMWriter::Op::LT:
            os << "lt";
            break;
        case VMWriter::Op::AND:
            os << "and";
            break;
        case VMWriter::Op::OR:
            os << "or";
            break;
        case VMWriter::Op::NOT:
            os << "not";
            break;
        default:
            break;
    }
    return os;
}
