#include "vmTranslator.hpp"


VMTranslator::VMTranslator(const std::filesystem::path inFile, const std::filesystem::path outFile) :
    parser(inFile), writer(outFile) {}

VMTranslator::~VMTranslator()
{
    writer.close();
}

void VMTranslator::translate()
{
    while (parser.hasMoreLines()) {
        parser.advance();
        switch (parser.commandType())
        {
            case Parser::CommandType::C_ARITHMETIC:
                writer.writeArithmetic(parser.arg1());
                break;
                
            case Parser::CommandType::C_PUSH:
            case Parser::CommandType::C_POP:
                writer.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
                break;
                
            case Parser::CommandType::C_LABEL:
                break;
                
            case Parser::CommandType::C_GOTO:
                break;
                
            case Parser::CommandType::C_RETURN:
                break;
                
            case Parser::CommandType::C_CALL:
                break;

            default:
                break;
        }
    }
}
