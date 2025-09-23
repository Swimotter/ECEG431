#pragma once

#include <string>


class Code {
    public:
        static std::string dest(std::string destHuman);
        static std::string comp(std::string compHuman);
        static std::string jump(std::string jumpHuman);
};
