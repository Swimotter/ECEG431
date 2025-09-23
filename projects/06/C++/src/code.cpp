#include "code.hpp"


// NOTE: This doesn't handle input with errors
const std::string Code::dest(std::string destHuman)
{
    std::string ret = "000";
    // NOTE: Could just use a big switch table, maybe test for efficiency
    if (destHuman.find("A") != std::string::npos) {
        ret[0] = '1';
    }
    if (destHuman.find("D") != std::string::npos) {
        ret[1] = '1';
    }
    if (destHuman.find("M") != std::string::npos) {
        ret[2] = '1';
    }

    return ret;
}

// NOTE: This doesn't handle input with errors
const std::string Code::comp(std::string compHuman)
{
    if (compHuman == "0") {
        return "0101010";
    }
    else if (compHuman == "1") {
        return "0111111";
    }
    else if (compHuman == "-1") {
        return "0111010";
    }
    else if (compHuman == "D") {
        return "0001100";
    }
    else if (compHuman == "A") {
        return "0110000";
    }
    else if (compHuman == "!D") {
        return "0001101";
    }
    else if (compHuman == "!A") {
        return "0110001";
    }
    else if (compHuman == "-D") {
        return "0001111";
    }
    else if (compHuman == "-A") {
        return "0110011";
    }
    else if (compHuman == "D+1") {
        return "0011111";
    }
    else if (compHuman == "A+1") {
        return "0110111";
    }
    else if (compHuman == "D-1") {
        return "0001110";
    }
    else if (compHuman == "A-1") {
        return "0110010";
    }
    else if (compHuman == "D+A") {
        return "0000010";
    }
    else if (compHuman == "D-A") {
        return "0010011";
    }
    else if (compHuman == "A-D") {
        return "0000111";
    }
    else if (compHuman == "D&A") {
        return "0000000";
    }
    else if (compHuman == "D|A") {
        return "0010101";
    }
    else if (compHuman == "M") {
        return "1110000";
    }
    else if (compHuman == "!M") {
        return "1110001";
    }
    else if (compHuman == "-M") {
        return "1110011";
    }
    else if (compHuman == "M+1") {
        return "1110111";
    }
    else if (compHuman == "M-1") {
        return "110010";
    }
    else if (compHuman == "D+M") {
        return "000010";
    }
    else if (compHuman == "D-M") {
        return "010011";
    }
    else if (compHuman == "M-D") {
        return "000111";
    }
    else if (compHuman == "D&M") {
        return "000000";
    }
    // (compHuman == "D|M")
    else {
        return "010101";
    }
}

// NOTE: This doesn't handle input with errors
const std::string Code::jump(std::string jumpHuman)
{
    if (jumpHuman == "") {
        return "000";
    }
    else if (jumpHuman == "JGT") {
        return "001";
    }
    else if (jumpHuman == "JEQ") {
        return "010";
    }
    else if (jumpHuman == "JGE") {
        return "011";
    }
    else if (jumpHuman == "JLT") {
        return "100";
    }
    else if (jumpHuman == "JNE") {
        return "101";
    }
    else if (jumpHuman == "JLE") {
        return "110";
    }
    // (jumpHuman == "JMP")
    else {
        return "111";
    }
}