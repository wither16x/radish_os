#pragma once

#include <cash/position.hpp>
#include <cash/token.hpp>

#include <melon/vector.hpp>
#include <melon/string.hpp>

namespace Cash
{
        class Lexer
        {
                tokens_t tokens;
                Position position;
                Melon::Typing::USize cursor;
                Melon::String::String curr_integer;
                Melon::String::String curr_name;
                Melon::String::String curr_string;
                Melon::String::String data;
                bool lexing;
                bool has_error;

                bool looksLikeCommandStart(this const Lexer &self);

        public:
                Lexer(const Melon::String::String &data = "");

                bool lex(this Lexer &self);
                void reset(this Lexer &self);
                void advance(this Lexer &self);
                void setData(this Lexer &self, const Melon::String::String &new_data);
                void addToken(this Lexer &self, TokenType type, const Melon::String::String &value);
                void error(this Lexer &self);

                bool foundBlank(this const Lexer &self);
                bool foundDigit(this const Lexer &self);

                const tokens_t &getTokens(this const Lexer &self);
        };
} // namespace Cash