#pragma once

#include <string>
#include <unordered_map>


class JackSymbolTable
{
    public:
        enum class Kind {
            NONE,
            STATIC,
            FIELD,
            ARG,
            VAR
        };

        struct Symbol {
            std::string name;
            std::string type;
            Kind kind;
            int index;

            // Symbol(const std::string& name, const std::string& type, Kind kind, int index) : name(name), type(type), kind(kind), index(index) {}
        };

        JackSymbolTable();

        void reset();
        void define(std::string name, std::string type, Kind kind);
        int varCount(Kind kind);
        bool contains(const std::string& name);
        Kind kindOf(const std::string& name);
        std::string typeOf(const std::string& name);
        int indexOf(const std::string& name);

    private:
        std::unordered_map<std::string, Symbol> scope;
        int numStatic;
        int numField;
        int numArg;
        int numVar;
};
