#include "parser.hpp"

#include <string>
#include <iostream>


bool Parser::isWhiteSpace() const
{
    return line.find_first_not_of(" \t\n\v\f\r") == line.length() - 1;
}

// NOTE: This assumes no end line comments
bool Parser::isComment() const
{
    return line.find("//") != std::string::npos;
}

Parser::Parser(std::filesystem::path file)
{
    ifs.open(file);
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
}

// NOTE: This assumes no leading whitespace
const Parser::InstructionType Parser::instructionType()
{
    switch (line[0])
    {
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

// NOTE: This assumes no leading whitespace
const std::string Parser::symbol() const
{
    if (type == InstructionType::A_INSTRUCTION) {
        return line.substr(1);
    }
    else {
        return line.substr(1, line.length() - 1);
    }
}

const std::string Parser::dest() const
{
    return line.substr(0, line.find("="));
}

const std::string Parser::comp() const
{
    return line.substr(line.find("=") + 1, line.find(";") - 1);
}

const std::string Parser::jump() const
{
    // We don't have a jump
    if (line.find(";") == std::string::npos) {
        return "";
    }
    else {
        return line.substr(line.find(";") + 1);
    }
}
