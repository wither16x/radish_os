#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/math.hpp>

namespace Kiwi::Test
{
        TARWI_MODULE(MathModule)
        {
                TARWI_SET_NAME("MathModule");

                TARWI_UNIT(unitMinFirstSmaller)
                {
                        TARWI_EXPECT(Lib::min(3, 7) == 3);
                }

                TARWI_UNIT(unitMinSecondSmaller)
                {
                        TARWI_EXPECT(Lib::min(7, 3) == 3);
                }

                TARWI_UNIT(unitMinEqualValues)
                {
                        TARWI_EXPECT(Lib::min(5, 5) == 5);
                }

                TARWI_UNIT(unitMinNegativeValues)
                {
                        TARWI_EXPECT(Lib::min(-5, -1) == -5);
                }

                TARWI_UNIT(unitMaxFirstBigger)
                {
                        TARWI_EXPECT(Lib::max(7, 3) == 7);
                }

                TARWI_UNIT(unitMaxSecondBigger)
                {
                        TARWI_EXPECT(Lib::max(3, 7) == 7);
                }

                TARWI_UNIT(unitMaxEqualValues)
                {
                        TARWI_EXPECT(Lib::max(5, 5) == 5);
                }

                TARWI_UNIT(unitMaxNegativeValues)
                {
                        TARWI_EXPECT(Lib::max(-5, -1) == -1);
                }

                TARWI_UNIT(unitClampWithinRange)
                {
                        TARWI_EXPECT(Lib::clamp(5, 0, 10) == 5);
                }

                TARWI_UNIT(unitClampBelowRange)
                {
                        TARWI_EXPECT(Lib::clamp(-3, 0, 10) == 0);
                }

                TARWI_UNIT(unitClampAboveRange)
                {
                        TARWI_EXPECT(Lib::clamp(15, 0, 10) == 10);
                }

                TARWI_UNIT(unitClampAtLowerBoundary)
                {
                        TARWI_EXPECT(Lib::clamp(0, 0, 10) == 0);
                }

                TARWI_UNIT(unitClampAtUpperBoundary)
                {
                        TARWI_EXPECT(Lib::clamp(10, 0, 10) == 10);
                }

                TARWI_UNIT(unitPowExponentZero)
                {
                        TARWI_EXPECT(Lib::pow(5, 0) == 1);
                }

                TARWI_UNIT(unitPowExponentOne)
                {
                        TARWI_EXPECT(Lib::pow(7, 1) == 7);
                }

                TARWI_UNIT(unitPowBasicSquare)
                {
                        TARWI_EXPECT(Lib::pow(3, 2) == 9);
                }

                TARWI_UNIT(unitPowBasicCube)
                {
                        TARWI_EXPECT(Lib::pow(2, 3) == 8);
                }

                TARWI_UNIT(unitPowEvenExponent)
                {
                        TARWI_EXPECT(Lib::pow(2, 4) == 16);
                }

                TARWI_UNIT(unitPowOddExponent)
                {
                        TARWI_EXPECT(Lib::pow(2, 5) == 32);
                }

                TARWI_UNIT(unitPowBaseZeroPositiveExponent)
                {
                        TARWI_EXPECT(Lib::pow(0, 5) == 0);
                }

                TARWI_UNIT(unitPowBaseZeroExponentZero)
                {
                        TARWI_EXPECT(Lib::pow(0, 0) == 1);
                }

                TARWI_UNIT(unitPowBaseOne)
                {
                        TARWI_EXPECT(Lib::pow(1, 100) == 1);
                }

                TARWI_UNIT(unitPowNegativeBaseEvenExponent)
                {
                        TARWI_EXPECT(Lib::pow(-2, 2) == 4);
                }

                TARWI_UNIT(unitPowNegativeBaseOddExponent)
                {
                        TARWI_EXPECT(Lib::pow(-2, 3) == -8);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitMinFirstSmaller);
                        TARWI_CALL_UNIT(unitMinSecondSmaller);
                        TARWI_CALL_UNIT(unitMinEqualValues);
                        TARWI_CALL_UNIT(unitMinNegativeValues);

                        TARWI_CALL_UNIT(unitMaxFirstBigger);
                        TARWI_CALL_UNIT(unitMaxSecondBigger);
                        TARWI_CALL_UNIT(unitMaxEqualValues);
                        TARWI_CALL_UNIT(unitMaxNegativeValues);

                        TARWI_CALL_UNIT(unitClampWithinRange);
                        TARWI_CALL_UNIT(unitClampBelowRange);
                        TARWI_CALL_UNIT(unitClampAboveRange);
                        TARWI_CALL_UNIT(unitClampAtLowerBoundary);
                        TARWI_CALL_UNIT(unitClampAtUpperBoundary);

                        TARWI_CALL_UNIT(unitPowExponentZero);
                        TARWI_CALL_UNIT(unitPowExponentOne);
                        TARWI_CALL_UNIT(unitPowBasicSquare);
                        TARWI_CALL_UNIT(unitPowBasicCube);
                        TARWI_CALL_UNIT(unitPowEvenExponent);
                        TARWI_CALL_UNIT(unitPowOddExponent);
                        TARWI_CALL_UNIT(unitPowBaseZeroPositiveExponent);
                        TARWI_CALL_UNIT(unitPowBaseZeroExponentZero);
                        TARWI_CALL_UNIT(unitPowBaseOne);
                        TARWI_CALL_UNIT(unitPowNegativeBaseEvenExponent);
                        TARWI_CALL_UNIT(unitPowNegativeBaseOddExponent);
                }
        };
} // namespace Kiwi::Test