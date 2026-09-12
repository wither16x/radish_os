#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/result.hpp>
#include <lib/typing.hpp>
#include <lib/memory.hpp>

namespace Kiwi::Test
{
        TARWI_MODULE(ResultModule)
        {
                TARWI_SET_NAME("ResultModule");

                TARWI_UNIT(unitValueConstruction)
                {
                        Lib::Result<int, bool> r(42);

                        TARWI_EXPECT(r.value() == 42);
                }

                TARWI_UNIT(unitErrorConstruction)
                {
                        Lib::Result<int, bool> r = Lib::Error(true);

                        TARWI_EXPECT(r.error() == true);
                }

                TARWI_UNIT(unitCopyConstructorValue)
                {
                        Lib::Result<int, bool> original(7);
                        Lib::Result<int, bool> copy = original;

                        TARWI_EXPECT(copy.value() == 7 and original.value() == 7);
                }

                TARWI_UNIT(unitCopyConstructorError)
                {
                        Lib::Result<int, bool> original = Lib::Error(true);
                        Lib::Result<int, bool> copy = original;

                        TARWI_EXPECT(copy.error() == true and original.error() == true);
                }

                TARWI_UNIT(unitMoveConstructorValue)
                {
                        Lib::Result<int, bool> original(7);
                        Lib::Result<int, bool> moved = Lib::move(original);

                        TARWI_EXPECT(moved.value() == 7);
                }

                TARWI_UNIT(unitMoveConstructorError)
                {
                        Lib::Result<int, bool> original = Lib::Error(true);
                        Lib::Result<int, bool> moved = Lib::move(original);

                        TARWI_EXPECT(moved.error() == true);
                }

                TARWI_UNIT(unitCopyAssignmentValueOverValue)
                {
                        Lib::Result<int, bool> a(1);
                        Lib::Result<int, bool> b(2);

                        a = b;

                        TARWI_EXPECT(a.value() == 2);
                }

                TARWI_UNIT(unitCopyAssignmentErrorOverValue)
                {
                        Lib::Result<int, bool> a(1);
                        Lib::Result<int, bool> b = Lib::Error(true);

                        a = b;

                        TARWI_EXPECT(a.error() == true);
                }

                TARWI_UNIT(unitCopyAssignmentValueOverError)
                {
                        Lib::Result<int, bool> a = Lib::Error(true);
                        Lib::Result<int, bool> b(9);

                        a = b;

                        TARWI_EXPECT(a.value() == 9);
                }

                TARWI_UNIT(unitCopyAssignmentSelf)
                {
                        Lib::Result<int, bool> a(5);

                        a = a;

                        TARWI_EXPECT(a.value() == 5);
                }

                TARWI_UNIT(unitReferenceValueConstruction)
                {
                        int x = 10;
                        Lib::Result<int&, bool> r(x);

                        TARWI_EXPECT(&r.value() == &x and r.value() == 10);
                }

                TARWI_UNIT(unitReferenceValueMutatesOriginal)
                {
                        int x = 10;
                        Lib::Result<int&, bool> r(x);

                        r.value() = 99;

                        TARWI_EXPECT(x == 99);
                }

                TARWI_UNIT(unitReferenceErrorConstruction)
                {
                        Lib::Result<int&, bool> r = Lib::Error(true);

                        TARWI_EXPECT(r.error() == true);
                }

                TARWI_UNIT(unitReferenceCopyConstructor)
                {
                        int x = 3;
                        Lib::Result<int&, bool> original(x);
                        Lib::Result<int&, bool> copy = original;

                        TARWI_EXPECT(&copy.value() == &x);
                }

                TARWI_UNIT(unitReferenceMoveConstructor)
                {
                        int x = 3;
                        Lib::Result<int&, bool> original(x);
                        Lib::Result<int&, bool> moved = Lib::move(original);

                        TARWI_EXPECT(&moved.value() == &x);
                }

                TARWI_UNIT(unitReferenceMoveAssignmentValueOverError)
                {
                        int x = 4;
                        Lib::Result<int&, bool> a = Lib::Error(true);
                        Lib::Result<int&, bool> b(x);

                        a = Lib::move(b);

                        TARWI_EXPECT(&a.value() == &x);
                }

                TARWI_UNIT(unitReferenceMoveAssignmentErrorOverValue)
                {
                        int x = 4;
                        Lib::Result<int&, bool> a(x);
                        Lib::Result<int&, bool> b = Lib::Error(true);

                        a = Lib::move(b);

                        TARWI_EXPECT(a.error() == true);
                }

                TARWI_UNIT(unitReferenceSelfCopyAssignment)
                {
                        int x = 4;
                        Lib::Result<int&, bool> a(x);

                        a = a;

                        TARWI_EXPECT(&a.value() == &x);
                }

                struct LifecycleProbe
                {
                        static inline int constructed = 0;
                        static inline int destructed = 0;

                        int value;

                        LifecycleProbe(int v = 0) : value(v) { ++constructed; }
                        LifecycleProbe(const LifecycleProbe &other) : value(other.value) { ++constructed; }
                        LifecycleProbe(LifecycleProbe &&other) noexcept : value(other.value) { ++constructed; other.value = -1; }
                        ~LifecycleProbe() { ++destructed; }

                        static void reset() { constructed = 0; destructed = 0; }
                };

                TARWI_UNIT(unitLifecycleValueBranchBalanced)
                {
                        LifecycleProbe::reset();

                        {
                                Lib::Result<LifecycleProbe, bool> r(LifecycleProbe(1));
                                (void)r.value();
                        }

                        TARWI_EXPECT(LifecycleProbe::constructed == LifecycleProbe::destructed);
                }

                TARWI_UNIT(unitLifecycleErrorBranchBalanced)
                {
                        LifecycleProbe::reset();

                        {
                                Lib::Result<bool, LifecycleProbe> r = Lib::Error(LifecycleProbe(1));
                                (void)r.error();
                        }

                        TARWI_EXPECT(LifecycleProbe::constructed == LifecycleProbe::destructed);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitValueConstruction);
                        TARWI_CALL_UNIT(unitErrorConstruction);

                        TARWI_CALL_UNIT(unitCopyConstructorValue);
                        TARWI_CALL_UNIT(unitCopyConstructorError);

                        TARWI_CALL_UNIT(unitMoveConstructorValue);
                        TARWI_CALL_UNIT(unitMoveConstructorError);

                        TARWI_CALL_UNIT(unitCopyAssignmentValueOverValue);
                        TARWI_CALL_UNIT(unitCopyAssignmentErrorOverValue);
                        TARWI_CALL_UNIT(unitCopyAssignmentValueOverError);
                        TARWI_CALL_UNIT(unitCopyAssignmentSelf);

                        TARWI_CALL_UNIT(unitReferenceValueConstruction);
                        TARWI_CALL_UNIT(unitReferenceValueMutatesOriginal);
                        TARWI_CALL_UNIT(unitReferenceErrorConstruction);
                        TARWI_CALL_UNIT(unitReferenceCopyConstructor);
                        TARWI_CALL_UNIT(unitReferenceMoveConstructor);
                        TARWI_CALL_UNIT(unitReferenceMoveAssignmentValueOverError);
                        TARWI_CALL_UNIT(unitReferenceMoveAssignmentErrorOverValue);
                        TARWI_CALL_UNIT(unitReferenceSelfCopyAssignment);

                        TARWI_CALL_UNIT(unitLifecycleValueBranchBalanced);
                        TARWI_CALL_UNIT(unitLifecycleErrorBranchBalanced);
                }
        };
} // namespace Kiwi::Test