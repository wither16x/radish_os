#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/conversion.hpp>
#include <lib/memory.hpp>
#include <lib/typing.hpp>
#include <lib/string.hpp>

namespace Kiwi::Test
{
        TARWI_MODULE(ConversionModule)
        {
                TARWI_SET_NAME("ConversionModule");

                TARWI_UNIT(unitIntToStringPositiveBase10)
                {
                        Lib::String s = Lib::intToString(42, Lib::Base::Decimal);

                        TARWI_EXPECT(s == "42");
                }

                TARWI_UNIT(unitIntToStringNegativeBase10)
                {
                        Lib::String s = Lib::intToString(-42, Lib::Base::Decimal);

                        TARWI_EXPECT(s == "-42");
                }

                TARWI_UNIT(unitIntToStringZero)
                {
                        Lib::String s = Lib::intToString(0, Lib::Base::Decimal);

                        TARWI_EXPECT(s == "0");
                }

                TARWI_UNIT(unitIntToStringBase16)
                {
                        Lib::String s = Lib::intToString(255, Lib::Base::Hexadecimal);

                        TARWI_EXPECT(s == "ff");
                }

                TARWI_UNIT(unitIntToStringBase2)
                {
                        Lib::String s = Lib::intToString(5, Lib::Base::Binary);

                        TARWI_EXPECT(s == "101");
                }

                TARWI_UNIT(unitIntToStringStaticBase2)
                {
                        Lib::String<32> s = Lib::intToString<int, 32>(5, Lib::Base::Binary);

                        TARWI_EXPECT(s == "101");
                }

                TARWI_UNIT(unitIntToStringNegativeBase16)
                {
                        Lib::String s = Lib::intToString(-255, Lib::Base::Hexadecimal);

                        TARWI_EXPECT(s == "-ff");
                }

                TARWI_UNIT(unitUintToStringBasic)
                {
                        Lib::String s = Lib::uintToString(42u, Lib::Base::Decimal);

                        TARWI_EXPECT(s == "42");
                }

                TARWI_UNIT(unitUintToStringZero)
                {
                        Lib::String s = Lib::uintToString(0u, Lib::Base::Decimal);

                        TARWI_EXPECT(s == "0");
                }

                TARWI_UNIT(unitUintToStringBase16)
                {
                        Lib::String s = Lib::uintToString(255u, Lib::Base::Hexadecimal);

                        TARWI_EXPECT(s == "ff");
                }

                TARWI_UNIT(unitUintToStringBase2)
                {
                        Lib::String s = Lib::uintToString(10u, Lib::Base::Binary);

                        TARWI_EXPECT(s == "1010");
                }

                TARWI_UNIT(unitStringToIntBasic)
                {
                        Lib::usize result = Lib::stringToInt<Lib::usize>("42", Lib::Base::Decimal);

                        TARWI_EXPECT(result == 42);
                }

                TARWI_UNIT(unitStringToIntBase16)
                {
                        Lib::usize result = Lib::stringToInt<Lib::usize>("ff", Lib::Base::Hexadecimal);

                        TARWI_EXPECT(result == 255);
                }

                TARWI_UNIT(unitStringToIntStopsAtSpace)
                {
                        Lib::usize result = Lib::stringToInt<Lib::usize>("123 45", Lib::Base::Decimal);

                        TARWI_EXPECT(result == 123);
                }

                TARWI_UNIT(unitStringToIntZero)
                {
                        Lib::usize result = Lib::stringToInt<Lib::usize>("0", Lib::Base::Decimal);

                        TARWI_EXPECT(result == 0);
                }

                TARWI_UNIT(unitCharToIntDigitZero)
                {
                        TARWI_EXPECT(Lib::charToInt<Lib::usize>('0') == 0);
                }

                TARWI_UNIT(unitCharToIntDigitNine)
                {
                        TARWI_EXPECT(Lib::charToInt<Lib::usize>('9') == 9);
                }

                TARWI_UNIT(unitCharToIntDigitMiddle)
                {
                        TARWI_EXPECT(Lib::charToInt<Lib::usize>('5') == 5);
                }

                // TARWI_UNIT(unitAtofIntegerValue)
                // {
                //         double result = Lib::atof("42");

                //         TARWI_EXPECT(result == 42.0);
                // }

                // TARWI_UNIT(unitAtofSimpleDecimal)
                // {
                //         double result = Lib::atof("2.5");

                //         TARWI_EXPECT(result == 2.5);
                // }

                // TARWI_UNIT(unitAtofNegativeValue)
                // {
                //         double result = Lib::atof("-3.5");

                //         TARWI_EXPECT(result == -3.5);
                // }

                // TARWI_UNIT(unitAtofZero)
                // {
                //         double result = Lib::atof("0");

                //         TARWI_EXPECT(result == 0.0);
                // }

                // TARWI_UNIT(unitAtofHalf)
                // {
                //         double result = Lib::atof("0.5");

                //         TARWI_EXPECT(result == 0.5);
                // }

                // TARWI_UNIT(unitFtoaIntegerValue)
                // {
                //         char buf[32];
                //         Lib::ftoa(42.0, buf);

                //         TARWI_EXPECT(Lib::strstartswith(buf, "42"));
                // }

                // TARWI_UNIT(unitFtoaSimpleDecimal)
                // {
                //         char buf[32];
                //         Lib::ftoa(2.5, buf);

                //         TARWI_EXPECT(Lib::strstartswith(buf, "2.5"));
                // }

                // TARWI_UNIT(unitFtoaNegativeValue)
                // {
                //         char buf[32];
                //         Lib::ftoa(-3.5, buf);

                //         TARWI_EXPECT(buf[0] == '-' and Lib::strstartswith(buf + 1, "3.5"));
                // }

                // TARWI_UNIT(unitFtoaZero)
                // {
                //         char buf[32];
                //         Lib::ftoa(0.0, buf);

                //         TARWI_EXPECT(Lib::strstartswith(buf, "0"));
                // }

                TARWI_UNIT(unitIntToStringStringToIntRoundTrip)
                {
                        Lib::String s = Lib::intToString(12345, Lib::Base::Decimal);
                        int result = Lib::stringToInt<int>(s, Lib::Base::Decimal);

                        TARWI_EXPECT(result == 12345);
                }

                // TARWI_UNIT(unitAtofFtoaRoundTrip)
                // {
                //         char buf[32];
                //         Lib::ftoa(2.5, buf);

                //         double result = Lib::atof(buf);

                //         TARWI_EXPECT(result == 2.5);
                // }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitIntToStringPositiveBase10);
                        TARWI_CALL_UNIT(unitIntToStringNegativeBase10);
                        TARWI_CALL_UNIT(unitIntToStringZero);
                        TARWI_CALL_UNIT(unitIntToStringBase16);
                        TARWI_CALL_UNIT(unitIntToStringBase2);
                        TARWI_CALL_UNIT(unitIntToStringStaticBase2);
                        TARWI_CALL_UNIT(unitIntToStringNegativeBase16);

                        TARWI_CALL_UNIT(unitUintToStringBasic);
                        TARWI_CALL_UNIT(unitUintToStringZero);
                        TARWI_CALL_UNIT(unitUintToStringBase16);
                        TARWI_CALL_UNIT(unitUintToStringBase2);

                        TARWI_CALL_UNIT(unitStringToIntBasic);
                        TARWI_CALL_UNIT(unitStringToIntBase16);
                        TARWI_CALL_UNIT(unitStringToIntStopsAtSpace);
                        TARWI_CALL_UNIT(unitStringToIntZero);

                        TARWI_CALL_UNIT(unitCharToIntDigitZero);
                        TARWI_CALL_UNIT(unitCharToIntDigitNine);
                        TARWI_CALL_UNIT(unitCharToIntDigitMiddle);

                        // TARWI_CALL_UNIT(unitAtofIntegerValue);
                        // TARWI_CALL_UNIT(unitAtofSimpleDecimal);
                        // TARWI_CALL_UNIT(unitAtofNegativeValue);
                        // TARWI_CALL_UNIT(unitAtofZero);
                        // TARWI_CALL_UNIT(unitAtofHalf);

                        // TARWI_CALL_UNIT(unitFtoaIntegerValue);
                        // TARWI_CALL_UNIT(unitFtoaSimpleDecimal);
                        // TARWI_CALL_UNIT(unitFtoaNegativeValue);
                        // TARWI_CALL_UNIT(unitFtoaZero);

                        TARWI_CALL_UNIT(unitIntToStringStringToIntRoundTrip);
                        // TARWI_CALL_UNIT(unitAtofFtoaRoundTrip);
                }
        };
} // namespace Kiwi::Test