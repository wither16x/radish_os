#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <FileSystemPackage/FileModule.hpp>

namespace Melon::Test
{
        TARWI_PACKAGE(FileSystemPackage)
        {
                TARWI_SET_NAME("FileSystemPackage");

                FileModule file;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(file);

                        TARWI_DISPLAY_RESULTS(
                                file.successfull_tests,
                                file.failed_tests,
                                file.skipped_tests
                        );
                }
        };
} // namespace Melon::Test