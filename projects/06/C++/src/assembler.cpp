#include "assembler.hpp"

#include <bitset>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "code.hpp"


static constexpr int MAX_BITS = 15;

Assembler::Assembler(const std::filesystem::path inFile, const std::filesystem::path outFile) : parser(inFile)
{
    ofs.open(outFile);

    initializeSymbolTable();
}

bool Assembler::isNumber(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

void Assembler::assemble()
{
    firstPass();
    parser.reset();
    secondPass();
}

void Assembler::initializeSymbolTable()
{
    table.addEntry("R0", 0);
    table.addEntry("R1", 1);
    table.addEntry("R2", 2);
    table.addEntry("R3", 3);
    table.addEntry("R4", 4);
    table.addEntry("R5", 5);
    table.addEntry("R6", 6);
    table.addEntry("R7", 7);
    table.addEntry("R8", 8);
    table.addEntry("R9", 9);
    table.addEntry("R10", 10);
    table.addEntry("R11", 11);
    table.addEntry("R12", 12);
    table.addEntry("R13", 13);
    table.addEntry("R14", 14);
    table.addEntry("R15", 15);

    table.addEntry("SP", 0);
    table.addEntry("LCL", 1);
    table.addEntry("ARG", 2);
    table.addEntry("THIS", 3);
    table.addEntry("THAT", 4);
    
    table.addEntry("SCREEN", 16384);
    table.addEntry("KBD", 24576);
}

void Assembler::firstPass()
{
    int instructionNum = 0;
    std::vector<std::string> possibleVars;
    while (parser.hasMoreLines()) {
        parser.advance();
        switch (parser.instructionType())
        {
            case Parser::InstructionType::L_INSTRUCTION:
                table.addEntry(parser.symbol(), instructionNum);
                break;

            default:
                instructionNum++;
                break;
        }
    }
}

void Assembler::secondPass()
{
    int availAddr = 16;

    while (parser.hasMoreLines()) {
        parser.advance();
        switch (parser.instructionType())
        {
            int val;
            case Parser::InstructionType::A_INSTRUCTION:
                // Is the symbol a number?
                if (isNumber(parser.symbol())) {
                    val = std::stoi(parser.symbol());
                }
                // Label or symbolic var
                else {
                    if (table.contains(parser.symbol())) {
                        val = table.getAddress(parser.symbol());
                    }
                    else {
                        val = availAddr;
                        table.addEntry(parser.symbol(), availAddr);
                        availAddr++;
                    }
                }
                ofs << "0" << std::bitset<16>(val).to_string().substr(1) << std::endl;
                break;

            case Parser::InstructionType::C_INSTRUCTION:
                ofs << "111" << Code::comp(parser.comp()) << Code::dest(parser.dest()) << Code::jump(parser.jump()) << std::endl;
                break;

            default:
                break;
        }
    }
}
