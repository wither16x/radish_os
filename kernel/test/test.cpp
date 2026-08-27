#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <test.hpp>
#include <LibPackage/Package.hpp>
#include <FloatPackage/Package.hpp>

namespace Kiwi::Test
{
        void testLib()
        {
                LibPackage::LibPackage lib_package;
                TARWI_RUN_PACKAGE(lib_package);
        }

        void testFloat()
        {
                FloatPackage::FloatPackage float_package;
                TARWI_RUN_PACKAGE(float_package);
        }
} // namespace Kiwi::Test