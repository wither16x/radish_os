#pragma once

#include <cash/ast.hpp>
#include <cash/token.hpp>

#include <melon/vector.hpp>
#include <melon/typing.hpp>

namespace Cash
{
        class Parser
        {
                ast_t nodes;
                tokens_t tokens;
                Melon::String::String source;
                NodeAllocator node_allocator;
                Melon::Typing::USize token_cursor;
                Melon::Typing::USize node_cursor;

                void flushCommandWord(this Parser &self, NodeCommand *node, Melon::String::String &curr, bool &first);

        public:
                Parser() = default;
                Parser(const tokens_t &tokens, Melon::String::String source);

                void parse(this Parser &self);
                NodeDecl *parseDecl(this Parser &self);
                NodeVarDecl *parseVarDecl(this Parser &self);
                NodeConstDecl *parseConstDecl(this Parser &self);
                NodeExpr *parseExpr(this Parser &self);
                NodeExpr *parseTerm(this Parser &self);
                NodeExpr *parsePrimaryExpr(this Parser &self);
                NodeUnaryOp *parseUnaryOp(this Parser &self);
                NodeInteger *parseInteger(this Parser &self);
                NodeBool *parseBool(this Parser &self);
                NodeName *parseName(this Parser &self);
                NodeString *parseString(this Parser &self);
                NodeAssign *parseAssign(this Parser &self);
                NodeList *parseList(this Parser &self);
                NodeCommand *parseCommand(this Parser &self);

                void reset(this Parser &self);
                void setTokens(this Parser &self, const tokens_t &new_tokens);
                void setSource(this Parser &self, const Melon::String::String &new_source);
                bool expect(this const Parser &self, TokenType token);
                const Token &advance(this Parser &self);
        
                const ast_t &getNodes(this const Parser &self);

                inline const Token &currentToken(this const Parser &self)
                {
                        return self.tokens[self.token_cursor];
                }

                inline const Token &precedentToken(this const Parser &self)
                {
                        return self.tokens[self.token_cursor - 1];
                }

                inline const Token &nextToken(this const Parser &self)
                {
                        return self.tokens[self.token_cursor + 1];
                }
        };
} // namespace Cash