#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <test.hpp>
#include <LibPackage/Package.hpp>

namespace Kiwi::Test
{
        void testLib()
        {
                LibPackage lib_package;
                TARWI_RUN_PACKAGE(lib_package);
        }
} // namespace Kiwi::Test