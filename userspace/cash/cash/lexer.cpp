#include <cash/lexer.hpp>
#include <cash/token.hpp>
#include <cash/error.hpp>

#include <melon/string.hpp>
#include <melon/typing.hpp>
#include <melon/filesystem.hpp>

using namespace Melon;

namespace Cash
{
        bool Lexer::looksLikeCommandStart(this const Lexer &self)
        {
                char c = self.data[self.cursor];

                if (Typing::isDigit(c))
                        return false;

                if (c == '+' or c == '-' or c == '(' or c == '"' or c == '[')
                        return false;

                if (isalpha(c) or c == '_') {
                        Typing::USize i = self.cursor;
                        String::String word;

                        while (i < self.data.length() and (isalnum(self.data[i]) or self.data[i] == '_')) {
                                word.appendChar(self.data[i]);
                                ++i;
                        }

                        if (word == "var" or word == "const"
                        or word == "true" or word == "false"
                        or word == "and" or word == "or"
                        or word == "xor" or word == "not")
                                return false;

                        Typing::USize j = i;
                        while (j < self.data.length() and Typing::isSpace(self.data[j]))
                                ++j;

                        if (j < self.data.length() and self.data[j] == '='  and (j + 1 >= self.data.length() or self.data[j + 1] != '='))
                                return false;

                        if (j < self.data.length()) {
                                char next = self.data[j];
                                if (next == '+' or next == '-' or next == '*' or next == '/'
                                or next == '=' or next == '!' or next == '<' or next == '>'
                                or next == '[')
                                        return false;
                        }

                        return true;
                }

                return true;
        }

        Lexer::Lexer(const String::String &data)
                : data(data)
        {}

        bool Lexer::lex(this Lexer &self)
        {
                self.reset();
                self.lexing = true;

                while (self.cursor < self.data.length() and self.lexing) {
                        // skip whitespaces, tabs and newlines
                        while (self.cursor < self.data.length() and self.foundBlank())
                                self.advance();

                        if (self.cursor >= self.data.length())
                                break;

                        Position start = self.position;
                        if ((self.position.column == 0 or self.data[self.cursor] == '\n') and self.looksLikeCommandStart()) {
                                self.curr_string = "";
                                while (self.cursor < self.data.length() and self.data[self.cursor] != '\n') {
                                        self.curr_string.appendChar(self.data[self.cursor]);
                                        self.advance();
                                }
                                self.tokens.emplaceBack(start, TokenType::RawLine, self.curr_string);
                                continue;
                        }

                        start = self.position;
                        // handle integers
                        if (self.foundDigit()) {
                                self.curr_integer = "";
                                while (self.cursor < self.data.length() and self.lexing and self.foundDigit()) {
                                        self.curr_integer.appendChar(self.data[self.cursor]);
                                        self.advance();
                                }
                                self.tokens.emplaceBack(start, TokenType::Integer, self.curr_integer);
                                continue;
                        }

                        start = self.position;
                        // names start with a character or a _
                        if (isalpha(self.data[self.cursor]) or self.data[self.cursor] == '_') {
                                self.curr_name = "";
                                while (self.cursor < self.data.length() and self.lexing and (isalnum(self.data[self.cursor]) or self.data[self.cursor] == '_')) {
                                        self.curr_name.appendChar(self.data[self.cursor]);
                                        self.advance();
                                }

                                if (self.curr_name == "var") {
                                        self.tokens.emplaceBack(start, TokenType::Var, self.curr_name);
                                } else if (self.curr_name == "const") {
                                        self.tokens.emplaceBack(start, TokenType::Const, self.curr_name);
                                } else if (self.curr_name == "true") {
                                        self.tokens.emplaceBack(start, TokenType::True, self.curr_name);
                                } else if (self.curr_name == "false") {
                                        self.tokens.emplaceBack(start, TokenType::False, self.curr_name);
                                } else if (self.curr_name == "and") {
                                        self.tokens.emplaceBack(start, TokenType::And, self.curr_name);
                                } else if (self.curr_name == "or") {
                                        self.tokens.emplaceBack(start, TokenType::Or, self.curr_name);
                                } else if (self.curr_name == "xor") {
                                        self.tokens.emplaceBack(start, TokenType::Xor, self.curr_name);
                                } else if (self.curr_name == "not") {
                                        self.tokens.emplaceBack(start, TokenType::Not, self.curr_name);
                                } else {
                                        self.tokens.emplaceBack(start, TokenType::Name, self.curr_name);
                                }
                                continue;
                        }

                        switch (self.data[self.cursor]) {
                        case '+':
                                self.addToken(TokenType::Plus, "+");
                                self.advance();
                                break;

                        case '-':
                                self.addToken(TokenType::Minus, "-");
                                self.advance();
                                break;

                        case '*':
                                self.addToken(TokenType::Star, "*");
                                self.advance();
                                break;

                        case '/':
                                self.addToken(TokenType::Slash, "/");
                                self.advance();
                                break;

                        case '(':
                                self.addToken(TokenType::LeftParenthesis, "(");
                                self.advance();
                                break;

                        case ')':
                                self.addToken(TokenType::RightParenthesis, ")");
                                self.advance();
                                break;

                        case '=':
                                self.advance();
                                if (self.data[self.cursor] == '=')
                                        self.addToken(TokenType::EqualEqual, "==");
                                else
                                        self.addToken(TokenType::Equal, "=");
                                self.advance();
                                break;

                        case '!':
                                self.advance();
                                if (self.data[self.cursor] == '=') {
                                        self.addToken(TokenType::NotEqual, "!=");
                                        self.advance();
                                } else {
                                        self.error();
                                }
                                break;

                        case '<':
                                self.advance();
                                if (self.data[self.cursor] == '=')
                                        self.addToken(TokenType::LesserThanEqual, "<=");
                                else
                                        self.addToken(TokenType::LesserThan, "<");
                                self.advance();
                                break;

                        case '>':
                                self.advance();
                                if (self.data[self.cursor] == '=')
                                        self.addToken(TokenType::GreaterThanEqual, ">=");
                                else
                                        self.addToken(TokenType::GreaterThan, ">");
                                self.advance();
                                break;

                        case '"':
                                self.advance();
                                self.curr_string = "";
                                while (self.data[self.cursor] != '"' and self.cursor < self.data.length()) {
                                        self.curr_string.appendChar(self.data[self.cursor]);
                                        self.advance();
                                }
                                self.addToken(TokenType::String, self.curr_string);
                                self.advance();
                                break;

                        case '[':
                                self.addToken(TokenType::LeftSquareBrace, "[");
                                self.advance();
                                break;

                        case ']':
                                self.addToken(TokenType::RightSquareBrace, "]");
                                self.advance();
                                break;

                        case ',':
                                self.addToken(TokenType::Comma, ",");
                                self.advance();
                                break;

                        case FileSystem::EndOfFile:
                                self.addToken(TokenType::EndOfFile, "EOF");
                                self.advance();
                                break;

                        default:
                                self.error();
                                break;
                        }
                }

                if (self.has_error)
                        return false;
                return true;
        }

        void Lexer::reset(this Lexer &self)
        {
                self.tokens.clear();
                self.cursor = 0;
                self.position = {0, 0, 0};
                self.has_error = false;
        }

        void Lexer::advance(this Lexer &self)
        {
                if (self.data[self.cursor] == '\n') {
                        self.position.column = 0;
                        ++self.position.row;
                } else {
                        ++self.position.column;
                }

                ++self.cursor;
                self.position.offset = self.cursor;
        }

        void Lexer::setData(this Lexer &self, const String::String &new_data)
        {
                self.data = new_data;
        }

        void Lexer::addToken(this Lexer &self, TokenType type, const String::String &value)
        {
                self.tokens.emplaceBack(self.position, type, value);
        }

        void Lexer::error(this Lexer &self)
        {
                illegalCharacterError(self.data[self.cursor], self.position);
                self.lexing = false;
                self.has_error = true;
        }

        bool Lexer::foundBlank(this const Lexer &self)
        {
                return Typing::isSpace(self.data[self.cursor]);
        }

        bool Lexer::foundDigit(this const Lexer &self)
        {
                return Typing::isDigit(self.data[self.cursor]);
        }

        const Vector::Vector<Token> &Lexer::getTokens(this const Lexer &self)
        {
                return self.tokens;
        }
} // namespace Cash