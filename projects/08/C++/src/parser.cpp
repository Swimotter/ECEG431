#include "parser.hpp"


bool Parser::isArithmetic() const
{
    return (
        line.find("add") != std::string::npos ||
        line.find("sub") != std::string::npos ||
        line.find("neg") != std::string::npos ||
        line.find("eq") != std::string::npos ||
        line.find("gt") != std::string::npos ||
        line.find("lt") != std::string::npos ||
        line.find("and") != std::string::npos ||
        line.find("or") != std::string::npos ||
        line.find("not") != std::string::npos
    );
}

Parser::Parser(const std::filesystem::path file)
{
    ifs.open(file);
}

bool Parser::hasMoreLines()
{
    return ifs.peek() != EOF;
}

void Parser::advance()
{
    while (hasMoreLines()) {
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
            return;
        }
    }
}

const Parser::CommandType Parser::commandType()
{
    if (line.find("push") != std::string::npos) {
        type = CommandType::C_PUSH;
    }
    else if (line.find("pop") != std::string::npos) {
        type = CommandType::C_POP;
    }
    else if (line.find("label") != std::string::npos) {
        type = CommandType::C_LABEL;
    }
    else if (line.find("if-goto") != std::string::npos) {
        type = CommandType::C_IF;
    }
    else if (line.find("goto") != std::string::npos) {
        type = CommandType::C_GOTO;
    }
    else if (line.find("function") != std::string::npos) {
        type = CommandType::C_FUNCTION;
    }
    else if (line.find("return") != std::string::npos) {
        type = CommandType::C_RETURN;
    }
    else if (line.find("call") != std::string::npos) {
        type = CommandType::C_CALL;
    }
    else if (isArithmetic()) {
        type = CommandType::C_ARITHMETIC;
    }
    else {
        type = CommandType::NONE;
    }
    
    return type;
}

const std::string Parser::arg1() const
{
    size_t start = line.find(" ") + 1;
    size_t end = line.rfind(" ");
    return line.substr(start, end - start);
}

int Parser::arg2() const
{
    size_t start = line.find(" ", line.find(" ") + 1) + 1;
    return std::atoi(line.substr(start, line.length() - start).c_str());
}
