#pragma once

#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <LibPackage/ConversionModule.hpp>
#include <LibPackage/VectorModule.hpp>

namespace Kiwi::Test::LibPackage
{
        TARWI_PACKAGE(LibPackage)
        {
                TARWI_SET_NAME("LibPackage");

                ConversionModule        conversion_module;
                VectorModule            vector_module;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(conversion_module);
                        TARWI_RUN_MODULE(vector_module);

                        TARWI_DISPLAY_RESULTS(
                                conversion_module.successfull_tests + vector_module.successfull_tests,
                                conversion_module.failed_tests + vector_module.failed_tests,
                                conversion_module.skipped_tests + vector_module.skipped_tests
                        );
                }
        };
} // namespave Kiwi::Test::LibPackage