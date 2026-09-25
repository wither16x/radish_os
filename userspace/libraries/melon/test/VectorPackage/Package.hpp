#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <VectorPackage/VectorModule.hpp>

namespace Melon::Test
{
        TARWI_PACKAGE(VectorPackage)
        {
                TARWI_SET_NAME("VectorPackage");

                VectorModule vector;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(vector);

                        TARWI_DISPLAY_RESULTS(
                                vector.successfull_tests,
                                vector.failed_tests,
                                vector.skipped_tests
                        );
                }
        };
} // namespace Melon::Test