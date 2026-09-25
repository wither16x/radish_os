#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <FmtPackage/FormatStringModule.hpp>

namespace Melon::Test
{
        TARWI_PACKAGE(FmtPackage)
        {
                TARWI_SET_NAME("FmtPackage");

                FormatStringModule format_string;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(format_string);

                        TARWI_DISPLAY_RESULTS(
                                format_string.successfull_tests,
                                format_string.failed_tests,
                                format_string.skipped_tests
                        );
                }
        };
} // namespace Melon::Test