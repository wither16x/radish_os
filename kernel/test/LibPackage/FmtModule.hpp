#pragma once

#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/string.hpp>
#include <lib/fmt.hpp>

namespace Kiwi::Test
{
        TARWI_MODULE(FmtModule)
        {
                TARWI_SET_NAME("FmtModule");

                TARWI_UNIT(unitFmtString)
                {
                        Lib::String name = "Joe";
                        Lib::String s = Lib::formatString("Hello, my name is {}!", name);
                
                        Lib::String<30> name2 = "Henry";
                        Lib::String<30> s2 = Lib::formatString<30>("And mine is {}!", name2);

                        TARWI_EXPECT(
                                s == "Hello, my name is Joe!"
                                and s2 == "And mine is Henry!"
                        );
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitFmtString);
                }
        };
} // namespace Kiwi::Test