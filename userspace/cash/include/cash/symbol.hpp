#pragma once

#include <cash/value.hpp>

#include <melon/string.hpp>
#include <melon/vector.hpp>

namespace Cash
{
        enum class SymbolType
        {
                Variable,
                Constant
        };

        struct Symbol
        {
                Melon::String::String name;
                EvalValue value;
                SymbolType type;

                Symbol(const Melon::String::String &name, EvalValue value, SymbolType type);
                
                inline constexpr bool isDefined(this const Symbol &self)
                {
                        return self.name != "__undefined__";
                }
        };

        class SymbolTable
        {
                Melon::Vector::Vector<Symbol> symbols;

        public:
                SymbolTable();

                bool addSymbol(this SymbolTable &self, const Symbol &symbol);
                bool removeSymbol(this SymbolTable &self, const Symbol &symbol);
                void clear(this SymbolTable &self);
                bool hasSymbol(this const SymbolTable &self, const Melon::String::String &name);
                const Symbol &getSymbol(this const SymbolTable &self, const Melon::String::String &name);
                void setSymbolValue(this SymbolTable &self, const Melon::String::String &name, EvalValue new_value);
        };
} // namespace Cash