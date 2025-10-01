#include "parser.hpp"

#include <string>


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
    bool isInstruction = false;
    while (!isInstruction && hasMoreLines()) {
        std::getline(ifs, line);

        // Strip end line comments
        size_t commentStart = line.find("//");
        if (commentStart != std::string::npos) {
            line = line.substr(0, commentStart);
        }
        
        // Strip whitespace on ends
        size_t firstNonWhitespace = line.find_first_not_of(" \t\n\v\f\r");
        if (firstNonWhitespace != std::string::npos) {
            size_t lastNonWhitespace = line.find_last_not_of(" \t\n\v\f\r");
            line = line.substr(firstNonWhitespace, lastNonWhitespace - firstNonWhitespace + 1);
        }
        else {
            line = "";
        }

        if (!line.empty()) {
            isInstruction = true;
        }
    }
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
            if (line.back() == ')') {
                type = InstructionType::L_INSTRUCTION;
            }
            else {
                type = InstructionType::C_INSTRUCTION;
            }
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
    std::string temp = line;

    //remove dest part if exists
    size_t equalPos = temp.find('=');
    if (equalPos != std::string::npos) {
        temp = temp.substr(equalPos + 1);
    }

    //remove jump part if exists
    size_t semicolonPos = temp.find(';');
    if (semicolonPos != std::string::npos) {
        temp = temp.substr(0, semicolonPos);
    }

    return temp;
}

const std::string Parser::jump() const
{
    // We don't have a jump
    size_t start = line.find(";");
    if (start == std::string::npos) {
        return "";
    }
    return line.substr(start + 1);
}
