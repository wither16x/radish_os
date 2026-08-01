#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <test.hpp>
#include <LibPackage/Package.hpp>
#include <FloatPackage/Package.hpp>

namespace kernel::test {

void test_lib()
{
        LibPackage::LibPackage lib_package;
        TARWI_RUN_PACKAGE(lib_package);
}

void test_float()
{
        FloatPackage::FloatPackage float_package;
        TARWI_RUN_PACKAGE(float_package);
}

} /* namespace kernel::test */