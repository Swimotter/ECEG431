#include "symbolTable.hpp"


void SymbolTable::addEntry(const std::string symbol, const int address)
{
    table[symbol] = address;
}

bool SymbolTable::contains(const std::string symbol)
{
    return table.contains(symbol);
}

int SymbolTable::getAddress(const std::string symbol)
{
    return table.at(symbol);
}
