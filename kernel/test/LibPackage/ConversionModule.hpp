#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/conversion.hpp>
#include <lib/memory.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Test
{
        TARWI_MODULE(ConversionModule)
        {
                TARWI_SET_NAME("ConversionModule");

                TARWI_UNIT(unitItoaPositiveBase10)
                {
                        char buf[32];
                        Lib::itoa(42, buf, 10);

                        TARWI_EXPECT(Lib::strcmp(buf, "42") == 0);
                }

                TARWI_UNIT(unitItoaNegativeBase10)
                {
                        char buf[32];
                        Lib::itoa(-42, buf, 10);

                        TARWI_EXPECT(Lib::strcmp(buf, "-42") == 0);
                }

                TARWI_UNIT(unitItoaZero)
                {
                        char buf[32];
                        Lib::itoa(0, buf, 10);

                        TARWI_EXPECT(Lib::strcmp(buf, "0") == 0);
                }

                TARWI_UNIT(unitItoaBase16)
                {
                        char buf[32];
                        Lib::itoa(255, buf, 16);

                        TARWI_EXPECT(Lib::strcmp(buf, "ff") == 0);
                }

                TARWI_UNIT(unitItoaBase2)
                {
                        char buf[32];
                        Lib::itoa(5, buf, 2);

                        TARWI_EXPECT(Lib::strcmp(buf, "101") == 0);
                }

                TARWI_UNIT(unitItoaReturnsBuffer)
                {
                        char buf[32];
                        char *ret = Lib::itoa(42, buf, 10);

                        TARWI_EXPECT(ret == buf);
                }

                TARWI_UNIT(unitItoaNegativeBase16)
                {
                        char buf[32];
                        Lib::itoa(-255, buf, 16);

                        TARWI_EXPECT(Lib::strcmp(buf, "-ff") == 0);
                }

                TARWI_UNIT(unitUtoaBasic)
                {
                        char buf[32];
                        Lib::utoa(42, buf, 10);

                        TARWI_EXPECT(Lib::strcmp(buf, "42") == 0);
                }

                TARWI_UNIT(unitUtoaZero)
                {
                        char buf[32];
                        Lib::utoa(0, buf, 10);

                        TARWI_EXPECT(Lib::strcmp(buf, "0") == 0);
                }

                TARWI_UNIT(unitUtoaBase16)
                {
                        char buf[32];
                        Lib::utoa(255, buf, 16);

                        TARWI_EXPECT(Lib::strcmp(buf, "ff") == 0);
                }

                TARWI_UNIT(unitUtoaBase2)
                {
                        char buf[32];
                        Lib::utoa(10, buf, 2);

                        TARWI_EXPECT(Lib::strcmp(buf, "1010") == 0);
                }

                TARWI_UNIT(unitAtoiBasic)
                {
                        Lib::usize result = Lib::atoi("42", 10, 2);

                        TARWI_EXPECT(result == 42);
                }

                TARWI_UNIT(unitAtoiBase16)
                {
                        Lib::usize result = Lib::atoi("ff", 16, 2);

                        TARWI_EXPECT(result == 255);
                }

                TARWI_UNIT(unitAtoiRespectsLength)
                {
                        Lib::usize result = Lib::atoi("12345", 10, 3);

                        TARWI_EXPECT(result == 123);
                }

                TARWI_UNIT(unitAtoiZero)
                {
                        Lib::usize result = Lib::atoi("0", 10, 1);

                        TARWI_EXPECT(result == 0);
                }

                TARWI_UNIT(unitCtoiDigitZero)
                {
                        TARWI_EXPECT(Lib::ctoi('0') == 0);
                }

                TARWI_UNIT(unitCtoiDigitNine)
                {
                        TARWI_EXPECT(Lib::ctoi('9') == 9);
                }

                TARWI_UNIT(unitCtoiDigitMiddle)
                {
                        TARWI_EXPECT(Lib::ctoi('5') == 5);
                }

                TARWI_UNIT(unitAtofIntegerValue)
                {
                        double result = Lib::atof("42");

                        TARWI_EXPECT(result == 42.0);
                }

                TARWI_UNIT(unitAtofSimpleDecimal)
                {
                        double result = Lib::atof("2.5");

                        TARWI_EXPECT(result == 2.5);
                }

                TARWI_UNIT(unitAtofNegativeValue)
                {
                        double result = Lib::atof("-3.5");

                        TARWI_EXPECT(result == -3.5);
                }

                TARWI_UNIT(unitAtofZero)
                {
                        double result = Lib::atof("0");

                        TARWI_EXPECT(result == 0.0);
                }

                TARWI_UNIT(unitAtofHalf)
                {
                        double result = Lib::atof("0.5");

                        TARWI_EXPECT(result == 0.5);
                }

                TARWI_UNIT(unitFtoaIntegerValue)
                {
                        char buf[32];
                        Lib::ftoa(42.0, buf);

                        TARWI_EXPECT(Lib::strstartswith(buf, "42"));
                }

                TARWI_UNIT(unitFtoaSimpleDecimal)
                {
                        char buf[32];
                        Lib::ftoa(2.5, buf);

                        TARWI_EXPECT(Lib::strstartswith(buf, "2.5"));
                }

                TARWI_UNIT(unitFtoaNegativeValue)
                {
                        char buf[32];
                        Lib::ftoa(-3.5, buf);

                        TARWI_EXPECT(buf[0] == '-' and Lib::strstartswith(buf + 1, "3.5"));
                }

                TARWI_UNIT(unitFtoaZero)
                {
                        char buf[32];
                        Lib::ftoa(0.0, buf);

                        TARWI_EXPECT(Lib::strstartswith(buf, "0"));
                }

                TARWI_UNIT(unitItoaAtoiRoundTrip)
                {
                        char buf[32];
                        Lib::itoa(12345, buf, 10);
                        Lib::usize result = Lib::atoi(buf, 10, Lib::strlen(buf));

                        TARWI_EXPECT(result == 12345);
                }

                TARWI_UNIT(unitAtofFtoaRoundTrip)
                {
                        char buf[32];
                        Lib::ftoa(2.5, buf);

                        double result = Lib::atof(buf);

                        TARWI_EXPECT(result == 2.5);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitItoaPositiveBase10);
                        TARWI_CALL_UNIT(unitItoaNegativeBase10);
                        TARWI_CALL_UNIT(unitItoaZero);
                        TARWI_CALL_UNIT(unitItoaBase16);
                        TARWI_CALL_UNIT(unitItoaBase2);
                        TARWI_CALL_UNIT(unitItoaReturnsBuffer);
                        TARWI_CALL_UNIT(unitItoaNegativeBase16);

                        TARWI_CALL_UNIT(unitUtoaBasic);
                        TARWI_CALL_UNIT(unitUtoaZero);
                        TARWI_CALL_UNIT(unitUtoaBase16);
                        TARWI_CALL_UNIT(unitUtoaBase2);

                        TARWI_CALL_UNIT(unitAtoiBasic);
                        TARWI_CALL_UNIT(unitAtoiBase16);
                        TARWI_CALL_UNIT(unitAtoiRespectsLength);
                        TARWI_CALL_UNIT(unitAtoiZero);

                        TARWI_CALL_UNIT(unitCtoiDigitZero);
                        TARWI_CALL_UNIT(unitCtoiDigitNine);
                        TARWI_CALL_UNIT(unitCtoiDigitMiddle);

                        TARWI_CALL_UNIT(unitAtofIntegerValue);
                        TARWI_CALL_UNIT(unitAtofSimpleDecimal);
                        TARWI_CALL_UNIT(unitAtofNegativeValue);
                        TARWI_CALL_UNIT(unitAtofZero);
                        TARWI_CALL_UNIT(unitAtofHalf);

                        TARWI_CALL_UNIT(unitFtoaIntegerValue);
                        TARWI_CALL_UNIT(unitFtoaSimpleDecimal);
                        TARWI_CALL_UNIT(unitFtoaNegativeValue);
                        TARWI_CALL_UNIT(unitFtoaZero);

                        TARWI_CALL_UNIT(unitItoaAtoiRoundTrip);
                        TARWI_CALL_UNIT(unitAtofFtoaRoundTrip);
                }
        };
} // namespace Kiwi::Test