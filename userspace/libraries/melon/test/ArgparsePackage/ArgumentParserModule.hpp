#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <Argparse.hpp>
#include <Print.hpp>

namespace Melon::Test
{
        TARWI_MODULE(ArgumentParserModule)
        {
                TARWI_SET_NAME("ArgumentParserModule");

                TARWI_UNIT(unitDescription)
                {
                        Argparse::ArgumentParser parser("Test parser.");
                        parser.run();

                        TARWI_EXPECT(true);
                }

                TARWI_UNIT(unitArguments)
                {
                        char **argv = new char *[2];
                        argv[0] = new char[10];
                        argv[1] = new char[10];
                        strcpy(argv[0], "--flag");
                        strcpy(argv[1], "value");
                        int argc = 2;
                
                        Argparse::ArgumentParser parser;
                        parser.setArguments(argc, argv);

                        bool testArg0 = parser.getArguments()[0] == "--flag";
                        bool testArg1 = parser.getArguments()[1] == "value";

                        delete[] argv[0];
                        delete[] argv[1];
                        delete[] argv;

                        TARWI_EXPECT(testArg0 and testArg1);
                }

                static void callback0()
                {
                        Print::println("Hello, world!");
                }

                TARWI_UNIT(unitParseArguments)
                {
                        char **argv = new char *[2];
                        argv[0] = new char[10];
                        argv[1] = new char[10];
                        strcpy(argv[0], "--flag");
                        strcpy(argv[1], "value");
                        int argc = 1;
                
                        Argparse::ArgumentParser parser("Test parser.");
                        parser.addArgument({{"-f", "--flag"}}, callback0);
                        parser.setArguments(argc, argv);
                        parser.run();

                        delete[] argv[0];
                        delete[] argv[1];
                        delete[] argv;
                        
                        TARWI_EXPECT(true);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitDescription);
                        TARWI_CALL_UNIT(unitArguments);
                        TARWI_CALL_UNIT(unitParseArguments);
                }
        };
} // namespace Melon::Test