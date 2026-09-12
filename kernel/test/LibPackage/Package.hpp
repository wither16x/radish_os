#pragma once

#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <LibPackage/BufferModule.hpp>

namespace Kiwi::Test::LibPackage
{
        TARWI_PACKAGE(LibPackage)
        {
                TARWI_SET_NAME("LibPackage");

                BufferModule            buffer_module;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(buffer_module);

                        TARWI_DISPLAY_RESULTS(
                                buffer_module.successfull_tests,
                                buffer_module.failed_tests,
                                buffer_module.skipped_tests
                        );
                }
        };
} // namespave Kiwi::Test::LibPackage