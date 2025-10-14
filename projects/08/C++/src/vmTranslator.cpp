#include "vmTranslator.hpp"


VMTranslator::VMTranslator(const std::filesystem::path outFile, const bool needsBootstrap) : parser(""), writer(outFile, needsBootstrap) {}

VMTranslator::~VMTranslator()
{
    writer.close();
}

void VMTranslator::translate(const std::filesystem::path& inFile)
{
    parser = Parser(inFile);
    writer.setFilename(inFile.stem().string());

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
                writer.writeLabel(parser.arg1());
                break;
                
            case Parser::CommandType::C_GOTO:
                writer.writeGoto(parser.arg1());
                break;
            
            case Parser::CommandType::C_IF:
                writer.writeIf(parser.arg1());
                break;

            case Parser::CommandType::C_FUNCTION:
                writer.writeFunction(parser.arg1(), parser.arg2());
                break;

            case Parser::CommandType::C_RETURN:
                writer.writeReturn();
                break;
                
            case Parser::CommandType::C_CALL:
                writer.writeCall(parser.arg1(), parser.arg2());
                break;

            default:
                break;
        }
    }
}
