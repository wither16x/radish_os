#include <test.hpp>
#include <LibPackage/Package.hpp>

namespace kernel::test {

void test_lib()
{
        LibPackage::LibPackage lib_package;
        TARWI_RUN_PACKAGE(lib_package);
}

} /* namespace kernel::test */