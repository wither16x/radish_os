#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <ConversionPackage/ConversionModule.hpp>

namespace Melon::Test
{
        TARWI_PACKAGE(ConversionPackage)
        {
                TARWI_SET_NAME("ConversionPackage");

                ConversionModule conversion;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(conversion);

                        TARWI_DISPLAY_RESULTS(
                                conversion.successfull_tests,
                                conversion.failed_tests,
                                conversion.skipped_tests
                        );
                }
        };
} // namespace Melon::Test