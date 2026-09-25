#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <ExceptionsPackage/ExceptionsModule.hpp>

namespace Melon::Test
{
        TARWI_PACKAGE(ExceptionsPackage)
        {
                TARWI_SET_NAME("ExceptionsPackage");

                ExceptionsModule exceptions_module;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(exceptions_module);

                        TARWI_DISPLAY_RESULTS(
                                exceptions_module.successfull_tests,
                                exceptions_module.failed_tests,
                                exceptions_module.skipped_tests
                        );
                }
        };
} // namespace Melon::Test 