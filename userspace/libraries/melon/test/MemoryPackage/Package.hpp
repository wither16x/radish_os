#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <MemoryPackage/BufferModule.hpp>
#include <MemoryPackage/CStringModule.hpp>

namespace Melon::Test
{
        TARWI_PACKAGE(MemoryPackage)
        {
                TARWI_SET_NAME("MemoryPackage");

                BufferModule buffer;
                CStringModule cstring;

                TARWI_PACKAGE_MAIN()
                {
                        TARWI_RUN_MODULE(buffer);
                        TARWI_RUN_MODULE(cstring);

                        TARWI_DISPLAY_RESULTS(
                                buffer.successfull_tests + cstring.successfull_tests,
                                buffer.failed_tests + cstring.failed_tests,
                                buffer.skipped_tests + cstring.skipped_tests
                        );
                }
        };
} // namespace Melon::Test 