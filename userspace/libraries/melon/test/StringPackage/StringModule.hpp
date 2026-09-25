#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <String.hpp>

namespace Melon::Test
{
        TARWI_MODULE(StringModule)
        {
                TARWI_SET_NAME("StringModule");

                TARWI_UNIT(unitCheckInit)
                {
                        String::String str("Hello, world!");

                        TARWI_EXPECT(strcmp(str.raw(), "Hello, world!") == 0);
                }

                TARWI_UNIT(unitGetByIndex)
                {
                        String::String str("abcde");

                        TARWI_EXPECT(
                                str[0] == 'a' and str[1] == 'b' and str[2] == 'c'
                                and str[3] == 'd' and str[4] == 'e'
                        );
                }

                TARWI_UNIT(unitSetByIndex)
                {
                        String::String str("banana");

                        str[1] = 'o';
                        str[3] = 'o';
                        str[5] = 'o';

                        TARWI_EXPECT(str == "bonono");
                }

                TARWI_UNIT(unitGetLength)
                {
                        String::String s1("melon"); // 5
                        String::String s2("radish"); // 6
                        String::String s3("kiwi"); // 4

                        TARWI_EXPECT(
                                s1.length() == 5 and s2.length() == 6
                                and s3.length() == 4
                        );
                }

                TARWI_UNIT(unitCheckAssign)
                {
                        String::String str = "I am a string!";

                        TARWI_EXPECT(str == "I am a string!");
                }

                TARWI_UNIT(unitDifferentSizes)
                {
                        String::String small_str = "small";
                        String::String very_big_str = "very big";

                        TARWI_EXPECT(
                                small_str < very_big_str and
                                very_big_str > small_str
                        );
                }

                TARWI_UNIT(unitCheckDifferent)
                {
                        String::String s1 = "first";
                        String::String s2 = "second";

                        TARWI_EXPECT(s1 != s2);
                }

                TARWI_UNIT(unitAddition)
                {
                        String::String s1 = "Hello";
                        String::String s2 = "world";
                        String::String s3 = s1 + ", " + s2 + "!";
                        TARWI_EXPECT(s3 == "Hello, world!");
                }

                TARWI_UNIT(unitAssignmentAddition)
                {
                        String::String firstname = "John";
                        firstname += " ";
                        String::String lastname = "Doe";
                        firstname += lastname;
                        TARWI_EXPECT(firstname == "John Doe");
                }

                TARWI_UNIT(unitSubstraction)
                {
                        String::String fullname = "John Doe";
                        String::String firstname = fullname.substract(4);
                        TARWI_EXPECT(firstname == "John");
                }

                TARWI_UNIT(unitTrim)
                {
                        String::String whitespaces = "   abc";
                        String::String o = "bananaooooo";
                        String::String left_right = "     trim_it  ";

                        whitespaces = whitespaces.ltrim();
                        o = o.rtrim('o');
                        left_right = left_right.trim();

                        TARWI_EXPECT(
                                whitespaces == "abc"
                                and o == "banana"
                                and left_right == "trim_it"
                        );
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitCheckInit);
                        TARWI_CALL_UNIT(unitGetByIndex);
                        TARWI_CALL_UNIT(unitSetByIndex);
                        TARWI_CALL_UNIT(unitGetLength);
                        TARWI_CALL_UNIT(unitCheckAssign);
                        TARWI_CALL_UNIT(unitDifferentSizes);
                        TARWI_CALL_UNIT(unitCheckDifferent);
                        TARWI_CALL_UNIT(unitAddition);
                        TARWI_CALL_UNIT(unitAssignmentAddition);
                        TARWI_CALL_UNIT(unitSubstraction);
                        TARWI_CALL_UNIT(unitTrim);
                }
        };
} // namespace Melon::Test