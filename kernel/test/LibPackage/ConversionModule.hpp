#pragma once

#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/conversion.hpp>
#include <lib/memory.hpp>

using kernel::lib::itoa;
using kernel::lib::strcmp;

namespace kernel::test::LibPackage {

TARWI_MODULE(ConversionModule)
{
        TARWI_SET_NAME("ConversionModule");

        TARWI_UNIT(unitTestItoa)
        {
                int i = 1234;
                char *s = itoa(i, 10);
                TARWI_EXPECT(strcmp(s, "1234") == 0);
        }

        TARWI_MODULE_MAIN()
        {
                TARWI_CALL_UNIT(unitTestItoa);
        }
};

} /* namespace kernel::test::LibPackage */