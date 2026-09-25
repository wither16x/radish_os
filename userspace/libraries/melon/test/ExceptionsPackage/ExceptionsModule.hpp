#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <Exceptions.hpp>

namespace Melon::Test
{
        TARWI_MODULE(ExceptionsModule)
        {
                TARWI_SET_NAME("ExceptionsModule");

                TARWI_UNIT(unitThrow)
                {
                        bool caught = false;

                        try {
                                throw Exceptions::BufferOverflow(10, 5);
                        } catch (const Exceptions::BufferOverflow &e) {
                                TARWI_OUTPUT("caught exception: %s\n", e.what());
                                caught = true;
                        }

                        TARWI_EXPECT(caught);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitThrow);
                }
        };
} // namespace Melon::Test