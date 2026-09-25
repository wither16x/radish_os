#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <Internal/Memory/CString.hpp>

namespace Melon::Test
{
        TARWI_MODULE(CStringModule)
        {
                TARWI_SET_NAME("CStringModule");

                TARWI_UNIT(unitLength)
                {
                        Memory::CString str("0123456789"); // length = 10
                        TARWI_EXPECT(str.length() == 10);
                }

                TARWI_UNIT(unitGetByIndex)
                {
                        Memory::CString str("Hello");
                        TARWI_EXPECT(
                                str[0] == 'H' and str[1] == 'e' and str[2] == 'l'
                                and str[3] == 'l' and str[4] == 'o'
                        );
                }

                TARWI_UNIT(unitOperatorEqual)
                {
                        Memory::CString str1("Foo");
                        Memory::CString str2("Foo");

                        TARWI_EXPECT(str1 == str2);
                }

                TARWI_UNIT(unitOperatorNotEqual)
                {
                        Memory::CString str1("Foo");
                        Memory::CString str2("Bar");

                        TARWI_EXPECT(str1 != str2);
                }

                TARWI_UNIT(unitSmallerBigger)
                {
                        Memory::CString str1("Fish");
                        Memory::CString str2("and chips");

                        TARWI_EXPECT(str1 < str2 and str2 > str1);
                }

                TARWI_UNIT(unitCopy)
                {
                        Memory::CString src("Apple");
                        Memory::CString dest("Banana");

                        src.copy(dest, dest.length());

                        TARWI_EXPECT(dest == src);
                }

                TARWI_UNIT(unitSetByIndex)
                {
                        Memory::CString str("foo");
                        str[0] = 'b';
                        str[1] = 'a';
                        str[2] = 'r';

                        TARWI_EXPECT(str == "bar");
                }

                TARWI_UNIT(unitCheckIfHasChar)
                {
                        Memory::CString str("orange");

                        TARWI_EXPECT(
                                str.has('o') and str.has('a') and str.has('n')
                                and str.has('g') and str.has('e') and not str.has('x')
                        );
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitLength);
                        TARWI_CALL_UNIT(unitGetByIndex);
                        TARWI_CALL_UNIT(unitOperatorEqual);
                        TARWI_CALL_UNIT(unitOperatorNotEqual);
                        TARWI_CALL_UNIT(unitSmallerBigger);
                        TARWI_CALL_UNIT(unitCopy);
                        TARWI_CALL_UNIT(unitSetByIndex);
                        TARWI_CALL_UNIT(unitCheckIfHasChar);
                }
        };
} // namespace Melon::Test