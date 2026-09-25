#include <cash/parser.hpp>
#include <cash/token.hpp>
#include <cash/ast.hpp>
#include <cash/error.hpp>
#include <melon/typing.hpp>

using namespace Melon;

namespace Cash
{
        void Parser::flushCommandWord(this Parser &self, NodeCommand *node, String::String &curr, bool &first)
        {
                if (curr.length() == 0)
                        return;

                NodeName *arg = self.node_allocator.allocateNode<NodeName>();
                arg->name = curr;
                arg->index = nullptr;
                arg->fallback = curr;

                if (first) {
                        node->path = arg;
                        first = false;
                } else {
                        node->arguments.pushBack(arg);
                }

                curr = "";
        }

        Parser::Parser(const tokens_t &tokens, String::String source)
                : tokens(tokens), source(source)
        {}

        void Parser::parse(this Parser &self)
        {
                self.reset();

                if (self.tokens.length() == 0)
                        return;

                while (self.token_cursor < self.tokens.length()) {
                        if (NodeDecl *decl = self.parseDecl()) {
                                self.nodes.pushBack(decl);
                                continue;
                        }

                        if (self.expect(TokenType::RawLine)) {
                                if (NodeCommand *cmd = self.parseCommand()) {
                                        self.nodes.pushBack(cmd);
                                        continue;
                                }
                        }

                        if (NodeExpr *expr = self.parseExpr()) {
                                self.nodes.pushBack(expr);
                                continue;
                        }

                        break;
                }
        }

        NodeDecl *Parser::parseDecl(this Parser &self)
        {
                if (self.token_cursor >= self.tokens.length()) {
                        syntaxError(self.precedentToken().value, self.precedentToken().position);
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                if (self.expect(TokenType::Var)) {
                        self.advance();
                        return self.parseVarDecl();
                } else if (self.expect(TokenType::Const)) {
                        self.advance();
                        return self.parseConstDecl();
                }

                self.node_allocator.freeAll();
                return nullptr;
        }

        NodeVarDecl *Parser::parseVarDecl(this Parser &self)
        {
                if (self.expect(TokenType::Name)) {
                        Token name = self.advance();

                        if (self.token_cursor >= self.tokens.length()) {
                                self.node_allocator.freeAll();
                                return nullptr;
                        }

                        if (self.expect(TokenType::Equal)) {
                                self.advance();
                                NodeExpr *value = self.parseExpr();

                                NodeVarDecl *node = self.node_allocator.allocateNode<NodeVarDecl>();
                                node->name = name.value;
                                node->value = value;

                                return node;
                        } else {
                                self.node_allocator.freeAll();
                                return nullptr;
                        }
                }

                self.node_allocator.freeAll();
                return nullptr;
        }

        NodeConstDecl *Parser::parseConstDecl(this Parser &self)
        {
                if (self.expect(TokenType::Name)) {
                        Token name = self.advance();

                        if (self.token_cursor >= self.tokens.length()) {
                                self.node_allocator.freeAll();
                                return nullptr;
                        }

                        if (self.expect(TokenType::Equal)) {
                                self.advance();
                                NodeExpr *value = self.parseExpr();

                                NodeConstDecl *node = self.node_allocator.allocateNode<NodeConstDecl>();
                                node->name = name.value;
                                node->value = value;

                                return node;
                        } else {
                                self.node_allocator.freeAll();
                                return nullptr;
                        }
                }

                self.node_allocator.freeAll();
                return nullptr;
        }

        NodeExpr *Parser::parseExpr(this Parser &self)
        {
                NodeExpr *left = self.parseTerm();
                if (not left) {
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                while (self.token_cursor < self.tokens.length()) {
                        if (not self.expect(TokenType::Plus)
                        and not self.expect(TokenType::Minus)
                        and not self.expect(TokenType::Or)
                        and not self.expect(TokenType::Xor)
                        and not self.expect(TokenType::EqualEqual)
                        and not self.expect(TokenType::NotEqual)
                        and not self.expect(TokenType::LesserThan)
                        and not self.expect(TokenType::LesserThanEqual)
                        and not self.expect(TokenType::GreaterThan)
                        and not self.expect(TokenType::GreaterThanEqual)
                        )
                                break;

                        Token op = self.advance();

                        NodeExpr *right = self.parseTerm();
                        if (not right) {
                                self.node_allocator.freeAll();
                                return nullptr;
                        }

                        NodeBinaryOp *node = self.node_allocator.allocateNode<NodeBinaryOp>();
                        node->left = left;
                        node->right = right;
                        node->op = op.type;

                        left = node;
                }

                return left;
        }

        NodeExpr *Parser::parseTerm(this Parser &self)
        {
                NodeExpr *left = self.parsePrimaryExpr();
                if (not left) {
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                while (self.token_cursor < self.tokens.length()) {
                        if (not self.expect(TokenType::Star)
                        and not self.expect(TokenType::Slash)
                        and not self.expect(TokenType::And))
                                break;

                        Token op = self.advance();

                        NodeExpr *right = self.parsePrimaryExpr();
                        if (not right) {
                                self.node_allocator.freeAll();
                                return nullptr;
                        }

                        NodeBinaryOp *node = self.node_allocator.allocateNode<NodeBinaryOp>();
                        node->left = left;
                        node->right = right;
                        node->op = op.type;

                        left = node;
                }

                return left;
        }

        NodeExpr *Parser::parsePrimaryExpr(this Parser &self)
        {
                if (self.token_cursor >= self.tokens.length()) {
                        syntaxError(self.precedentToken().value, self.precedentToken().position);
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                if (self.expect(TokenType::Plus)
                or self.expect(TokenType::Minus)
                or self.expect(TokenType::Not))
                        return self.parseUnaryOp();

                if (self.expect(TokenType::LeftParenthesis)) {
                        self.advance();

                        NodeExpr *inner = self.parseExpr();

                        if (self.token_cursor >= self.tokens.length() or not self.expect(TokenType::RightParenthesis)) {
                                syntaxError(self.precedentToken().value, self.precedentToken().position);
                                self.node_allocator.freeAll();
                                return nullptr;
                        }

                        self.advance();

                        return inner;
                }

                if (NodeInteger *int_node = self.parseInteger())
                        return int_node;
                else if (NodeBool *bool_node = self.parseBool())
                        return bool_node;
                else if (NodeAssign *assign_node = self.parseAssign())
                        return assign_node;
                else if (NodeString *string_node = self.parseString())
                        return string_node;
                else if (NodeList *list_node = self.parseList())
                        return list_node;
                return self.parseName();
        }

        NodeUnaryOp *Parser::parseUnaryOp(this Parser &self)
        {
                if (self.token_cursor >= self.tokens.length()) {
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                Token op = self.advance();

                NodeExpr *value = self.parsePrimaryExpr();
                if (not value) {
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                NodeUnaryOp *node = self.node_allocator.allocateNode<NodeUnaryOp>();
                node->value = value;
                node->op = op.type;

                return node;
        }

        NodeInteger *Parser::parseInteger(this Parser &self)
        {
                if (self.token_cursor >= self.tokens.length()) {
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                if (self.expect(TokenType::Integer)) {
                        self.advance();

                        NodeInteger *node = self.node_allocator.allocateNode<NodeInteger>();
                        node->value = self.precedentToken().value;

                        return node;
                }

                self.node_allocator.freeAll();
                return nullptr;
        };

        NodeBool *Parser::parseBool(this Parser &self)
        {
                if (self.token_cursor >= self.tokens.length()) {
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                if (self.expect(TokenType::True) or self.expect(TokenType::False)) {
                        self.advance();

                        NodeBool *node = self.node_allocator.allocateNode<NodeBool>();
                        node->value = self.precedentToken().value;

                        return node;
                }

                self.node_allocator.freeAll();
                return nullptr;
        }

        NodeName *Parser::parseName(this Parser &self)
        {
                if (self.token_cursor >= self.tokens.length()) {
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                Token name = self.currentToken();

                if (self.expect(TokenType::Name)) {
                        self.advance();

                        if (self.expect(TokenType::LeftSquareBrace)) {
                                self.advance();
                                NodeName *node = self.node_allocator.allocateNode<NodeName>();
                                node->name = name.value;
                                node->index = self.parseExpr();

                                if (not self.expect(TokenType::RightSquareBrace)) {
                                        syntaxError(self.currentToken().value, self.currentToken().position);
                                        self.node_allocator.freeAll();
                                        return nullptr;
                                }
                                
                                return node;
                        } else {
                                NodeName *node = self.node_allocator.allocateNode<NodeName>();
                                node->name = name.value;
                                node->index = nullptr;
                                return node;
                        }
                }

                self.node_allocator.freeAll();
                return nullptr;
        }

        NodeString *Parser::parseString(this Parser &self)
        {
                if (self.token_cursor >= self.tokens.length()) {
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                if (self.expect(TokenType::String)) {
                        self.advance();

                        NodeString *node = self.node_allocator.allocateNode<NodeString>();
                        node->value = self.precedentToken().value;

                        return node;
                }

                self.node_allocator.freeAll();
                return nullptr;
        }

        NodeAssign *Parser::parseAssign(this Parser &self)
        {
                Typing::USize start_cursor = self.token_cursor;
                NodeExpr *index;

                if (self.token_cursor >= self.tokens.length()) {
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                if (self.expect(TokenType::Name)) {
                        Token name = self.advance();

                        if (self.token_cursor >= self.tokens.length()) {
                                self.token_cursor = start_cursor;
                                self.node_allocator.freeAll();
                                return nullptr;
                        }

                        if (self.expect(TokenType::LeftSquareBrace)) {
                                self.advance();
                                index = self.parseExpr();
                                
                                if (not self.expect(TokenType::RightSquareBrace)) {
                                        syntaxError(self.currentToken().value, self.currentToken().position);
                                        self.node_allocator.freeAll();
                                        return nullptr;
                                }

                                self.advance();
                        }

                        if (self.expect(TokenType::Equal)) {
                                self.advance();

                                NodeExpr *value = self.parseExpr();
                                if (value) {
                                        NodeAssign *node = self.node_allocator.allocateNode<NodeAssign>();
                                        node->name = name.value;
                                        node->index = index;
                                        node->value = value;

                                        return node;
                                }
                        }
                }

                self.token_cursor = start_cursor;
                self.node_allocator.freeAll();
                return nullptr;
        }

        NodeList *Parser::parseList(this Parser &self)
        {
                if (self.token_cursor >= self.tokens.length()) {
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                if (self.expect(TokenType::LeftSquareBrace)) {
                        NodeList *node = self.node_allocator.allocateNode<NodeList>();
                        self.advance();

                        if (not self.expect(TokenType::RightSquareBrace)) {
                                while (true) {
                                        NodeExpr *expr = self.parseExpr();
                                        if (not expr) {
                                                self.node_allocator.freeAll();
                                                return nullptr;
                                        }
                                        node->content.pushBack(expr);

                                        if (self.token_cursor >= self.tokens.length()) {
                                                syntaxError(self.precedentToken().value, self.precedentToken().position);
                                                self.node_allocator.freeAll();
                                                return nullptr;
                                        }

                                        if (self.expect(TokenType::Comma)) {
                                                self.advance();
                                                continue;
                                        }

                                        break;
                                }
                        }

                        if (not self.expect(TokenType::RightSquareBrace)) {
                                syntaxError(self.currentToken().value, self.currentToken().position);
                                self.node_allocator.freeAll();
                                return nullptr;
                        }

                        self.advance();

                        return node;
                }

                self.node_allocator.freeAll();
                return nullptr;
        }

        NodeCommand *Parser::parseCommand(this Parser &self)
        {
                if (self.token_cursor >= self.tokens.length()) {
                        syntaxError(self.precedentToken().value, self.precedentToken().position);
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                Token tok = self.advance();
                String::String line = tok.value;

                NodeCommand *node = self.node_allocator.allocateNode<NodeCommand>();
                String::String curr;
                bool is_in_quotes = false;
                bool is_first_word = true;

                for (Typing::USize i = 0; i < line.length(); ++i) {
                        char c = line[i];

                        if (c == '"') {
                                is_in_quotes = not is_in_quotes;
                                continue;
                        }

                        if (Typing::isSpace(c) and not is_in_quotes) {
                                self.flushCommandWord(node, curr, is_first_word);
                                continue;
                        }

                        curr.appendChar(c);
                }

                self.flushCommandWord(node, curr, is_first_word);

                if (is_first_word) {
                        syntaxError(tok.value, tok.position);
                        self.node_allocator.freeAll();
                        return nullptr;
                }

                return node;
        }

        void Parser::reset(this Parser &self)
        {
                self.token_cursor = 0;
                self.node_cursor = 0;
                self.nodes.clear();
                self.node_allocator.freeAll();
        }

        void Parser::setTokens(this Parser &self, const tokens_t &new_tokens)
        {
                self.tokens = new_tokens;
        }

        void Parser::setSource(this Parser &self, const String::String &new_source)
        {
                self.source = new_source;
        }

        bool Parser::expect(this const Parser &self, TokenType token)
        {
                if (self.token_cursor >= self.tokens.length())
                        return false;
                return self.currentToken().type == token;
        }

        const Token &Parser::advance(this Parser &self)
        {
                const Token &tok = self.currentToken();
                ++self.token_cursor;
                return tok;
        }

        const ast_t &Parser::getNodes(this const Parser &self)
        {
                return self.nodes;
        }
} // namespace Cash