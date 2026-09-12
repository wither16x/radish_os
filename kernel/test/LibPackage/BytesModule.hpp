#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/bytes.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Test
{
        TARWI_MODULE(BytesModule)
        {
                TARWI_SET_NAME("BytesModule");

                TARWI_UNIT(unitAlignUpAlreadyAligned)
                {
                        TARWI_EXPECT(Lib::alignUp(16, 16) == 16);
                }

                TARWI_UNIT(unitAlignUpRoundsUp)
                {
                        TARWI_EXPECT(Lib::alignUp(17, 16) == 32);
                }

                TARWI_UNIT(unitAlignUpJustBelowBoundary)
                {
                        TARWI_EXPECT(Lib::alignUp(15, 16) == 16);
                }

                TARWI_UNIT(unitAlignUpZeroValue)
                {
                        TARWI_EXPECT(Lib::alignUp(0, 16) == 0);
                }

                TARWI_UNIT(unitAlignUpAlignmentOfOne)
                {
                        TARWI_EXPECT(Lib::alignUp(42, 1) == 42);
                }

                TARWI_UNIT(unitAlignUpPageSize)
                {
                        TARWI_EXPECT(
                                Lib::alignUp(4096, 4096) == 4096
                                and Lib::alignUp(4097, 4096) == 8192
                                and Lib::alignUp(1, 4096) == 4096
                        );
                }

                TARWI_UNIT(unitAlignUpLargePowerOfTwo)
                {
                        TARWI_EXPECT(Lib::alignUp(1000000, 65536) == 1048576);
                }

                TARWI_UNIT(unitAlignUpNonPowerOfTwoIsIncorrect)
                {
                        Lib::usize result = Lib::alignUp(15, 10);

                        TARWI_EXPECT(result != 16);
                }

                TARWI_UNIT(unitAlignUpOverflowWraps)
                {
                        Lib::usize near_max = static_cast<Lib::usize>(-1) - 2;
                        Lib::usize result = Lib::alignUp(near_max, 16);

                        TARWI_EXPECT(result < near_max);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitAlignUpAlreadyAligned);
                        TARWI_CALL_UNIT(unitAlignUpRoundsUp);
                        TARWI_CALL_UNIT(unitAlignUpJustBelowBoundary);
                        TARWI_CALL_UNIT(unitAlignUpZeroValue);
                        TARWI_CALL_UNIT(unitAlignUpAlignmentOfOne);
                        TARWI_CALL_UNIT(unitAlignUpPageSize);
                        TARWI_CALL_UNIT(unitAlignUpLargePowerOfTwo);
                        TARWI_CALL_UNIT(unitAlignUpNonPowerOfTwoIsIncorrect);
                        TARWI_CALL_UNIT(unitAlignUpOverflowWraps);
                }
        };
} // namespace Kiwi::Test