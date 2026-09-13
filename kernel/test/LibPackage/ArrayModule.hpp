#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/array.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Test
{
        TARWI_MODULE(ArrayModule)
        {
                TARWI_SET_NAME("ArrayModule");

                TARWI_UNIT(unitDefaultConstructorNothingConstructed)
                {
                        Lib::Array<Lib::u8, 4> arr;

                        TARWI_EXPECT(arr.destroy(0));
                }

                TARWI_UNIT(unitConstructValidIndex)
                {
                        Lib::Array<Lib::u8, 4> arr;

                        bool ok = arr.construct(1, 42);

                        TARWI_EXPECT(ok and arr[1] == 42);
                }

                TARWI_UNIT(unitConstructOutOfRange)
                {
                        Lib::Array<Lib::u8, 4> arr;

                        TARWI_EXPECT(
                                arr.construct(4, 1) == false
                                and arr.construct(100, 1) == false
                        );
                }

                TARWI_UNIT(unitDestroyValidAndInvalidIndex)
                {
                        Lib::Array<Lib::u8, 4> arr;
                        arr.construct(0, 1);

                        TARWI_EXPECT(
                                arr.destroy(0) == true
                                and arr.destroy(4) == false
                        );
                }

                TARWI_UNIT(unitDestroyThenReconstruct)
                {
                        Lib::Array<Lib::u8, 4> arr;
                        arr.construct(0, 1);
                        arr.destroy(0);

                        bool ok = arr.construct(0, 2);

                        TARWI_EXPECT(ok and arr[0] == 2);
                }

                TARWI_UNIT(unitConstructFromRawArray)
                {
                        const Lib::u8 objects[] = {1, 2, 3, 4};
                        Lib::Array<Lib::u8, 4> arr(objects);

                        TARWI_EXPECT(
                                arr[0] == 1 and arr[1] == 2
                                and arr[2] == 3 and arr[3] == 4
                        );
                }

                TARWI_UNIT(unitClearAllowsReconstruction)
                {
                        const Lib::u8 objects[] = {1, 2, 3, 4};
                        Lib::Array<Lib::u8, 4> arr(objects);

                        arr.clear();
                        bool ok = arr.construct(0, 9);

                        TARWI_EXPECT(ok and arr[0] == 9);
                }

                TARWI_UNIT(unitRawEqualsBegin)
                {
                        const Lib::u8 objects[] = {1, 2, 3, 4};
                        const Lib::Array<Lib::u8, 4> arr(objects);

                        TARWI_EXPECT(arr.raw() == arr.begin());
                }

                TARWI_UNIT(unitBeginEndSpanMatchesLength)
                {
                        const Lib::u8 objects[] = {1, 2, 3, 4};
                        Lib::Array<Lib::u8, 4> arr(objects);

                        TARWI_EXPECT(arr.end() - arr.begin() == 4);
                }

                TARWI_UNIT(unitCopyConstructorSameContent)
                {
                        const Lib::u8 objects[] = {1, 2, 3, 4};
                        Lib::Array<Lib::u8, 4> original(objects);
                        Lib::Array<Lib::u8, 4> copy = original;

                        TARWI_EXPECT(
                                copy[0] == 1 and copy[1] == 2
                                and copy[2] == 3 and copy[3] == 4
                        );
                }

                TARWI_UNIT(unitCopyIndependent)
                {
                        const Lib::u8 objects[] = {1, 2, 3, 4};
                        Lib::Array<Lib::u8, 4> original(objects);
                        Lib::Array<Lib::u8, 4> copy = original;

                        copy[0] = 99;

                        TARWI_EXPECT(original[0] == 1 and copy[0] == 99);
                }

                TARWI_UNIT(unitMoveConstructorTransfersContent)
                {
                        const Lib::u8 objects[] = {1, 2, 3, 4};
                        Lib::Array<Lib::u8, 4> original(objects);
                        Lib::Array<Lib::u8, 4> moved = Lib::move(original);

                        TARWI_EXPECT(
                                moved[0] == 1 and moved[1] == 2
                                and moved[2] == 3 and moved[3] == 4
                        );
                }

                TARWI_UNIT(unitCopyAssignmentIndependent)
                {
                        const Lib::u8 objectsA[] = {1, 2, 3, 4};
                        const Lib::u8 objectsB[] = {5, 6, 7, 8};
                        Lib::Array<Lib::u8, 4> a(objectsA);
                        Lib::Array<Lib::u8, 4> b(objectsB);

                        a = b;
                        b[0] = 99;

                        TARWI_EXPECT(a[0] == 5 and b[0] == 99);
                }

                TARWI_UNIT(unitMoveAssignment)
                {
                        const Lib::u8 objects[] = {1, 2, 3, 4};
                        Lib::Array<Lib::u8, 4> a;
                        Lib::Array<Lib::u8, 4> b(objects);

                        a = Lib::move(b);

                        TARWI_EXPECT(a[0] == 1 and a[1] == 2 and a[2] == 3 and a[3] == 4);
                }

                TARWI_UNIT(unitSelfCopyAssignment)
                {
                        const Lib::u8 objects[] = {1, 2, 3, 4};
                        Lib::Array<Lib::u8, 4> arr(objects);

                        arr = arr;

                        TARWI_EXPECT(arr[0] == 1 and arr[1] == 2 and arr[2] == 3 and arr[3] == 4);
                }

                TARWI_UNIT(unitSelfMoveAssignment)
                {
                        const Lib::u8 objects[] = {1, 2, 3, 4};
                        Lib::Array<Lib::u8, 4> arr(objects);

                        arr = Lib::move(arr);

                        TARWI_EXPECT(arr[0] == 1 and arr[1] == 2 and arr[2] == 3 and arr[3] == 4);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitDefaultConstructorNothingConstructed);

                        TARWI_CALL_UNIT(unitConstructValidIndex);
                        TARWI_CALL_UNIT(unitConstructOutOfRange);
                        TARWI_CALL_UNIT(unitDestroyValidAndInvalidIndex);
                        TARWI_CALL_UNIT(unitDestroyThenReconstruct);

                        TARWI_CALL_UNIT(unitConstructFromRawArray);
                        TARWI_CALL_UNIT(unitClearAllowsReconstruction);

                        TARWI_CALL_UNIT(unitRawEqualsBegin);
                        TARWI_CALL_UNIT(unitBeginEndSpanMatchesLength);

                        TARWI_CALL_UNIT(unitCopyConstructorSameContent);
                        TARWI_CALL_UNIT(unitCopyIndependent);

                        TARWI_CALL_UNIT(unitMoveConstructorTransfersContent);

                        TARWI_CALL_UNIT(unitCopyAssignmentIndependent);
                        TARWI_CALL_UNIT(unitMoveAssignment);
                        TARWI_CALL_UNIT(unitSelfCopyAssignment);
                        TARWI_CALL_UNIT(unitSelfMoveAssignment);
                }
        };
} // namespace Kiwi::Test