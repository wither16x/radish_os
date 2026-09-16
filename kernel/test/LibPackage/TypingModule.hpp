#pragma once

#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/typing.hpp>

namespace Kiwi::Test
{
        TARWI_MODULE(TypingModule)
        {
                TARWI_SET_NAME("TypingModule");

                // this test is kinda useless since Lib::is_same_type is compile-time
                TARWI_UNIT(unitCheckSameTypes)
                {
                        using bit8_int_t = Lib::i8;
                        bool result = Lib::is_same_type<bit8_int_t, Lib::i8>;
                        TARWI_EXPECT(result);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitCheckSameTypes);
                }
        };
} // namespace Kiwi::Test