#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <InputPackage/InputModule.hpp>

namespace Melon::Test
{
        TARWI_PACKAGE(InputPackage)
        {
                TARWI_SET_NAME("InputPackage");

                InputModule input;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(input);

                        TARWI_DISPLAY_RESULTS(
                                input.successfull_tests,
                                input.failed_tests,
                                input.skipped_tests
                        );
                }
        };
} // namespace Melon::Test