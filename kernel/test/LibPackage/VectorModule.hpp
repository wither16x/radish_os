#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/vector.hpp>
#include <lib/typing.hpp>
#include <lib/memory.hpp>

namespace Kiwi::Test
{
TARWI_MODULE(VectorModule)
{
                TARWI_SET_NAME("VectorModule");

                TARWI_UNIT(unitDefaultConstructor)
                {
                        Lib::Vector<Lib::u8> v;

                        TARWI_EXPECT(v.length() == 0 and v.capacity() == 1 and v.isEmpty());
                }

                TARWI_UNIT(unitConstructFromRawArray)
                {
                        const Lib::u8 objects[] = {1, 2, 3, 4};
                        Lib::Vector<Lib::u8> v(objects);

                        TARWI_EXPECT(
                                v.length() == 4 and v.capacity() == 4
                                and v[0] == 1 and v[1] == 2 and v[2] == 3 and v[3] == 4
                        );
                }

                TARWI_UNIT(unitPushBackSingle)
                {
                        Lib::Vector<Lib::u8> v;
                        v.pushBack(42);

                        TARWI_EXPECT(v.length() == 1 and v[0] == 42);
                }

                TARWI_UNIT(unitPushBackGrowthSequence)
                {
                        Lib::Vector<Lib::u8> v;
                        v.pushBack(10);
                        v.pushBack(20);
                        v.pushBack(30);
                        v.pushBack(40);
                        v.pushBack(50);

                        TARWI_EXPECT(
                                v.length() == 5 and v.capacity() == 8
                                and v[0] == 10 and v[1] == 20 and v[2] == 30
                                and v[3] == 40 and v[4] == 50
                        );
                }

                struct Pair
                {
                        int a, b;
                        Pair(int x, int y) : a(x), b(y) {}
                };

                TARWI_UNIT(unitEmplaceBackConstructsInPlace)
                {
                        Lib::Vector<Pair> v;
                        v.emplaceBack(3, 4);

                        TARWI_EXPECT(v.length() == 1 and v[0].a == 3 and v[0].b == 4);
                }

                TARWI_UNIT(unitPopBackReturnsLastAndShrinks)
                {
                        Lib::Vector<Lib::u8> v;
                        v.pushBack(1);
                        v.pushBack(2);
                        v.pushBack(3);

                        auto result = v.popBack();

                        TARWI_EXPECT(result.value() == 3 and v.length() == 2);
                }

                TARWI_UNIT(unitPopBackOnEmptyIsError)
                {
                        Lib::Vector<Lib::u8> v;

                        auto result = v.popBack();

                        TARWI_EXPECT(result.error() == false);
                }

                TARWI_UNIT(unitEraseMiddleShiftsElements)
                {
                        const Lib::u8 objects[] = {1, 2, 3, 4};
                        Lib::Vector<Lib::u8> v(objects);

                        bool ok = v.erase(1);

                        TARWI_EXPECT(
                                ok and v.length() == 3
                                and v[0] == 1 and v[1] == 3 and v[2] == 4
                        );
                }

                TARWI_UNIT(unitEraseLastElement)
                {
                        const Lib::u8 objects[] = {1, 2, 3};
                        Lib::Vector<Lib::u8> v(objects);

                        bool ok = v.erase(2);

                        TARWI_EXPECT(ok and v.length() == 2 and v[0] == 1 and v[1] == 2);
                }

                TARWI_UNIT(unitEraseOutOfRangeReturnsFalse)
                {
                        const Lib::u8 objects[] = {1, 2, 3};
                        Lib::Vector<Lib::u8> v(objects);

                        bool ok = v.erase(3);

                        TARWI_EXPECT(not ok and v.length() == 3);
                }

                TARWI_UNIT(unitEraseOnEmptyReturnsFalse)
                {
                        Lib::Vector<Lib::u8> v;

                        TARWI_EXPECT(v.erase(0) == false);
                }

                TARWI_UNIT(unitClearResetsLengthKeepsCapacity)
                {
                        const Lib::u8 objects[] = {1, 2, 3, 4};
                        Lib::Vector<Lib::u8> v(objects);
                        Lib::usize cap_before = v.capacity();

                        v.clear();

                        TARWI_EXPECT(v.length() == 0 and v.isEmpty() and v.capacity() == cap_before);
                }

                TARWI_UNIT(unitClearThenPushBackWorks)
                {
                        const Lib::u8 objects[] = {1, 2, 3};
                        Lib::Vector<Lib::u8> v(objects);

                        v.clear();
                        v.pushBack(9);

                        TARWI_EXPECT(v.length() == 1 and v[0] == 9);
                }

                TARWI_UNIT(unitDataEqualsBegin)
                {
                        const Lib::u8 objects[] = {1, 2, 3};
                        const Lib::Vector<Lib::u8> v(objects);

                        TARWI_EXPECT(v.data() == v.begin());
                }

                TARWI_UNIT(unitBeginEndSpanMatchesLength)
                {
                        const Lib::u8 objects[] = {1, 2, 3};
                        Lib::Vector<Lib::u8> v(objects);

                        TARWI_EXPECT(v.end() - v.begin() == 3);
                }

                TARWI_UNIT(unitToBufferMatchesLengthNotCapacity)
                {
                        Lib::Vector<Lib::u8> v;
                        v.pushBack(1);
                        v.pushBack(2);
                        v.pushBack(3);

                        Lib::Buffer<Lib::u8> buf = v.toBuffer();

                        TARWI_EXPECT(
                                buf.size() == v.length()
                                and buf[0] == 1 and buf[1] == 2 and buf[2] == 3
                        );
                }

                TARWI_UNIT(unitIsEmptyTrueThenFalse)
                {
                        Lib::Vector<Lib::u8> v;
                        bool empty_before = v.isEmpty();

                        v.pushBack(1);

                        TARWI_EXPECT(empty_before == true and v.isEmpty() == false);
                }

                TARWI_UNIT(unitOperatorEqualsSameContent)
                {
                        const Lib::u8 objects[] = {1, 2, 3};
                        Lib::Vector<Lib::u8> a(objects);
                        Lib::Vector<Lib::u8> b(objects);

                        TARWI_EXPECT(a == b);
                }

                TARWI_UNIT(unitOperatorEqualsDifferentLength)
                {
                        const Lib::u8 objectsA[] = {1, 2, 3};
                        const Lib::u8 objectsB[] = {1, 2};
                        Lib::Vector<Lib::u8> a(objectsA);
                        Lib::Vector<Lib::u8> b(objectsB);

                        TARWI_EXPECT(not (a == b));
                }

                TARWI_UNIT(unitOperatorEqualsDifferentContent)
                {
                        const Lib::u8 objectsA[] = {1, 2, 3};
                        const Lib::u8 objectsB[] = {1, 9, 3};
                        Lib::Vector<Lib::u8> a(objectsA);
                        Lib::Vector<Lib::u8> b(objectsB);

                        TARWI_EXPECT(not (a == b));
                }

                TARWI_UNIT(unitOperatorEqualsSameLengthDifferentCapacity)
                {
                        Lib::Vector<Lib::u8> a;
                        a.pushBack(1);
                        a.pushBack(2);

                        const Lib::u8 objectsB[] = {1, 2};
                        Lib::Vector<Lib::u8> b(objectsB);

                        TARWI_EXPECT(a == b);
                }

                TARWI_UNIT(unitIndexReadWrite)
                {
                        const Lib::u8 objects[] = {1, 2, 3};
                        Lib::Vector<Lib::u8> v(objects);

                        v[1] = 99;

                        TARWI_EXPECT(v[1] == 99);
                }

                TARWI_UNIT(unitCopyConstructorSameContentAndCapacity)
                {
                        const Lib::u8 objects[] = {1, 2, 3};
                        Lib::Vector<Lib::u8> original(objects);
                        Lib::Vector<Lib::u8> copy = original;

                        TARWI_EXPECT(
                                copy.length() == original.length()
                                and copy.capacity() == original.capacity()
                                and copy[0] == 1 and copy[1] == 2 and copy[2] == 3
                        );
                }

                TARWI_UNIT(unitCopyConstructorIndependent)
                {
                        const Lib::u8 objects[] = {1, 2, 3};
                        Lib::Vector<Lib::u8> original(objects);
                        Lib::Vector<Lib::u8> copy = original;

                        copy[0] = 99;
                        copy.pushBack(4);

                        TARWI_EXPECT(
                                original.length() == 3 and original[0] == 1
                                and copy.length() == 4 and copy[0] == 99
                        );
                }

                TARWI_UNIT(unitMoveConstructorTransfersAndEmptiesSource)
                {
                        const Lib::u8 objects[] = {1, 2, 3};
                        Lib::Vector<Lib::u8> original(objects);
                        Lib::Vector<Lib::u8> moved = Lib::move(original);

                        TARWI_EXPECT(
                                original.length() == 0 and original.capacity() == 0
                                and moved.length() == 3
                                and moved[0] == 1 and moved[1] == 2 and moved[2] == 3
                        );
                }

                TARWI_UNIT(unitCopyAssignmentIndependent)
                {
                        const Lib::u8 objectsA[] = {1, 2, 3};
                        const Lib::u8 objectsB[] = {9, 8};
                        Lib::Vector<Lib::u8> a(objectsA);
                        Lib::Vector<Lib::u8> b(objectsB);

                        a = b;
                        b.pushBack(7);

                        TARWI_EXPECT(a.length() == 2 and a[0] == 9 and a[1] == 8 and b.length() == 3);
                }

                TARWI_UNIT(unitMoveAssignment)
                {
                        const Lib::u8 objects[] = {1, 2, 3};
                        Lib::Vector<Lib::u8> a;
                        Lib::Vector<Lib::u8> b(objects);

                        a = Lib::move(b);

                        TARWI_EXPECT(
                                a.length() == 3 and a[0] == 1 and a[1] == 2 and a[2] == 3
                                and b.length() == 0 and b.capacity() == 0
                        );
                }

                TARWI_UNIT(unitSelfCopyAssignment)
                {
                        const Lib::u8 objects[] = {1, 2, 3};
                        Lib::Vector<Lib::u8> v(objects);

                        v = v;

                        TARWI_EXPECT(v.length() == 3 and v[0] == 1 and v[1] == 2 and v[2] == 3);
                }

                TARWI_UNIT(unitSelfMoveAssignment)
                {
                        const Lib::u8 objects[] = {1, 2, 3};
                        Lib::Vector<Lib::u8> v(objects);

                        v = Lib::move(v);

                        TARWI_EXPECT(v.length() == 3 and v[0] == 1 and v[1] == 2 and v[2] == 3);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitDefaultConstructor);
                        TARWI_CALL_UNIT(unitConstructFromRawArray);

                        TARWI_CALL_UNIT(unitPushBackSingle);
                        TARWI_CALL_UNIT(unitPushBackGrowthSequence);

                        TARWI_CALL_UNIT(unitEmplaceBackConstructsInPlace);

                        TARWI_CALL_UNIT(unitPopBackReturnsLastAndShrinks);
                        TARWI_CALL_UNIT(unitPopBackOnEmptyIsError);

                        TARWI_CALL_UNIT(unitEraseMiddleShiftsElements);
                        TARWI_CALL_UNIT(unitEraseLastElement);
                        TARWI_CALL_UNIT(unitEraseOutOfRangeReturnsFalse);
                        TARWI_CALL_UNIT(unitEraseOnEmptyReturnsFalse);

                        TARWI_CALL_UNIT(unitClearResetsLengthKeepsCapacity);
                        TARWI_CALL_UNIT(unitClearThenPushBackWorks);

                        TARWI_CALL_UNIT(unitDataEqualsBegin);
                        TARWI_CALL_UNIT(unitBeginEndSpanMatchesLength);
                        TARWI_CALL_UNIT(unitToBufferMatchesLengthNotCapacity);

                        TARWI_CALL_UNIT(unitIsEmptyTrueThenFalse);

                        TARWI_CALL_UNIT(unitOperatorEqualsSameContent);
                        TARWI_CALL_UNIT(unitOperatorEqualsDifferentLength);
                        TARWI_CALL_UNIT(unitOperatorEqualsDifferentContent);
                        TARWI_CALL_UNIT(unitOperatorEqualsSameLengthDifferentCapacity);

                        TARWI_CALL_UNIT(unitIndexReadWrite);

                        TARWI_CALL_UNIT(unitCopyConstructorSameContentAndCapacity);
                        TARWI_CALL_UNIT(unitCopyConstructorIndependent);

                        TARWI_CALL_UNIT(unitMoveConstructorTransfersAndEmptiesSource);

                        TARWI_CALL_UNIT(unitCopyAssignmentIndependent);
                        TARWI_CALL_UNIT(unitMoveAssignment);
                        TARWI_CALL_UNIT(unitSelfCopyAssignment);
                        TARWI_CALL_UNIT(unitSelfMoveAssignment);
                }
        };
} // namespace Kiwi::Test