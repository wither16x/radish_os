#pragma once

#include "Typing.hpp"
#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <Internal/Memory/Buffer.hpp>
#include <Exceptions.hpp>

namespace Melon::Test
{
        TARWI_MODULE(BufferModule)
        {
                TARWI_SET_NAME("BufferModule");

                TARWI_UNIT(unitCheckInit)
                {
                        char data[] = "abcdefg";
                        Memory::Buffer<char> buf(data, strlen(data));

                        TARWI_EXPECT(strcmp(buf.get(), data) == 0);
                }

                TARWI_UNIT(unitCheckCopy)
                {
                        char a_data[] = "01234";
                        char b_data[] = "abcde";

                        Memory::Buffer<char> a(a_data, strlen(a_data));
                        Memory::Buffer<char> b(b_data, strlen(b_data));

                        a.copy(b, a.size());

                        TARWI_EXPECT(strcmp(a.get(), b.get()) == 0);
                }

                TARWI_UNIT(unitCopyOverflow)
                {
                        char a_data[] = "01234";
                        char b_data[] = "abcde";

                        Memory::Buffer<char> a(a_data, strlen(a_data));
                        Memory::Buffer<char> b(b_data, strlen(b_data));

                        try {
                                a.copy(b, a.size() + 2);
                        } catch (const Exceptions::BufferOverflow &e) {
                                TARWI_OUTPUT("buffer copy failed: %s\n", e.what());
                        }

                        TARWI_EXPECT(strcmp(a.get(), b.get()) != 0);
                }

                TARWI_UNIT(unitCheckSet)
                {
                        Typing::Uint8 data[5];
                        Memory::Buffer<Typing::Uint8> buf(data, sizeof(data));

                        buf.set(12, buf.size());

                        TARWI_EXPECT(
                                buf[0] == 12 and buf[1] == 12 and buf[2] == 12 and
                                buf[3] == 12 and buf[4] == 12
                        );
                }

                TARWI_UNIT(unitCheckMove)
                {
                        char a_data[] = "01234";
                        char b_data[] = "abcde";

                        Memory::Buffer<char> a(a_data, strlen(a_data));
                        Memory::Buffer<char> b(b_data, strlen(b_data));

                        a.move(b, a.size());

                        TARWI_EXPECT(strcmp(a.get(), b.get()) == 0);
                }

                TARWI_UNIT(unitCheckComparison)
                {
                        char small_data[] = "Apple";
                        char enormous_data[] = "Banana bunch";
                        char equal_data0[] = "Equal";
                        char equal_data1[] = "Equal";

                        Typing::USize size1 = strlen(small_data) < strlen(enormous_data) ? strlen(small_data) : strlen(enormous_data);
                        
                        Memory::Buffer<char> small(small_data, strlen(small_data));
                        Memory::Buffer<char> enormous(enormous_data, strlen(enormous_data));
                        Memory::Buffer<char> equal0(equal_data0, strlen(equal_data0));
                        Memory::Buffer<char> equal1(equal_data1, strlen(equal_data1));

                        int is_smaller = small.compare(enormous, size1);
                        int is_bigger = enormous.compare(small, size1);
                        int is_equal = equal0.compare(equal1, equal0.size());

                        TARWI_EXPECT(
                                is_smaller < 0
                                and is_bigger > 0
                                and is_equal == 0
                        );
                }

                TARWI_UNIT(unitOperatorEqual)
                {
                        Typing::Uint8 a_data[] = {25, 42, 12};
                        Typing::Uint8 b_data[] = {25, 42, 12};

                        Memory::Buffer<Typing::Uint8> a(a_data, sizeof(a_data));
                        Memory::Buffer<Typing::Uint8> b(b_data, sizeof(b_data));

                        TARWI_EXPECT(a == b);
                }

                TARWI_UNIT(unitOperatorNotEqual)
                {
                        Typing::Uint8 a_data[] = {25, 42};
                        Typing::Uint8 b_data[] = {25, 42, 12};

                        Memory::Buffer<Typing::Uint8> a(a_data, sizeof(a_data));
                        Memory::Buffer<Typing::Uint8> b(b_data, sizeof(b_data));

                        TARWI_EXPECT(a != b);
                }

                TARWI_UNIT(unitSmallerBigger)
                {
                        Typing::Uint8 a_data[] = {25, 42};
                        Typing::Uint8 b_data[] = {25, 42, 12};

                        Memory::Buffer<Typing::Uint8> a(a_data, sizeof(a_data));
                        Memory::Buffer<Typing::Uint8> b(b_data, sizeof(b_data));

                        TARWI_EXPECT(a < b and b > a);
                }

                TARWI_UNIT(unitGetByIndex)
                {
                        Typing::Uint8 data[] = {10, 20, 30};
                        Memory::Buffer<Typing::Uint8> buf(data, sizeof(data));

                        TARWI_EXPECT(buf[0] == 10 and buf[1] == 20 and buf[2] == 30);
                }

                TARWI_UNIT(unitGetOutOfRange)
                {
                        Typing::Uint8 data[] = {10, 20, 30};
                        Memory::Buffer<Typing::Uint8> buf(data, sizeof(data));
                        bool caught = false;

                        try {
                                Typing::Uint8 elem4 = buf[4];
                                (void)elem4;
                        } catch (const Exceptions::OutOfRange &e) {
                                TARWI_OUTPUT("buffer get failed: %s\n", e.what());
                                caught = true;
                        }

                        TARWI_EXPECT(caught);
                }

                TARWI_UNIT(unitCheckResize)
                {
                        Typing::Uint32 data[] = {150, 250, 550, 780};
                        Memory::Buffer<Typing::Uint32> buf(data, sizeof(data));

                        buf.resize(buf.size() * 2);
                        buf[4] = 950;
                        buf[5] = 1230;
                        buf[6] = 2350;
                        buf[7] = 3225;

                        TARWI_EXPECT(
                                buf[0] == 150 and buf[1] == 250 and buf[2] == 550
                                and buf[3] == 780 and buf[4] == 950 and buf[5] == 1230
                                and buf[6] == 2350 and buf[7] == 3225
                        );
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitCheckInit);
                        TARWI_CALL_UNIT(unitCheckCopy);
                        TARWI_CALL_UNIT(unitCopyOverflow);
                        TARWI_CALL_UNIT(unitCheckSet);
                        TARWI_CALL_UNIT(unitCheckMove);
                        TARWI_CALL_UNIT(unitCheckComparison);
                        TARWI_CALL_UNIT(unitOperatorEqual);
                        TARWI_CALL_UNIT(unitOperatorNotEqual);
                        TARWI_CALL_UNIT(unitSmallerBigger);
                        TARWI_CALL_UNIT(unitGetByIndex);
                        TARWI_CALL_UNIT(unitGetOutOfRange);
                        TARWI_CALL_UNIT(unitCheckResize);
                }
        };
} // namespace Melon::Test