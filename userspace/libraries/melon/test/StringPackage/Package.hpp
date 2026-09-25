#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <StringPackage/StringModule.hpp>

namespace Melon::Test
{
        TARWI_PACKAGE(StringPackage)
        {
                TARWI_SET_NAME("StringPackage");

                StringModule string_module;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(string_module);

                        TARWI_DISPLAY_RESULTS(
                                string_module.successfull_tests,
                                string_module.failed_tests,
                                string_module.skipped_tests
                        );
                }
        };
} // namespace Melon::Test