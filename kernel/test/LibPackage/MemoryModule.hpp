#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/memory.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Test
{
        TARWI_MODULE(MemoryModule)
        {
                TARWI_SET_NAME("MemoryModule");

                TARWI_UNIT(unitMemcpyBasic)
                {
                        const Lib::u8 src[] = {1, 2, 3, 4, 5};
                        Lib::u8 dest[5] = {0};

                        Lib::memcpy(dest, src, sizeof(src));

                        TARWI_EXPECT(Lib::memcmp(dest, src, sizeof(src)) == 0);
                }

                TARWI_UNIT(unitMemcpyZeroLength)
                {
                        Lib::u8 dest[3] = {9, 9, 9};
                        const Lib::u8 src[] = {1, 2, 3};

                        Lib::memcpy(dest, src, 0);

                        TARWI_EXPECT(dest[0] == 9 and dest[1] == 9 and dest[2] == 9);
                }

                TARWI_UNIT(unitMemcpyReturnsDest)
                {
                        Lib::u8 dest[3] = {0};
                        const Lib::u8 src[] = {1, 2, 3};

                        void *ret = Lib::memcpy(dest, src, sizeof(src));

                        TARWI_EXPECT(ret == dest);
                }

                TARWI_UNIT(unitMemsetBasic)
                {
                        Lib::u8 buf[4] = {0};

                        Lib::memset(buf, 7, sizeof(buf));

                        TARWI_EXPECT(buf[0] == 7 and buf[1] == 7 and buf[2] == 7 and buf[3] == 7);
                }

                TARWI_UNIT(unitMemsetZeroLength)
                {
                        Lib::u8 buf[3] = {9, 9, 9};

                        Lib::memset(buf, 0, 0);

                        TARWI_EXPECT(buf[0] == 9 and buf[1] == 9 and buf[2] == 9);
                }

                TARWI_UNIT(unitMemsetTruncatesToByte)
                {
                        Lib::u8 buf[2] = {0};

                        Lib::memset(buf, 0x141, sizeof(buf));

                        TARWI_EXPECT(buf[0] == 0x41 and buf[1] == 0x41);
                }

                TARWI_UNIT(unitMemmoveNonOverlapping)
                {
                        const Lib::u8 src[] = {1, 2, 3, 4};
                        Lib::u8 dest[4] = {0};

                        Lib::memmove(dest, src, sizeof(src));

                        TARWI_EXPECT(Lib::memcmp(dest, src, sizeof(src)) == 0);
                }

                TARWI_UNIT(unitMemmoveOverlapForward)
                {
                        Lib::u8 buf[5] = {1, 2, 3, 4, 0};

                        Lib::memmove(buf + 1, buf, 4);

                        TARWI_EXPECT(
                                buf[0] == 1 and buf[1] == 1 and buf[2] == 2
                                and buf[3] == 3 and buf[4] == 4
                        );
                }

                TARWI_UNIT(unitMemmoveOverlapBackward)
                {
                        Lib::u8 buf[5] = {0, 1, 2, 3, 4};

                        Lib::memmove(buf, buf + 1, 4);

                        TARWI_EXPECT(
                                buf[0] == 1 and buf[1] == 2 and buf[2] == 3 and buf[3] == 4
                        );
                }

                TARWI_UNIT(unitMemmoveZeroLength)
                {
                        Lib::u8 buf[3] = {9, 9, 9};

                        Lib::memmove(buf, buf + 1, 0);

                        TARWI_EXPECT(buf[0] == 9 and buf[1] == 9 and buf[2] == 9);
                }

                TARWI_UNIT(unitMemcmpEqual)
                {
                        const Lib::u8 a[] = {1, 2, 3};
                        const Lib::u8 b[] = {1, 2, 3};

                        TARWI_EXPECT(Lib::memcmp(a, b, sizeof(a)) == 0);
                }

                TARWI_UNIT(unitMemcmpDiffersAtFirstByte)
                {
                        const Lib::u8 a[] = {9, 2, 3};
                        const Lib::u8 b[] = {1, 2, 3};

                        TARWI_EXPECT(Lib::memcmp(a, b, sizeof(a)) > 0);
                }

                TARWI_UNIT(unitMemcmpDiffersAtLastByte)
                {
                        const Lib::u8 a[] = {1, 2, 3};
                        const Lib::u8 b[] = {1, 2, 9};

                        TARWI_EXPECT(Lib::memcmp(a, b, sizeof(a)) < 0);
                }

                TARWI_UNIT(unitMemcmpZeroLength)
                {
                        const Lib::u8 a[] = {1};
                        const Lib::u8 b[] = {2};

                        TARWI_EXPECT(Lib::memcmp(a, b, 0) == 0);
                }

                TARWI_UNIT(unitStrcmpEqual)
                {
                        TARWI_EXPECT(Lib::strcmp("hello", "hello") == 0);
                }

                TARWI_UNIT(unitStrcmpDiffersAtFirstChar)
                {
                        TARWI_EXPECT(Lib::strcmp("zbc", "abc") > 0);
                }

                TARWI_UNIT(unitStrcmpOnePrefixOfOther)
                {
                        TARWI_EXPECT(Lib::strcmp("ab", "abc") < 0);
                }

                TARWI_UNIT(unitStrcmpEmptyStrings)
                {
                        TARWI_EXPECT(Lib::strcmp("", "") == 0);
                }

                TARWI_UNIT(unitStrcmpCaseSensitive)
                {
                        TARWI_EXPECT(Lib::strcmp("abc", "ABC") != 0);
                }

                TARWI_UNIT(unitStrncmpEqualWithinLength)
                {
                        TARWI_EXPECT(Lib::strncmp("abcxyz", "abcdef", 3) == 0);
                }

                TARWI_UNIT(unitStrncmpDiffersAfterLength)
                {
                        TARWI_EXPECT(Lib::strncmp("abc123", "abc456", 3) == 0);
                }

                TARWI_UNIT(unitStrncmpZeroLength)
                {
                        TARWI_EXPECT(Lib::strncmp("abc", "xyz", 0) == 0);
                }

                TARWI_UNIT(unitStrncmpLengthExceedsContent)
                {
                        TARWI_EXPECT(Lib::strncmp("abc", "abd", 10) < 0);
                }

                TARWI_UNIT(unitStrlenBasic)
                {
                        TARWI_EXPECT(Lib::strlen("hello") == 5);
                }

                TARWI_UNIT(unitStrlenEmpty)
                {
                        TARWI_EXPECT(Lib::strlen("") == 0);
                }

                TARWI_UNIT(unitStrcpyBasic)
                {
                        char dest[16] = {0};

                        Lib::strcpy("hello", dest);

                        TARWI_EXPECT(Lib::strcmp(dest, "hello") == 0);
                }

                TARWI_UNIT(unitStrcpyEmptySource)
                {
                        char dest[8] = {'x', 'x', 'x', 0};

                        Lib::strcpy("", dest);

                        TARWI_EXPECT(dest[0] == '\0');
                }

                TARWI_UNIT(unitStrstartswithTrue)
                {
                        TARWI_EXPECT(Lib::strstartswith("hello world", "hello") == true);
                }

                TARWI_UNIT(unitStrstartswithFalse)
                {
                        TARWI_EXPECT(Lib::strstartswith("hello world", "world") == false);
                }

                TARWI_UNIT(unitStrstartswithEmptyPrefix)
                {
                        TARWI_EXPECT(Lib::strstartswith("hello", "") == true);
                }

                TARWI_UNIT(unitStrstartswithPrefixLongerThanString)
                {
                        TARWI_EXPECT(Lib::strstartswith("hi", "hello") == false);
                }

                TARWI_UNIT(unitStrstartswithExactMatch)
                {
                        TARWI_EXPECT(Lib::strstartswith("hello", "hello") == true);
                }

                TARWI_UNIT(unitStrcatBasic)
                {
                        char dest[16] = "foo";

                        Lib::strcat(dest, "bar");

                        TARWI_EXPECT(Lib::strcmp(dest, "foobar") == 0);
                }

                TARWI_UNIT(unitStrcatEmptySource)
                {
                        char dest[16] = "foo";

                        Lib::strcat(dest, "");

                        TARWI_EXPECT(Lib::strcmp(dest, "foo") == 0);
                }

                TARWI_UNIT(unitStrcatOntoEmptyDest)
                {
                        char dest[16] = "";

                        Lib::strcat(dest, "bar");

                        TARWI_EXPECT(Lib::strcmp(dest, "bar") == 0);
                }

                TARWI_UNIT(unitCopyRangeBasic)
                {
                        const int src[] = {1, 2, 3, 4};
                        int dest[4] = {0};

                        int *ret = Lib::copy(src, src + 4, dest);

                        TARWI_EXPECT(
                                dest[0] == 1 and dest[1] == 2 and dest[2] == 3 and dest[3] == 4
                                and ret == dest + 4
                        );
                }

                TARWI_UNIT(unitCopyRangeEmpty)
                {
                        const int src[] = {1, 2, 3};
                        int dest[3] = {9, 9, 9};

                        int *ret = Lib::copy(src, src, dest);

                        TARWI_EXPECT(dest[0] == 9 and dest[1] == 9 and dest[2] == 9 and ret == dest);
                }

                struct MoveProbe
                {
                        bool was_moved_from = false;
                        bool constructed_via_move = false;

                        MoveProbe() = default;

                        MoveProbe(const MoveProbe &)
                                : was_moved_from(false), constructed_via_move(false)
                        {}

                        MoveProbe(MoveProbe &&other) noexcept
                                : was_moved_from(false), constructed_via_move(true)
                        {
                                other.was_moved_from = true;
                        }
                };

                TARWI_UNIT(unitMoveSelectsMoveConstructor)
                {
                        MoveProbe original;
                        MoveProbe moved_to = Lib::move(original);

                        TARWI_EXPECT(
                                moved_to.constructed_via_move == true
                                and original.was_moved_from == true
                        );
                }

                TARWI_UNIT(unitCopyDoesNotTriggerMove)
                {
                        MoveProbe original;
                        MoveProbe copied = original;

                        TARWI_EXPECT(
                                copied.constructed_via_move == false
                                and original.was_moved_from == false
                        );
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitMemcpyBasic);
                        TARWI_CALL_UNIT(unitMemcpyZeroLength);
                        TARWI_CALL_UNIT(unitMemcpyReturnsDest);

                        TARWI_CALL_UNIT(unitMemsetBasic);
                        TARWI_CALL_UNIT(unitMemsetZeroLength);
                        TARWI_CALL_UNIT(unitMemsetTruncatesToByte);

                        TARWI_CALL_UNIT(unitMemmoveNonOverlapping);
                        TARWI_CALL_UNIT(unitMemmoveOverlapForward);
                        TARWI_CALL_UNIT(unitMemmoveOverlapBackward);
                        TARWI_CALL_UNIT(unitMemmoveZeroLength);

                        TARWI_CALL_UNIT(unitMemcmpEqual);
                        TARWI_CALL_UNIT(unitMemcmpDiffersAtFirstByte);
                        TARWI_CALL_UNIT(unitMemcmpDiffersAtLastByte);
                        TARWI_CALL_UNIT(unitMemcmpZeroLength);

                        TARWI_CALL_UNIT(unitStrcmpEqual);
                        TARWI_CALL_UNIT(unitStrcmpDiffersAtFirstChar);
                        TARWI_CALL_UNIT(unitStrcmpOnePrefixOfOther);
                        TARWI_CALL_UNIT(unitStrcmpEmptyStrings);
                        TARWI_CALL_UNIT(unitStrcmpCaseSensitive);

                        TARWI_CALL_UNIT(unitStrncmpEqualWithinLength);
                        TARWI_CALL_UNIT(unitStrncmpDiffersAfterLength);
                        TARWI_CALL_UNIT(unitStrncmpZeroLength);
                        TARWI_CALL_UNIT(unitStrncmpLengthExceedsContent);

                        TARWI_CALL_UNIT(unitStrlenBasic);
                        TARWI_CALL_UNIT(unitStrlenEmpty);

                        TARWI_CALL_UNIT(unitStrcpyBasic);
                        TARWI_CALL_UNIT(unitStrcpyEmptySource);

                        TARWI_CALL_UNIT(unitStrstartswithTrue);
                        TARWI_CALL_UNIT(unitStrstartswithFalse);
                        TARWI_CALL_UNIT(unitStrstartswithEmptyPrefix);
                        TARWI_CALL_UNIT(unitStrstartswithPrefixLongerThanString);
                        TARWI_CALL_UNIT(unitStrstartswithExactMatch);

                        TARWI_CALL_UNIT(unitStrcatBasic);
                        TARWI_CALL_UNIT(unitStrcatEmptySource);
                        TARWI_CALL_UNIT(unitStrcatOntoEmptyDest);

                        TARWI_CALL_UNIT(unitCopyRangeBasic);
                        TARWI_CALL_UNIT(unitCopyRangeEmpty);

                        TARWI_CALL_UNIT(unitMoveSelectsMoveConstructor);
                        TARWI_CALL_UNIT(unitCopyDoesNotTriggerMove);
                }
        };
} // namespace Kiwi::Test