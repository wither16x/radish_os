#pragma once

#include <cash/position.hpp>

#include <melon/string.hpp>

namespace Cash
{
        using tokens_t = Melon::Vector::Vector<struct Token>;

        enum class TokenType
        {
                Integer,
                Name,
                String,
                RawLine,

                Plus,
                Minus,
                Star,
                Slash,
                Equal,
                And,
                Or,
                Xor,
                Not,
                EqualEqual,
                NotEqual,
                LesserThan,
                GreaterThan,
                LesserThanEqual,
                GreaterThanEqual,

                LeftParenthesis,
                RightParenthesis,
                LeftSquareBrace,
                RightSquareBrace,
                Comma,

                Var,
                Const,
                True,
                False,

                EndOfFile
        };

        struct Token
        {
                Position position;
                TokenType type;
                Melon::String::String value = "";
        };

        Melon::String::String tokenTypeToString(TokenType toktype);
} // namespace Cash