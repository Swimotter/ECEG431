#include "symbolTable.hpp"


void SymbolTable::addEntry(std::string symbol, int address)
{
    table[symbol] = address;
}

bool SymbolTable::contains(std::string symbol)
{
    return table.contains(symbol);
}

int SymbolTable::getAddress(std::string symbol)
{
    return table.at(symbol);
}
