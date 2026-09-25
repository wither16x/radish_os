#pragma once

#include <cash/token.hpp>

#include <melon/string.hpp>
#include <melon/fmt.hpp>
#include <typeinfo>

namespace Cash
{
        using ast_t = Melon::Vector::Vector<struct Node *>;

        struct Node
        {
                virtual ~Node()
                {}

                inline virtual Melon::String::String info() const
                {
                        return "Node()";
                }
        };

        struct NodeExpr : Node
        {
                inline Melon::String::String info() const override
                {
                        return "NodeExpr()";
                }
        };

        struct NodeDecl : Node
        {
                inline Melon::String::String info() const override
                {
                        return "NodeDecl()";
                }
        };

        struct NodeVarDecl : NodeDecl
        {
                Melon::String::String name;
                NodeExpr *value;

                inline Melon::String::String info() const override
                {
                        return Melon::Fmt::formatString("NodeVarDecl(name={}, value={})",
                                this->name, this->value->info()
                        );
                }
        };

        struct NodeConstDecl : NodeDecl
        {
                Melon::String::String name;
                NodeExpr *value;

                inline Melon::String::String info() const override
                {
                        return Melon::Fmt::formatString("NodeConstDecl(name={}, value={})",
                                this->name, this->value->info()
                        );
                }
        };

        struct NodeUnaryOp : NodeExpr
        {
                NodeExpr *value;
                TokenType op;

                inline Melon::String::String info() const override
                {
                        return Melon::Fmt::formatString("NodeUnaryOp(value={}, op={})",
                                this->value->info(), tokenTypeToString(this->op)
                        );
                }
        };

        struct NodeBinaryOp : NodeExpr
        {
                NodeExpr *left;
                NodeExpr *right;
                TokenType op;

                inline Melon::String::String info() const override
                {
                        return Melon::Fmt::formatString("NodeBinaryOp(left={}, right={}, op={})",
                                this->left->info(), this->right->info(), tokenTypeToString(this->op)
                        );
                }
        };

        struct NodeInteger : NodeExpr
        {
                Melon::String::String value;

                inline Melon::String::String info() const override
                {
                        return Melon::Fmt::formatString("NodeInteger(value={})",
                                this->value
                        );
                }
        };

        struct NodeBool : NodeExpr
        {
                Melon::String::String value;

                inline Melon::String::String info() const override
                {
                        return Melon::Fmt::formatString("NodeBool(value={})",
                                this->value
                        );
                }
        };

        struct NodeName : NodeExpr
        {
                Melon::String::String name;
                Melon::String::String fallback;
                NodeExpr *index;

                inline Melon::String::String info() const override
                {
                        return Melon::Fmt::formatString("NodeName(name={}, fallback={}, index={})",
                                this->name, this->fallback, this->index ? this->index->info() : "(nullptr)"
                        );
                }
        };

        struct NodeString : NodeExpr
        {
                Melon::String::String value;

                inline Melon::String::String info() const override
                {
                        return Melon::Fmt::formatString("NodeString(value={})",
                                this->value
                        );
                }
        };

        struct NodeAssign : NodeExpr
        {
                Melon::String::String name;
                NodeExpr *index;
                NodeExpr *value;

                inline Melon::String::String info() const override
                {
                        return Melon::Fmt::formatString("NodeAssign(name={}, index={}, value={})",
                                this->name, this->index ? this->index->info() : "(nullptr)", this->value->info()
                        );
                }
        };

        struct NodeList : NodeExpr
        {
                Melon::Vector::Vector<NodeExpr *> content;

                inline Melon::String::String info() const override
                {
                        return "NodeList()";
                }
        };

        struct NodeCommand : NodeExpr
        {
                NodeExpr *path;
                Melon::Vector::Vector<NodeExpr *> arguments;

                inline Melon::String::String info() const override
                {
                        return Melon::Fmt::formatString("NodeCommand(path={}, arguments=())",
                                path->info()
                        );
                }
        };

        template<typename T>
        inline constexpr bool isNodeType(Node *const &node)
        {
                const std::type_info &node_type = typeid(*node);
                return node_type == typeid(T);
        }

        class NodeAllocator
        {
                ast_t nodes;

        public:
                template<typename T>
                T *allocateNode(this NodeAllocator &self)
                {
                        T *node = new T;
                        self.nodes.pushBack(node);
                        return node;
                }

                void freeNode(this NodeAllocator &self, Node *node);
                void freeAll(this NodeAllocator &self);
        };
} // namespace Cash