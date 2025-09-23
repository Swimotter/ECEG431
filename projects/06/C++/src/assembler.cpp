#include "assembler.hpp"

#include <bitset>
#include <filesystem>
#include <fstream>
#include <string>

#include "code.hpp"


static constexpr int MAX_BITS = 15;

Assembler::Assembler(std::filesystem::path file) : parser(file)
{
    std::string outFile = file.stem().string() + ".hack";
    ofs.open(outFile);

    initializeSymbolTable();
}

void Assembler::assemble()
{
    int instructionNum = 0;
    int firstOpenAddress = 16;
    
    while (parser.hasMoreLines()) {
        parser.advance();
        switch (parser.instructionType())
        {
            case Parser::A_INSTRUCTION:
                if (table.contains(parser.symbol())) {
                    ofs << "0" << std::bitset<MAX_BITS>(table.getAddress(parser.symbol())).to_string() << std::endl;
                }
                else {
                    // Is the symbol a number?
                    try {
                        int address = std::stoi(parser.symbol());
                        ofs << "0" << std::bitset<MAX_BITS>(address).to_string() << std::endl;
                    }
                    // Not a number
                    catch (const std::invalid_argument& e) {
                        table.addEntry(parser.symbol(), firstOpenAddress);
                        ofs << "0" << std::bitset<MAX_BITS>(firstOpenAddress).to_string() << std::endl;
                        firstOpenAddress++;
                    }
                }

                instructionNum++;
                break;

            case Parser::C_INSTRUCTION:
                ofs << "111" << Code::comp(parser.comp()) << Code::dest(parser.dest()) << Code::jump(parser.jump()) << std::endl;;
                instructionNum++;
                break;

            case Parser::L_INSTRUCTION:
                table.addEntry(parser.symbol(), instructionNum);
                break;
        }
    }
}

void Assembler::initializeSymbolTable() {
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
