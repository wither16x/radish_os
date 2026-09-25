#include <cash/ast.hpp>
#include <cash/token.hpp>
#include <cash/interpreter.hpp>
#include <cash/error.hpp>

#include <melon/conversion.hpp>
#include <variant>

#include <unistd.h>

using namespace Melon;

namespace Cash
{
        EvalValue Interpreter::runCommand(const EvalValue &path, const Vector::Vector<EvalValue> &arguments)
        {
                String::String path_str = path.toString();
                Vector::Vector<String::String> strings;

                strings.pushBack(path_str);
                for (const auto &arg : arguments)
                        strings.pushBack(arg.toString());

                char **argv = new char *[strings.length() + 1];

                for (Typing::USize i = 0; i < strings.length(); ++i) {
                        const char *raw = strings[i].raw();
                        argv[i] = new char[strlen(raw) + 1];
                        strcpy(argv[i], raw);
                }
                argv[strings.length()] = nullptr;

                pid_t pid = fork();

                if (pid == 0) {
                        exec(path_str.raw(), static_cast<int>(strings.length()), argv, nullptr);
                        return null_value;
                } else {
                        wait();
                }

                for (Typing::USize i = 0; i < strings.length(); ++i)
                        delete[] argv[i];
                delete[] argv;

                return null_value;
        }

        Interpreter::Interpreter(const ast_t &nodes)
                : nodes(nodes)
        {}

        void Interpreter::interpret(this Interpreter &self)
        {
                self.reset();

                for (auto &nd : self.nodes)
                        self.eval_values.pushBack(self.evaluate(nd));
                }

        EvalValue Interpreter::evaluate(this Interpreter &self, Node *const &node)
        {
                if (isNodeType<Node>(node)) {
                        return null_value;
                } else if (isNodeType<NodeExpr>(node)) {
                        return null_value;
                } else if (isNodeType<NodeDecl>(node)) {
                        return null_value;
                } else if (isNodeType<NodeVarDecl>(node)) {
                        NodeVarDecl *vardecl_node = static_cast<NodeVarDecl *>(node);
                        EvalValue value = self.evaluate(vardecl_node->value);

                        Symbol symbol(
                                vardecl_node->name,
                                value,
                                SymbolType::Variable
                        );
                        
                        self.symbol_table.addSymbol(symbol);
                        return null_value;
                } else if (isNodeType<NodeConstDecl>(node)) {
                        NodeConstDecl *vardecl_node = static_cast<NodeConstDecl *>(node);
                        EvalValue value = self.evaluate(vardecl_node->value);

                        Symbol symbol(
                                vardecl_node->name,
                                value,
                                SymbolType::Constant
                        );

                        self.symbol_table.addSymbol(symbol);
                        return null_value;
                } else if (isNodeType<NodeUnaryOp>(node)) {
                        NodeUnaryOp *unop_node = static_cast<NodeUnaryOp *>(node);
                        EvalValue val = self.evaluate(unop_node->value);

                        int int_val = val.toInt();

                        if (unop_node->op == TokenType::Not) {
                                bool result = not int_val;
                                EvalValue value = {
                                        result
                                };
                                return value;
                        }
                        
                        int result;
                        if (unop_node->op == TokenType::Plus)
                                result = +int_val;
                        else if (unop_node->op == TokenType::Minus)
                                result = -int_val;

                        EvalValue value = {
                                result
                        };
                        return value;
                } else if (isNodeType<NodeBinaryOp>(node)) {
                        NodeBinaryOp *binop_node = static_cast<NodeBinaryOp *>(node);
                        EvalValue left = self.evaluate(binop_node->left);
                        EvalValue right = self.evaluate(binop_node->right);
                        
                        if (std::holds_alternative<String::String>(left.value)
                        and std::holds_alternative<String::String>(right.value)) {
                                if (binop_node->op == TokenType::Plus)
                                        return {left.toString() + right.toString()};
                                else if (binop_node->op == TokenType::EqualEqual)
                                        return {left.toString() == right.toString()};
                                else if (binop_node->op == TokenType::NotEqual)
                                        return {left.toString() != right.toString()};
                        }

                        int left_val = left.toInt();
                        int right_val = right.toInt();

                        if (binop_node->op == TokenType::And
                        or binop_node->op == TokenType::Or
                        or binop_node->op == TokenType::Xor
                        or binop_node->op == TokenType::EqualEqual
                        or binop_node->op == TokenType::NotEqual
                        or binop_node->op == TokenType::LesserThan
                        or binop_node->op == TokenType::LesserThanEqual
                        or binop_node->op == TokenType::GreaterThan
                        or binop_node->op == TokenType::GreaterThanEqual) {
                                bool result;

                                if (binop_node->op == TokenType::And)
                                        result = left_val and right_val;
                                else if (binop_node->op == TokenType::Or)
                                        result = left_val or right_val;
                                else if (binop_node->op == TokenType::Xor)
                                        result = left_val xor right_val;
                                else if (binop_node->op == TokenType::EqualEqual)
                                        result = left_val == right_val;
                                else if (binop_node->op == TokenType::NotEqual)
                                        result = left_val != right_val;
                                else if (binop_node->op == TokenType::LesserThan)
                                        result = left_val < right_val;
                                else if (binop_node->op == TokenType::LesserThanEqual)
                                        result = left_val <= right_val;
                                else if (binop_node->op == TokenType::GreaterThan)
                                        result = left_val > right_val;
                                else if (binop_node->op == TokenType::GreaterThanEqual)
                                        result = left_val >= right_val;

                                EvalValue value = {
                                        result
                                };
                                return value;
                        }

                        int result;
                        if (binop_node->op == TokenType::Plus) {
                                result = left_val + right_val;
                        } else if (binop_node->op == TokenType::Minus) {
                                result = left_val - right_val;
                        } else if (binop_node->op == TokenType::Star) {
                                result = left_val * right_val;
                        } else if (binop_node->op == TokenType::Slash) {
                                if (right_val == 0) {
                                        divisionByZeroError(left_val);
                                        return null_value;
                                }
                                result = left_val / right_val;
                        }

                        EvalValue value = {
                                result
                        };
                        return value;
                } else if (isNodeType<NodeInteger>(node)) {
                        NodeInteger *int_node = static_cast<NodeInteger *>(node);

                        EvalValue value = {
                                Conversion::stringToInt<int>(int_node->value)
                        };
                        return value;
                } else if (isNodeType<NodeName>(node)) {
                        NodeName *name_node = static_cast<NodeName *>(node);
                        const Symbol &symbol = self.symbol_table.getSymbol(name_node->name);
                        
                        if (not symbol.isDefined()) {
                                if (name_node->fallback.length() != 0)
                                        return EvalValue{name_node->fallback};
                                notDeclaredError(name_node->name);
                                return null_value;
                        }
                        
                        if (name_node->index) {
                                EvalValue index = self.evaluate(name_node->index);
                                if (index.toInt() < 0 or static_cast<Typing::USize>(index.toInt()) >= symbol.value.toVector().length()) {
                                        indexOutOfRangeError(index.toInt(), symbol.value.toVector().length());
                                        return null_value;
                                }
                                EvalValue value = symbol.value.toVector()[index.toInt()];
                                return value;
                        } else {
                                EvalValue value = symbol.value;
                                return value;
                        }
                } else if (isNodeType<NodeAssign>(node)) {
                        NodeAssign *assign_node = static_cast<NodeAssign *>(node);
                        EvalValue value = self.evaluate(assign_node->value);
                        const Symbol &symbol = self.symbol_table.getSymbol(assign_node->name);
                        if (not symbol.isDefined())
                                return null_value;

                        if (assign_node->index) {
                                EvalValue index = self.evaluate(assign_node->index);
                                int idx = index.toInt();

                                Vector::Vector<EvalValue> vec = symbol.value.toVector();

                                if (idx < 0 or static_cast<Typing::USize>(idx) >= vec.length()) {
                                        indexOutOfRangeError(idx, vec.length());
                                        return null_value;
                                }

                                vec[idx] = value;
                                self.symbol_table.setSymbolValue(assign_node->name, EvalValue{vec});
                                return self.symbol_table.getSymbol(assign_node->name).value;
                        } else {
                                self.symbol_table.setSymbolValue(assign_node->name, value);
                                return self.symbol_table.getSymbol(assign_node->name).value;
                        }
                } else if (isNodeType<NodeBool>(node)) {
                        NodeBool *bool_node = static_cast<NodeBool *>(node);
                        EvalValue value = {
                                bool_node->value == "true" ? true : false
                        };
                        return value;
                } else if (isNodeType<NodeString>(node)) {
                        NodeString *string_node = static_cast<NodeString *>(node);
                        EvalValue value = {
                                string_node->value
                        };
                        return value;
                } else if (isNodeType<NodeList>(node)) {
                        NodeList *list_node = static_cast<NodeList *>(node);
                        
                        Vector::Vector<EvalValue> eval_values;
                        for (auto &expr : list_node->content)
                                eval_values.pushBack(self.evaluate(expr));

                        EvalValue value = {
                                eval_values
                        };

                        return value;
                } else if (isNodeType<NodeCommand>(node)) {
                        NodeCommand *commandd_node = static_cast<NodeCommand *>(node);

                        EvalValue path = self.evaluate(commandd_node->path);

                        Vector::Vector<EvalValue> arguments;
                        for (auto &arg : commandd_node->arguments)
                                arguments.pushBack(self.evaluate(arg));

                        return self.runCommand(path, arguments);
                }

                return null_value;
        }

        void Interpreter::reset(this Interpreter &self)
        {
                self.eval_values.clear();
                self.symbol_table.clear();
        }

        void Interpreter::setNodes(this Interpreter &self, const ast_t &new_nodes)
        {
                self.nodes = new_nodes;
        }

        const eval_values_t &Interpreter::getEvalValues(this const Interpreter &self)
        {
                return self.eval_values;
        }
} // namespace Cash