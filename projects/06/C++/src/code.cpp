#include "code.hpp"


std::string Code::dest(std::string destHuman)
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

std::string Code::comp(std::string compHuman)
{
    switch (compHuman)
    {
        case "0":
            return "0101010";

        case "1":
            return "0111111";

        case "-1":
            return "0111010";

        case "D":
            return "0001100";

        case "A":
            return "0110000";

        case "!D":
            return "0001101";

        case "!A":
            return "0110001"

        case "-D":
            return "0001111";

        case "-A":
            return "0110011";

        case "D+1":
            return "0011111";

        case "A+1":
            return "0110111";

        case "D-1":
            return "0001110";

        case "A-1":
            return "0110010";

        case "D+A":
            return "0000010";

        case "D-A":
            return "0010011";

        case "A-D":
            return "0000111";

        case "D&A":
            return "0000000";

        case "D|A":
            return "0010101";

        case "M":
            return "1110000"

        case "!M":
            return "1110001"

        case "-M":
            return "1110011"

        case "M+1":
            return "1110111"

        case "M-1":
            return "110010"

        case "D+M":
            return "000010"

        case "D-M":
            return "010011"

        case "M-D":
            return "000111"

        case "D&M":
            return "000000"

        case "D|M":
            return "010101"
    }
}

std::string Code::jump(std::string jumpHuman)
{
    switch (jumpHuman)
    {
        case "":
            return "000";

        case "JGT":
            return "001";

        case "JEQ":
            return "010";

        case "JGE":
            return "011";

        case "JLT":
            return "100";

        case "JNE":
            return "101";

        case "JLE":
            return "110";

        case "JMP":
            return "111";
    }
}