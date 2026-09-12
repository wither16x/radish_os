#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/buffer.hpp>
#include <lib/typing.hpp>
#include <lib/memory.hpp>

namespace Kiwi::Test
{
        TARWI_MODULE(BufferModule)
        {
                TARWI_SET_NAME("BufferModule");

                TARWI_UNIT(unitDefaultConstructor)
                {
                        Lib::Buffer<Lib::u8> buf;
                        Lib::Buffer<Lib::u8> buf2(nullptr, 0);

                        TARWI_EXPECT(
                                buf.size() == 0 and buf.begin() == buf.end()
                                and buf2.size() == 0 and buf2.begin() == buf2.end()
                        );
                }

                TARWI_UNIT(unitConstructor)
                {
                        const Lib::u8 data[] = {5, 10, 15, 20};
                        unsigned int expected_size = sizeof(data);

                        Lib::Buffer<Lib::u8> buf(data, sizeof(data));

                        TARWI_EXPECT(
                                buf.size() == expected_size
                                and buf.end() - buf.begin() == expected_size
                                and Lib::memcmp(buf.get(), data, buf.size()) == 0
                        );
                }

                TARWI_UNIT(unitSizeConstructorNonZero)
                {
                        Lib::Buffer<Lib::u8> buf(5);

                        TARWI_EXPECT(buf.size() == 5 and buf.get() != nullptr);
                }

                TARWI_UNIT(unitSizeConstructorZero)
                {
                        Lib::Buffer<Lib::u8> buf(0);

                        TARWI_EXPECT(buf.size() == 0 and buf.get() == nullptr);
                }

                TARWI_UNIT(unitDoesCopy)
                {
                        const Lib::u8 data[] = {5, 10, 15, 20};
                        Lib::Buffer<Lib::u8> buf(data, sizeof(data));

                        buf[2] = 50;

                        TARWI_EXPECT(data[2] == 15 and buf[2] == 50);
                }

                TARWI_UNIT(unitCheckCopy)
                {
                        const Lib::u8 data[] = {2, 4, 6, 8, 10, 12};
                        Lib::Buffer<Lib::u8> original_buf(data, sizeof(data));
                        Lib::Buffer<Lib::u8> copy_buf = original_buf;

                        TARWI_EXPECT(
                                copy_buf.size() == original_buf.size()
                                and copy_buf.begin() != original_buf.begin()
                                and copy_buf.end() != original_buf.end()
                                and copy_buf.get() != original_buf.get()
                        );
                }

                TARWI_UNIT(unitCopyIndependent)
                {
                        const Lib::u8 data[] = {2, 4, 6, 8, 10, 12};
                        Lib::Buffer<Lib::u8> original_buf(data, sizeof(data));
                        Lib::Buffer<Lib::u8> copy_buf = original_buf;

                        copy_buf[3] = 16;

                        TARWI_EXPECT(
                                original_buf[3] == 8
                                and copy_buf[3] == 16
                        );
                }

                TARWI_UNIT(unitCheckMove)
                {
                        const Lib::u8 data[] = {2, 4, 6, 8, 10, 12};
                        Lib::Buffer<Lib::u8> original_buf(data, sizeof(data));
                        Lib::Buffer<Lib::u8> copy_buf = Lib::move(original_buf);

                        TARWI_EXPECT(
                                original_buf.size() == 0 and original_buf.get() == nullptr
                                and copy_buf[0] == data[0] and copy_buf[1] == data[1]
                                and copy_buf[2] == data[2] and copy_buf[3] == data[3]
                                and copy_buf[4] == data[4] and copy_buf[5] == data[5]
                        );
                }

                TARWI_UNIT(unitConstructValidIndex)
                {
                        Lib::Buffer<Lib::u8> buf(3);
                        bool ok = buf.construct(1, 42);

                        TARWI_EXPECT(ok and buf[1] == 42);
                }

                TARWI_UNIT(unitConstructOutOfRange)
                {
                        Lib::Buffer<Lib::u8> buf(3);

                        TARWI_EXPECT(
                                buf.construct(3, 1) == false
                                and buf.construct(100, 1) == false
                        );
                }

                TARWI_UNIT(unitDestroyValidAndInvalidIndex)
                {
                        Lib::Buffer<Lib::u8> buf(3);
                        buf.construct(0, 1);

                        TARWI_EXPECT(
                                buf.destroy(0) == true
                                and buf.destroy(3) == false
                        );
                }

                TARWI_UNIT(unitGetEqualsBegin)
                {
                        const Lib::u8 data[] = {5, 6, 7};
                        const Lib::Buffer<Lib::u8> buf(data, sizeof(data));

                        TARWI_EXPECT(buf.get() == buf.begin());
                }

                TARWI_UNIT(unitBeginEndMutation)
                {
                        Lib::u8 data[] = {0, 0, 0, 0};
                        Lib::Buffer<Lib::u8> buf(data, sizeof(data));

                        Lib::u8 v = 1;
                        for (Lib::u8 *it = buf.begin(); it != buf.end(); ++it)
                                *it = v++;

                        TARWI_EXPECT(
                                buf[0] == 1 and buf[1] == 2
                                and buf[2] == 3 and buf[3] == 4
                        );
                }

                TARWI_UNIT(unitResizeGrow)
                {
                        const Lib::u8 data[] = {1, 2, 3};
                        Lib::Buffer<Lib::u8> buf(data, sizeof(data));

                        buf.resize(6);

                        TARWI_EXPECT(
                                buf.size() == 6
                                and buf[0] == 1 and buf[1] == 2 and buf[2] == 3
                        );
                }

                TARWI_UNIT(unitResizeShrink)
                {
                        const Lib::u8 data[] = {1, 2, 3, 4, 5, 6};
                        Lib::Buffer<Lib::u8> buf(data, sizeof(data));

                        buf.resize(3);

                        TARWI_EXPECT(
                                buf.size() == 3
                                and buf[0] == 1 and buf[1] == 2 and buf[2] == 3
                        );
                }

                TARWI_UNIT(unitResizeToZero)
                {
                        const Lib::u8 data[] = {1, 2, 3};
                        Lib::Buffer<Lib::u8> buf(data, sizeof(data));

                        buf.resize(0);

                        TARWI_EXPECT(buf.size() == 0 and buf.begin() == buf.end());
                }

                TARWI_UNIT(unitOperatorEqualsSameContent)
                {
                        const Lib::u8 data[] = {1, 2, 3};
                        Lib::Buffer<Lib::u8> a(data, sizeof(data));
                        Lib::Buffer<Lib::u8> b(data, sizeof(data));

                        TARWI_EXPECT(a == b);
                }

                TARWI_UNIT(unitOperatorEqualsDifferentSize)
                {
                        const Lib::u8 dataA[] = {1, 2, 3};
                        const Lib::u8 dataB[] = {1, 2, 3, 4};
                        Lib::Buffer<Lib::u8> a(dataA, sizeof(dataA));
                        Lib::Buffer<Lib::u8> b(dataB, sizeof(dataB));

                        TARWI_EXPECT(a != b and not (a == b));
                }

                TARWI_UNIT(unitOperatorLessThanDiffersAtByte)
                {
                        const Lib::u8 dataA[] = {1, 2, 3};
                        const Lib::u8 dataB[] = {1, 5, 3};
                        Lib::Buffer<Lib::u8> a(dataA, sizeof(dataA));
                        Lib::Buffer<Lib::u8> b(dataB, sizeof(dataB));

                        TARWI_EXPECT(a < b and b > a and not (a > b));
                }

                TARWI_UNIT(unitOperatorLessThanShorterPrefix)
                {
                        const Lib::u8 dataA[] = {1, 2, 3};
                        const Lib::u8 dataB[] = {1, 2, 3, 4};
                        Lib::Buffer<Lib::u8> a(dataA, sizeof(dataA));
                        Lib::Buffer<Lib::u8> b(dataB, sizeof(dataB));

                        TARWI_EXPECT(a < b and b > a);
                }

                TARWI_UNIT(unitOperatorLessEqualGreaterEqual)
                {
                        const Lib::u8 data[] = {1, 2, 3};
                        Lib::Buffer<Lib::u8> a(data, sizeof(data));
                        Lib::Buffer<Lib::u8> b(data, sizeof(data));

                        TARWI_EXPECT(a <= b and a >= b);
                }

                TARWI_UNIT(unitOperatorTransitivity)
                {
                        const Lib::u8 va[] = {1};
                        const Lib::u8 vb[] = {2};
                        const Lib::u8 vc[] = {3};
                        Lib::Buffer<Lib::u8> a(va, 1);
                        Lib::Buffer<Lib::u8> b(vb, 1);
                        Lib::Buffer<Lib::u8> c(vc, 1);

                        TARWI_EXPECT(a < b and b < c and a < c);
                }

                TARWI_UNIT(unitIndexReadWrite)
                {
                        Lib::u8 data[] = {1, 2, 3};
                        Lib::Buffer<Lib::u8> buf(data, sizeof(data));

                        buf[1] = 99;

                        TARWI_EXPECT(buf[1] == 99);
                }

                TARWI_UNIT(unitIndexConstRead)
                {
                        const Lib::u8 data[] = {7, 8, 9};
                        const Lib::Buffer<Lib::u8> buf(data, sizeof(data));

                        TARWI_EXPECT(buf[0] == 7 and buf[1] == 8 and buf[2] == 9);
                }

                TARWI_UNIT(unitCopyAssignmentIndependentMemory)
                {
                        const Lib::u8 data[] = {1, 2, 3};
                        Lib::Buffer<Lib::u8> src(data, sizeof(data));
                        Lib::Buffer<Lib::u8> dst;

                        dst = src;

                        TARWI_EXPECT(dst.size() == src.size() and dst.get() != src.get());
                }

                TARWI_UNIT(unitMoveAssignment)
                {
                        const Lib::u8 data[] = {1, 2, 3};
                        Lib::Buffer<Lib::u8> src(data, sizeof(data));
                        Lib::Buffer<Lib::u8> dst;

                        dst = Lib::move(src);

                        TARWI_EXPECT(
                                src.size() == 0 and src.get() == nullptr
                                and dst.size() == 3
                                and dst[0] == 1 and dst[1] == 2 and dst[2] == 3
                        );
                }

                TARWI_UNIT(unitSelfCopyAssignment)
                {
                        Lib::u8 data[] = {1, 2, 3};
                        Lib::Buffer<Lib::u8> buf(data, sizeof(data));

                        buf = buf;

                        TARWI_EXPECT(
                                buf.size() == 3
                                and buf[0] == 1 and buf[1] == 2 and buf[2] == 3
                        );
                }

                TARWI_UNIT(unitSelfMoveAssignment)
                {
                        Lib::u8 data[] = {1, 2, 3};
                        Lib::Buffer<Lib::u8> buf(data, sizeof(data));

                        buf = Lib::move(buf);

                        TARWI_EXPECT(
                                buf.size() == 3
                                and buf[0] == 1 and buf[1] == 2 and buf[2] == 3
                        );
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitDefaultConstructor);
                        TARWI_CALL_UNIT(unitConstructor);
                        TARWI_CALL_UNIT(unitSizeConstructorNonZero);
                        TARWI_CALL_UNIT(unitSizeConstructorZero);
                        TARWI_CALL_UNIT(unitDoesCopy);

                        TARWI_CALL_UNIT(unitCheckCopy);
                        TARWI_CALL_UNIT(unitCopyIndependent);

                        TARWI_CALL_UNIT(unitCheckMove);

                        TARWI_CALL_UNIT(unitConstructValidIndex);
                        TARWI_CALL_UNIT(unitConstructOutOfRange);
                        TARWI_CALL_UNIT(unitDestroyValidAndInvalidIndex);

                        TARWI_CALL_UNIT(unitGetEqualsBegin);
                        TARWI_CALL_UNIT(unitBeginEndMutation);

                        TARWI_CALL_UNIT(unitResizeGrow);
                        TARWI_CALL_UNIT(unitResizeShrink);
                        TARWI_CALL_UNIT(unitResizeToZero);

                        TARWI_CALL_UNIT(unitOperatorEqualsSameContent);
                        TARWI_CALL_UNIT(unitOperatorEqualsDifferentSize);
                        TARWI_CALL_UNIT(unitOperatorLessThanDiffersAtByte);
                        TARWI_CALL_UNIT(unitOperatorLessThanShorterPrefix);
                        TARWI_CALL_UNIT(unitOperatorLessEqualGreaterEqual);
                        TARWI_CALL_UNIT(unitOperatorTransitivity);

                        TARWI_CALL_UNIT(unitIndexReadWrite);
                        TARWI_CALL_UNIT(unitIndexConstRead);

                        TARWI_CALL_UNIT(unitCopyAssignmentIndependentMemory);
                        TARWI_CALL_UNIT(unitMoveAssignment);
                        TARWI_CALL_UNIT(unitSelfCopyAssignment);
                        TARWI_CALL_UNIT(unitSelfMoveAssignment);
                }
        };
} // namespace Kiwi::Test