#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <ArgparsePackage/ArgumentParserModule.hpp>

namespace Melon::Test
{
        TARWI_PACKAGE(ArgparsePackage)
        {
                TARWI_SET_NAME("ArgparsePackage");

                ArgumentParserModule argument_parser;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(argument_parser);

                        TARWI_DISPLAY_RESULTS(
                                argument_parser.successfull_tests,
                                argument_parser.failed_tests,
                                argument_parser.skipped_tests
                        );
                }
        };
} // namespace Melon::Test