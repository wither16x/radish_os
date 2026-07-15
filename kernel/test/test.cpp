#include <test.hpp>
#include <LibPackage/Package.hpp>

namespace kernel::test {

void test_lib()
{
        LibPackage::LibPackage package;
        TARWI_RUN_PACKAGE(package);
}

} /* namespace kernel::test */