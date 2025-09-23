#include "parser.hpp"

#include <string>


bool Parser::isWhiteSpace() const
{
    return line.find_first_not_of(" \t\n\v\f\r") == std::string::npos;
}

// NOTE: This only returns true if the entire line is a comment
bool Parser::isComment() const
{
    return line.length() >= 2 && line.substr(line.find_first_not_of(" \t\n\v\f\r"), 2) == "//";
}

Parser::Parser(std::filesystem::path file)
{
    ifs.open(file);
}

void Parser::reset()
{
    ifs.seekg(0);
    line = "";
    type = InstructionType::NONE;
}

bool Parser::hasMoreLines()
{
    return ifs.peek() != EOF;
}

void Parser::advance()
{
    std::getline(ifs, line);
    
    bool isInstruction = !isWhiteSpace() && !isComment();

    while (!isInstruction && hasMoreLines()) {
        std::getline(ifs, line);
        isInstruction = !isWhiteSpace() && !isComment();
    }


    // Strip end line comments
    int commentStart = line.find("//");
    line = line.substr(0, commentStart);
    
    // Strip whitespace on ends
    int firstNonWhitespace = line.find_first_not_of(" \t\n\v\f\r");
    int lastNonWhitespace = line.find_last_not_of(" \t\n\v\f\r");
    line = line.substr(firstNonWhitespace, lastNonWhitespace - firstNonWhitespace + 1);
}

const Parser::InstructionType Parser::instructionType()
{
    switch (line[0])
    {
        case '\0':
            type = InstructionType::NONE;
            break;

        // A-Instruction
        case '@':
            type = InstructionType::A_INSTRUCTION;
            break;
        
        // L-Instruction
        case '(':
            type = InstructionType::L_INSTRUCTION;
            break;
        
        // C-Instruction
        default:
            type = InstructionType::C_INSTRUCTION;
            break;
    }
    
    return type;
}

const std::string Parser::symbol() const
{
    if (type == InstructionType::A_INSTRUCTION) {
        return line.substr(1);
    }
    else if (type == InstructionType::L_INSTRUCTION) {
        return line.substr(1, line.length() - 2);
    }
    else {
        return "";
    }
}

const std::string Parser::dest() const
{
    size_t end = line.find("=");
    if (end == std::string::npos) {
        return "";
    }
    return line.substr(0, end);
}

const std::string Parser::comp() const
{
    size_t start = line.find("=");
    if (start == std::string::npos) {
        start = 0;
    }
    else {
        start = start + 1;
    }

    size_t end = line.find(";");
    if (end == std::string::npos) {
        end = line.length();
    }
    else {
        end = start + 1;
    }

    return line.substr(start, end - start);
}

const std::string Parser::jump() const
{
    // We don't have a jump
    size_t start = line.find(";");
    if (start == std::string::npos) {
        return "";
    }
    else {
        start = start + 1;
    }

    return line.substr(start);
}
