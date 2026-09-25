#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <PrintPackage/PrintModule.hpp>

namespace Melon::Test
{
        TARWI_PACKAGE(PrintPackage)
        {
                TARWI_SET_NAME("PrintPackage");

                PrintModule print;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(print);

                        TARWI_DISPLAY_RESULTS(
                                print.successfull_tests,
                                print.failed_tests,
                                print.skipped_tests
                        );
                }
        };
} // namespace Melon::Test