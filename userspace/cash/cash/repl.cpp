#include <cash/repl.hpp>
#include <cash/lexer.hpp>
#include <cash/parser.hpp>
#include <cash/interpreter.hpp>

#include <melon/string.hpp>
#include <melon/input.hpp>
#include <melon/print.hpp>

using namespace Melon;

namespace Cash
{
        void repl()
        {
                Lexer lexer;
                Parser parser;
                Interpreter interpreter;

                bool running = true;

                while (running) {
                        String::String command = Input::input("cash> ");
                        if (command.length() == 0)
                                continue;
                        
                        lexer.setData(command);
                        if (not lexer.lex())
                                continue;

                        // for (auto &tok : lexer.getTokens())
                        //         Print::println("{}", tok.value);
                        
                        tokens_t tokens = lexer.getTokens();
                        parser.setSource(command);
                        parser.setTokens(tokens);
                        parser.parse();

                        // for (auto &node : parser.getNodes())
                        //         Print::println("{}", node->info());

                        ast_t nodes = parser.getNodes();
                        interpreter.setNodes(nodes);
                        interpreter.interpret();

                        for (auto &val : interpreter.getEvalValues()) {
                                if (val.value != null_value.value)
                                        Print::println("{}", val.toString());
                        }
                }
        }
} // namespace Cash