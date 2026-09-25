#include <cash/token.hpp>

#include <utility>

using namespace Melon;

namespace Cash
{
        namespace
        {
                const char *token_type_strings[] = {
                        "Integer",
                        "Name",
                        "String",
                        "Rawline",
                        "Plus",
                        "Minus",
                        "Star",
                        "Slash",
                        "Equal",
                        "And",
                        "Or",
                        "Xor",
                        "Not",
                        "EqualEqual",
                        "NotEqual",
                        "LesserThan",
                        "GreaterThan",
                        "LesserThanEqual",
                        "GreaterThanEqual",
                        "LeftParenthesis",
                        "RightParenthesis",
                        "LeftSquareBrace",
                        "RightSquareBrace",
                        "Comma",
                        "Var",
                        "Const",
                        "True",
                        "False",
                        "EndOfFile"
                };
        } // anonymous namespace

        String::String tokenTypeToString(TokenType toktype)
        {
                return token_type_strings[std::to_underlying(toktype)];
        }
} // namespace Cash