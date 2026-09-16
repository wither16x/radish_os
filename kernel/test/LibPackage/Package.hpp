#pragma once

#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <LibPackage/TypingModule.hpp>
#include <LibPackage/ResultModule.hpp>
#include <LibPackage/MemoryModule.hpp>
#include <LibPackage/BufferModule.hpp>
#include <LibPackage/BytesModule.hpp>
#include <LibPackage/ArrayModule.hpp>
#include <LibPackage/MathModule.hpp>
#include <LibPackage/ConversionModule.hpp>
#include <LibPackage/VectorModule.hpp>
#include <LibPackage/BitmapModule.hpp>
#include <LibPackage/LinkedListModule.hpp>
#include <LibPackage/StringModule.hpp>

namespace Kiwi::Test
{
        TARWI_PACKAGE(LibPackage)
        {
                TARWI_SET_NAME("LibPackage");

                TypingModule            typing_module;
                ResultModule            result_module;
                MemoryModule            memory_module;
                BufferModule            buffer_module;
                BytesModule             bytes_module;
                ArrayModule             array_module;
                MathModule              math_module;
                ConversionModule        conversion_module;
                VectorModule            vector_module;
                BitmapModule            bitmap_module;
                LinkedListModule        linked_list_module;
                StringModule            string_module;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(typing_module);
                        TARWI_RUN_MODULE(result_module);
                        TARWI_RUN_MODULE(memory_module);
                        TARWI_RUN_MODULE(buffer_module);
                        TARWI_RUN_MODULE(bytes_module);
                        TARWI_RUN_MODULE(array_module);
                        TARWI_RUN_MODULE(math_module);
                        TARWI_RUN_MODULE(conversion_module);
                        TARWI_RUN_MODULE(vector_module);
                        TARWI_RUN_MODULE(bitmap_module);
                        TARWI_RUN_MODULE(linked_list_module);
                        TARWI_RUN_MODULE(string_module);

                        TARWI_DISPLAY_RESULTS(
                                typing_module.successfull_tests +
                                result_module.successfull_tests +
                                memory_module.successfull_tests +
                                buffer_module.successfull_tests +
                                bytes_module.successfull_tests +
                                array_module.successfull_tests +
                                math_module.successfull_tests +
                                conversion_module.successfull_tests +
                                vector_module.successfull_tests +
                                bitmap_module.successfull_tests +
                                linked_list_module.successfull_tests +
                                string_module.successfull_tests,

                                typing_module.failed_tests +
                                result_module.failed_tests +
                                memory_module.failed_tests +
                                buffer_module.failed_tests +
                                bytes_module.failed_tests +
                                array_module.failed_tests +
                                math_module.failed_tests +
                                conversion_module.failed_tests +
                                vector_module.failed_tests +
                                bitmap_module.failed_tests +
                                linked_list_module.failed_tests +
                                string_module.failed_tests,

                                typing_module.skipped_tests +
                                result_module.skipped_tests +
                                memory_module.skipped_tests +
                                buffer_module.skipped_tests +
                                bytes_module.skipped_tests +
                                array_module.skipped_tests +
                                math_module.skipped_tests +
                                conversion_module.skipped_tests +
                                vector_module.skipped_tests +
                                bitmap_module.skipped_tests +
                                linked_list_module.skipped_tests +
                                string_module.skipped_tests
                        );
                }
        };
} // namespave Kiwi::Test