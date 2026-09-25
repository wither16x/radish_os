#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <Input.hpp>

namespace Melon::Test
{
        TARWI_MODULE(InputModule)
        {
                TARWI_SET_NAME("InputModule");

                TARWI_UNIT(unitInputString)
                {
                        String::String str = Input::input("Please enter 'apple': ");
                        TARWI_EXPECT(str == "apple");
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitInputString);
                }
        };
} // namespace Melon::Test