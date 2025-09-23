#pragma once

#include <string>
#include <unordered_map>


class SymbolTable {
    public:
        void addEntry(std::string symbol, int address);
        bool contains(std::string symbol);
        int getAddress(std::string symbol);

    private:
        std::unordered_map<std::string, int> table;
};
