#pragma once

#include <string>
#include <unordered_map>


class SymbolTable {
    public:
        void addEntry(const std::string symbol, const int address);
        bool contains(const std::string symbol);
        int getAddress(const std::string symbol);

    private:
        std::unordered_map<std::string, int> table;
};
