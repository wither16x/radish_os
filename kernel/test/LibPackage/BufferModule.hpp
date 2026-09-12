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

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitDefaultConstructor);
                        TARWI_CALL_UNIT(unitConstructor);
                        TARWI_CALL_UNIT(unitDoesCopy);
                        TARWI_CALL_UNIT(unitCheckCopy);
                        TARWI_CALL_UNIT(unitCopyIndependent);
                        TARWI_CALL_UNIT(unitCheckMove);
                }
        };
} // namespace Kiwi::Test