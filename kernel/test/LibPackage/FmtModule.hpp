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
                
                        TARWI_EXPECT(s == "Hello, my name is Joe!");
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitFmtString);
                }
        };
} // namespace Kiwi::Test