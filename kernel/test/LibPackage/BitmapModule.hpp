#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/bitmap.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Test
{
        TARWI_MODULE(BitmapModule)
        {
                TARWI_SET_NAME("BitmapModule");

                TARWI_UNIT(unitStaticSetAndTest)
                {
                        Lib::StaticBitmap<64> bmp;
                        bmp.set(5);

                        TARWI_EXPECT(bmp.test(5) == true);
                }

                TARWI_UNIT(unitStaticUnsetBitIsFalseByDefault)
                {
                        Lib::StaticBitmap<64> bmp;

                        TARWI_EXPECT(bmp.test(5) == false);
                }

                TARWI_UNIT(unitStaticClearBit)
                {
                        Lib::StaticBitmap<64> bmp;
                        bmp.set(5);
                        bmp.clear(5);

                        TARWI_EXPECT(bmp.test(5) == false);
                }

                TARWI_UNIT(unitStaticSetDoesNotAffectOtherBits)
                {
                        Lib::StaticBitmap<64> bmp;
                        bmp.set(5);

                        TARWI_EXPECT(
                                bmp.test(4) == false
                                and bmp.test(5) == true
                                and bmp.test(6) == false
                        );
                }

                TARWI_UNIT(unitStaticSetOutOfBoundsIsIgnored)
                {
                        Lib::StaticBitmap<64> bmp;
                        bmp.set(1000);

                        TARWI_EXPECT(bmp.test(1000) == false);
                }

                TARWI_UNIT(unitStaticClearOutOfBoundsDoesNotCrash)
                {
                        Lib::StaticBitmap<64> bmp;
                        bmp.clear(1000);

                        TARWI_EXPECT(true);
                }

                TARWI_UNIT(unitStaticSetAllSetsOnlyWithinRange)
                {
                        Lib::StaticBitmap<10> bmp;
                        bmp.setAll();

                        bool all_set = true;
                        for (Lib::usize i = 0; i < 10; ++i)
                                if (not bmp.test(i))
                                        all_set = false;

                        TARWI_EXPECT(all_set);
                }

                TARWI_UNIT(unitStaticClearAllClearsEverything)
                {
                        Lib::StaticBitmap<10> bmp;
                        bmp.setAll();
                        bmp.clearAll();

                        bool all_clear = true;
                        for (Lib::usize i = 0; i < 10; ++i)
                                if (bmp.test(i))
                                        all_clear = false;

                        TARWI_EXPECT(all_clear);
                }

                TARWI_UNIT(unitStaticSizeReturnsN)
                {
                        Lib::StaticBitmap<42> bmp;

                        TARWI_EXPECT(bmp.size() == 42);
                }

                TARWI_UNIT(unitStaticWordBoundaryLowerWord)
                {
                        Lib::StaticBitmap<100> bmp;
                        bmp.set(63);

                        TARWI_EXPECT(bmp.test(63) == true and bmp.test(64) == false);
                }

                TARWI_UNIT(unitStaticWordBoundaryUpperWord)
                {
                        Lib::StaticBitmap<100> bmp;
                        bmp.set(64);

                        TARWI_EXPECT(bmp.test(64) == true and bmp.test(63) == false);
                }

                TARWI_UNIT(unitStaticNonMultipleOf64Size)
                {
                        Lib::StaticBitmap<100> bmp;
                        bmp.set(99);

                        TARWI_EXPECT(bmp.test(99) == true and bmp.size() == 100);
                }

                TARWI_UNIT(unitStaticSetTwiceIsIdempotent)
                {
                        Lib::StaticBitmap<64> bmp;
                        bmp.set(5);
                        bmp.set(5);

                        TARWI_EXPECT(bmp.test(5) == true);
                }

                TARWI_UNIT(unitStaticClearUnsetBitIsNoOp)
                {
                        Lib::StaticBitmap<64> bmp;
                        bmp.clear(5);

                        TARWI_EXPECT(bmp.test(5) == false);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitStaticSetAndTest);
                        TARWI_CALL_UNIT(unitStaticUnsetBitIsFalseByDefault);
                        TARWI_CALL_UNIT(unitStaticClearBit);
                        TARWI_CALL_UNIT(unitStaticSetDoesNotAffectOtherBits);
                        TARWI_CALL_UNIT(unitStaticSetOutOfBoundsIsIgnored);
                        TARWI_CALL_UNIT(unitStaticClearOutOfBoundsDoesNotCrash);
                        TARWI_CALL_UNIT(unitStaticSetAllSetsOnlyWithinRange);
                        TARWI_CALL_UNIT(unitStaticClearAllClearsEverything);
                        TARWI_CALL_UNIT(unitStaticSizeReturnsN);
                        TARWI_CALL_UNIT(unitStaticWordBoundaryLowerWord);
                        TARWI_CALL_UNIT(unitStaticWordBoundaryUpperWord);
                        TARWI_CALL_UNIT(unitStaticNonMultipleOf64Size);
                        TARWI_CALL_UNIT(unitStaticSetTwiceIsIdempotent);
                        TARWI_CALL_UNIT(unitStaticClearUnsetBitIsNoOp);
                }
        };
} // namespace Kiwi::Test