#pragma once

#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <LibPackage/ResultModule.hpp>
#include <LibPackage/MemoryModule.hpp>
#include <LibPackage/BufferModule.hpp>
#include <LibPackage/BytesModule.hpp>
#include <LibPackage/ArrayModule.hpp>
#include <LibPackage/MathModule.hpp>

namespace Kiwi::Test::LibPackage
{
        TARWI_PACKAGE(LibPackage)
        {
                TARWI_SET_NAME("LibPackage");

                ResultModule            result_module;
                MemoryModule            memory_module;
                BufferModule            buffer_module;
                BytesModule             bytes_module;
                ArrayModule             array_module;
                MathModule              math_module;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(result_module);
                        TARWI_RUN_MODULE(memory_module);
                        TARWI_RUN_MODULE(buffer_module);
                        TARWI_RUN_MODULE(bytes_module);
                        TARWI_RUN_MODULE(array_module);
                        TARWI_RUN_MODULE(math_module);

                        TARWI_DISPLAY_RESULTS(
                                result_module.successfull_tests +
                                memory_module.successfull_tests +
                                buffer_module.successfull_tests +
                                bytes_module.successfull_tests +
                                array_module.successfull_tests +
                                math_module.successfull_tests,

                                result_module.failed_tests +
                                memory_module.failed_tests +
                                buffer_module.failed_tests +
                                bytes_module.failed_tests +
                                array_module.failed_tests +
                                math_module.failed_tests,

                                result_module.skipped_tests +
                                memory_module.skipped_tests +
                                buffer_module.skipped_tests +
                                bytes_module.skipped_tests +
                                array_module.skipped_tests +
                                math_module.skipped_tests
                        );
                }
        };
} // namespave Kiwi::Test::LibPackage