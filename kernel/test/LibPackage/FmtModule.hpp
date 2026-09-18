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

                TARWI_UNIT(unitFmtChar)
                {
                        char letter5 = 'e';
                        Lib::String s1 = Lib::formatString("The fifth letter in the alphabet is {}.",
                                letter5
                        );

                        char n = '5';
                        Lib::String s2 = Lib::formatString("3 + 2 = {}.", n);

                        TARWI_EXPECT(
                                s1 == "The fifth letter in the alphabet is e."
                                and s2 == "3 + 2 = 5."
                        );
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitFmtString);
                        TARWI_CALL_UNIT(unitFmtChar);
                }
        };
} // namespace Kiwi::Test