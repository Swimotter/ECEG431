#include "jackSymbolTable.hpp"

#include <unordered_set>
#include <iostream>


JackSymbolTable::JackSymbolTable()
{
    scope = {};
    numStatic = 0;
    numField = 0;
    numArg = 0;
    numVar = 0;
}

void JackSymbolTable::reset()
{
    scope.clear();
    numStatic = 0;
    numField = 0;
    numArg = 0;
    numVar = 0;
}

void JackSymbolTable::define(std::string name, std::string type, Kind kind)
{
    Symbol symbol;
    symbol.name = name;
    symbol.type = type;
    symbol.kind = kind;
    symbol.index = varCount(kind);
    
    // std::cout << symbol.name << ", " << symbol.type << ", " << (int)symbol.kind << ", " << symbol.index << std::endl;

    switch (kind)
    {
        case JackSymbolTable::Kind::STATIC:
            numStatic++;
            break;

        case JackSymbolTable::Kind::FIELD:
            numField++;
            break;

        case JackSymbolTable::Kind::ARG:
            numArg++;
            break;

        case JackSymbolTable::Kind::VAR:
            numVar++;
            break;

        default:
            break;
    }

    scope[name] = symbol;


}

int JackSymbolTable::varCount(JackSymbolTable::Kind kind)
{
    switch (kind)
    {
        case JackSymbolTable::Kind::STATIC:
            return numStatic;

        case JackSymbolTable::Kind::FIELD:
            return numField;

        case JackSymbolTable::Kind::ARG:
            return numArg;

        case JackSymbolTable::Kind::VAR:
            return numVar;

        default:
            return 0;
    }
}

bool JackSymbolTable::contains(const std::string &name)
{
    return scope.contains(name);
}

JackSymbolTable::Kind JackSymbolTable::kindOf(const std::string &name)
{
    return scope[name].kind;
}

std::string JackSymbolTable::typeOf(const std::string &name)
{
    return scope[name].type;
}

int JackSymbolTable::indexOf(const std::string &name)
{
    return scope[name].index;
}
