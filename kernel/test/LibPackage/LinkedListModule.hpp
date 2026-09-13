#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/linked_list.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Test
{
                struct TestNode : Lib::LinkedListHeader
                {
                        int value;
                };

        TARWI_MODULE(LinkedListModule)
        {
                TARWI_SET_NAME("LinkedListModule");

                TARWI_UNIT(unitConstructFromBaseSetsHead)
                {
                        TestNode node{{}, 0};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&node));

                        TARWI_EXPECT(list.first() == &node);
                }

                TARWI_UNIT(unitConstructFromBaseNextPrevAreNull)
                {
                        TestNode node{{}, 0};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&node));

                        TARWI_EXPECT(node.next == nullptr and node.prev == nullptr);
                }

                TARWI_UNIT(unitFirstAndLastEqualOnSingleNode)
                {
                        TestNode node{{}, 0};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&node));

                        TARWI_EXPECT(list.first() == list.last());
                }

                TARWI_UNIT(unitAppendSingleNode)
                {
                        TestNode head{{}, 1};
                        TestNode second{{}, 2};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&head));

                        list.append(&second);

                        TARWI_EXPECT(list.last() == &second);
                }

                TARWI_UNIT(unitAppendUpdatesForwardLink)
                {
                        TestNode head{{}, 1};
                        TestNode second{{}, 2};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&head));

                        list.append(&second);

                        TARWI_EXPECT(head.next == &second);
                }

                TARWI_UNIT(unitAppendUpdatesBackwardLink)
                {
                        TestNode head{{}, 1};
                        TestNode second{{}, 2};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&head));

                        list.append(&second);

                        TARWI_EXPECT(second.prev == &head);
                }

                TARWI_UNIT(unitAppendedNodeNextIsNull)
                {
                        TestNode head{{}, 1};
                        TestNode second{{}, 2};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&head));

                        list.append(&second);

                        TARWI_EXPECT(second.next == nullptr);
                }

                TARWI_UNIT(unitAppendDoesNotChangeFirst)
                {
                        TestNode head{{}, 1};
                        TestNode second{{}, 2};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&head));

                        list.append(&second);

                        TARWI_EXPECT(list.first() == &head);
                }

                TARWI_UNIT(unitAppendMultipleNodesChain)
                {
                        TestNode a{{}, 1};
                        TestNode b{{}, 2};
                        TestNode c{{}, 3};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&a));

                        list.append(&b);
                        list.append(&c);

                        TARWI_EXPECT(
                                list.first() == &a
                                and list.last() == &c
                                and a.next == &b and b.prev == &a
                                and b.next == &c and c.prev == &b
                                and c.next == nullptr
                        );
                }

                TARWI_UNIT(unitTraverseForwardVisitsAllValues)
                {
                        TestNode a{{}, 1};
                        TestNode b{{}, 2};
                        TestNode c{{}, 3};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&a));
                        list.append(&b);
                        list.append(&c);

                        int sum = 0;
                        TestNode *curr = list.first();
                        while (curr) {
                                sum += curr->value;
                                curr = static_cast<TestNode *>(curr->next);
                        }

                        TARWI_EXPECT(sum == 6);
                }

                TARWI_UNIT(unitTraverseBackwardFromLast)
                {
                        TestNode a{{}, 1};
                        TestNode b{{}, 2};
                        TestNode c{{}, 3};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&a));
                        list.append(&b);
                        list.append(&c);

                        int sum = 0;
                        TestNode *curr = list.last();
                        while (curr) {
                                sum += curr->value;
                                curr = static_cast<TestNode *>(curr->prev);
                        }

                        TARWI_EXPECT(sum == 6);
                }

                TARWI_UNIT(unitSetBaseReplacesHead)
                {
                        TestNode a{{}, 1};
                        TestNode b{{}, 2};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&a));

                        list.setBase(reinterpret_cast<Lib::uptr>(&b));

                        TARWI_EXPECT(list.first() == &b);
                }

                TARWI_UNIT(unitSetBaseResetsNextPrev)
                {
                        TestNode a{{}, 1};
                        TestNode b{{}, 2};
                        TestNode c{{}, 3};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&a));
                        list.append(&b);

                        list.setBase(reinterpret_cast<Lib::uptr>(&c));

                        TARWI_EXPECT(c.next == nullptr and c.prev == nullptr);
                }

                TARWI_UNIT(unitResetClearsFirst)
                {
                        TestNode a{{}, 1};
                        Lib::LinkedList<TestNode> list(reinterpret_cast<Lib::uptr>(&a));

                        list.reset();

                        TARWI_EXPECT(list.first() == nullptr);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitConstructFromBaseSetsHead);
                        TARWI_CALL_UNIT(unitConstructFromBaseNextPrevAreNull);

                        TARWI_CALL_UNIT(unitFirstAndLastEqualOnSingleNode);

                        TARWI_CALL_UNIT(unitAppendSingleNode);
                        TARWI_CALL_UNIT(unitAppendUpdatesForwardLink);
                        TARWI_CALL_UNIT(unitAppendUpdatesBackwardLink);
                        TARWI_CALL_UNIT(unitAppendedNodeNextIsNull);
                        TARWI_CALL_UNIT(unitAppendDoesNotChangeFirst);
                        TARWI_CALL_UNIT(unitAppendMultipleNodesChain);
                        TARWI_CALL_UNIT(unitTraverseForwardVisitsAllValues);
                        TARWI_CALL_UNIT(unitTraverseBackwardFromLast);

                        TARWI_CALL_UNIT(unitSetBaseReplacesHead);
                        TARWI_CALL_UNIT(unitSetBaseResetsNextPrev);

                        TARWI_CALL_UNIT(unitResetClearsFirst);
                }
        };
} // namespace Kiwi::Test