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
                        Lib::String<15> s("i am a string");

                        TARWI_EXPECT(Lib::strcmp(s.raw(), "i am a string") == 0);
                }

                TARWI_UNIT(unitStaticDefaultConstructor)
                {
                        Lib::String<5> s;

                        bool is_zeroed = true;
                        for (auto &c : s) {
                                if (c != 0)
                                        is_zeroed = false;
                        }

                        TARWI_EXPECT(is_zeroed);
                }

                TARWI_UNIT(unitStaticCopyConstructor)
                {
                        Lib::String<15> origin("fish and chips");
                        Lib::String<15> copy(origin);

                        TARWI_EXPECT(copy == origin);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitStaticConstructFromBase);
                        TARWI_CALL_UNIT(unitStaticDefaultConstructor);
                        TARWI_CALL_UNIT(unitStaticCopyConstructor);
                }
        };
} // namespace Kiwi::Test