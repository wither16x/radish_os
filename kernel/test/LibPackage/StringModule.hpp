#pragma once

#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/string.hpp>

namespace Kiwi::Test
{
        TARWI_MODULE(StringModule)
        {
                TARWI_SET_NAME("StringModule");

                TARWI_UNIT(unitStaticConstructFromBase)
                {
                        Lib::String<10> s("i am a string");

                        TARWI_EXPECT(Lib::strcmp(s.raw(), "i am a string") == 0);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitStaticConstructFromBase);
                }
        };
} // namespace Kiwi::Test