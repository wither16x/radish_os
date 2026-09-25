#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <Print.hpp>

namespace Melon::Test
{
        TARWI_MODULE(PrintModule)
        {
                TARWI_SET_NAME("PrintModule");

                TARWI_UNIT(unitHelloWorld)
                {
                        Print::println("Hello, world!");

                        TARWI_EXPECT(true);
                }

                TARWI_UNIT(unitIntroduce)
                {
                        String::String firstname = "John";
                        String::String lastname = "Doe";
                        int age = 42;

                        Print::println("Hello! My name is {} {} and I am {} years old!", firstname, lastname, age);
                
                        TARWI_EXPECT(true);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitHelloWorld);
                        TARWI_CALL_UNIT(unitIntroduce);
                }
        };
} // namespace Melon::Test