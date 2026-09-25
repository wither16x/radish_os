#include <melon/print.hpp>
#include <melon/filesystem.hpp>
#include <melon/string.hpp>

#include <cash/repl.hpp>
#include <cash/lexer.hpp>
#include <cash/parser.hpp>
#include <cash/interpreter.hpp>

#include <unistd.h>

using namespace Melon;

int main(int argc, char **argv)
{
        if (argc < 2)
                Cash::repl();

        Cash::Lexer lexer;
        Cash::Parser parser;
        Cash::Interpreter interpreter;

        for (int i = 1; i < argc; i++) {
                if (strcmp(argv[i], "--help") == 0 or strcmp(argv[i], "-h") == 0) {
                        Print::println(
                                "cash -- the certainly amazing shell\n\n"
                                "Usage: cash [arguments]\n"
                                "If no option is given, the REPL will be executed.\n"
                                "Options:\n"
                                "\t--help, -h: display this message\n"
                                "\t--file, -f: <pathname>: path to the file"
                        );
                } else if (strcmp(argv[i], "--file") == 0 or strcmp(argv[i], "-f") == 0) {                        
                        if (not argv[i + 1]) {
                                Print::println("Usage: {} <pathname>", argv[i]);
                                break;
                        }
                        
                        FileSystem::File f(argv[i + 1], "r");
                        auto buf = f.read();
                        buf[buf.size() - 1] = '\0';
                        String::String data = buf.get();

                        lexer.setData(data);
                        if (not lexer.lex())
                                break;

                        // for (auto &tok : lexer.getTokens())
                        //         Print::println("{}", tok.value);
                        
                        Cash::tokens_t tokens = lexer.getTokens();
                        parser.setSource(data);
                        parser.setTokens(tokens);
                        parser.parse();

                        // for (auto &node : parser.getNodes())
                        //         Print::println("{}", node->info());

                        Cash::ast_t nodes = parser.getNodes();
                        interpreter.setNodes(nodes);
                        interpreter.interpret();

                        for (auto &val : interpreter.getEvalValues()) {
                                if (val.value != Cash::null_value.value)
                                        Print::println("{}", val.toString());
                        }

                        ++i;
                        continue;
                } else {
                        Print::println("Invalid argument: \"{}\". Try using --help.", argv[i]);
                }
        }

        return 0;
}