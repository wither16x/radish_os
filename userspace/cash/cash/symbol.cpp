#include <cash/symbol.hpp>
#include <cash/error.hpp>

#include <melon/string.hpp>

using namespace Melon;

namespace Cash
{
        Symbol::Symbol(const String::String &name, EvalValue value, SymbolType type)
                : name(name), value(value), type(type)
        {}

        SymbolTable::SymbolTable()
        {
                this->symbols.emplaceBack(
                        "__undefined__",
                        null_value,
                        SymbolType::Constant
                );
        }

        bool SymbolTable::addSymbol(this SymbolTable &self, const Symbol &symbol)
        {
                if (self.hasSymbol(symbol.name)) {
                        alreadyDeclaredError(symbol.name);
                        return false;
                }

                self.symbols.pushBack(symbol);
                return true;
        }

        bool SymbolTable::removeSymbol(this SymbolTable &self, const Symbol &symbol)
        {
                for (Melon::Typing::USize i = 0; i < self.symbols.length(); i++) {
                        if (self.symbols[i].name == symbol.name) {
                                self.symbols.erase(i);
                                return true;
                        }
                }

                notDeclaredError(symbol.name);
                return false;
        }

        void SymbolTable::clear(this SymbolTable &self)
        {
                Typing::USize i = 0;
                while (self.symbols[i].name != "__undefined__")
                        self.symbols.erase(i);
        }

        bool SymbolTable::hasSymbol(this const SymbolTable &self, const String::String &name)
        {
                for (auto &sym : self.symbols) {
                        if (sym.name == name)
                                return true;
                }

                return false;
        }

        const Symbol &SymbolTable::getSymbol(this const SymbolTable &self, const String::String &name)
        {
                for (auto &sym : self.symbols) {
                        if (sym.name == name)
                                return sym;
                }

                return self.symbols[0];
        }

        void SymbolTable::setSymbolValue(this SymbolTable &self, const String::String &name, EvalValue new_value)
        {
                for (auto &sym : self.symbols) {
                        if (sym.name == name) {
                                if (sym.type == SymbolType::Constant) {
                                        isConstantError(name);
                                        return;
                                }

                                sym.value = new_value;
                                return;
                        }
                }

                notDeclaredError(name);
        }
} // namespace Cash