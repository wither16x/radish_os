#pragma once

#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/conversion.hpp>
#include <lib/memory.hpp>

namespace Kiwi::Test::LibPackage
{
        TARWI_MODULE(ConversionModule)
        {
                TARWI_SET_NAME("ConversionModule");

                TARWI_UNIT(unitTestItoa)
                {
                        char buf[10];
                        int i = 1234;
                        char *s = Lib::itoa(i, buf, 10);
                        TARWI_EXPECT(Lib::strcmp(s, "1234") == 0);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitTestItoa);
                }
        };
} // namespace Kiwi::Test::LibPackage