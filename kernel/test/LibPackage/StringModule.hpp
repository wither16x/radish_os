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

                TARWI_UNIT(unitStaticMoveConstructor)
                {
                        Lib::String<15> origin("to move");
                        Lib::String<15> copy(Lib::move(origin));

                        TARWI_EXPECT(copy == "to move");
                }

                TARWI_UNIT(unitStaticSubString)
                {
                        Lib::String<20> base("Apple pie");
                        Lib::String<20> derived = base.subString(6);

                        TARWI_EXPECT(derived == "pie");
                }

                TARWI_UNIT(unitStaticAddition)
                {
                        Lib::String<20> firstname = "John";
                        Lib::String<20> lastname = "Doe";
                        Lib::String<20> fullname = firstname + " " + lastname;
                
                        TARWI_EXPECT(fullname == "John Doe");
                }

                TARWI_UNIT(unitStaticCopyAssignment)
                {
                        Lib::String<10> s = "Hello";
                        Lib::String<10> s2 = s;

                        TARWI_EXPECT(s2 == s);
                }

                TARWI_UNIT(unitStaticMoveAssignment)
                {
                        Lib::String<10> s = "Hello";
                        Lib::String<10> s2 = Lib::move(s);

                        TARWI_EXPECT(s2 == s); 
                }

                TARWI_UNIT(unitDynamicDefaultConstructor)
                {
                        Lib::String s;
                        
                        TARWI_EXPECT(s.isEmpty());
                }

                TARWI_UNIT(unitDynamicConstruct)
                {
                        Lib::String s("I am a string.");

                        TARWI_EXPECT(s == "I am a string.");
                }

                TARWI_UNIT(unitDynamicCopyConstructor)
                {
                        Lib::String origin("strawberries");
                        Lib::String copy(origin);

                        TARWI_EXPECT(copy == origin);
                }

                TARWI_UNIT(unitDynamicMoveConstructor)
                {
                        Lib::String origin("chicken");
                        Lib::String copy(Lib::move(origin));

                        TARWI_EXPECT(copy == "chicken" and origin.isEmpty());
                }

                TARWI_UNIT(unitDynamicSubString)
                {
                        Lib::String base = "Hello, world!";
                        Lib::String derived = base.subString(7);

                        TARWI_EXPECT(derived == "world!");
                }

                TARWI_UNIT(unitDynamicAppendChar)
                {
                        Lib::String s = "abc";
                        s.appendChar('d');

                        TARWI_EXPECT(s == "abcd");
                }

                TARWI_UNIT(unitDynamicIndex)
                {
                        Lib::String s = "strings are amazing";
                        s[9] = 'd';

                        TARWI_EXPECT(s[0] == 's' and s[8] == 'a' and s[9] == 'd' and s[12] == 'a');
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitStaticConstructFromBase);
                        TARWI_CALL_UNIT(unitStaticDefaultConstructor);
                        TARWI_CALL_UNIT(unitStaticCopyConstructor);
                        TARWI_CALL_UNIT(unitStaticMoveConstructor);
                        TARWI_CALL_UNIT(unitStaticSubString);
                        TARWI_CALL_UNIT(unitStaticAddition);
                        TARWI_CALL_UNIT(unitStaticCopyAssignment);
                        TARWI_CALL_UNIT(unitStaticMoveAssignment);

                        TARWI_CALL_UNIT(unitDynamicDefaultConstructor);
                        TARWI_CALL_UNIT(unitDynamicConstruct);
                        TARWI_CALL_UNIT(unitDynamicCopyConstructor);
                        TARWI_CALL_UNIT(unitDynamicMoveConstructor);
                        TARWI_CALL_UNIT(unitDynamicSubString);
                        TARWI_CALL_UNIT(unitDynamicAppendChar);
                        TARWI_CALL_UNIT(unitDynamicIndex);
                }
        };
} // namespace Kiwi::Test