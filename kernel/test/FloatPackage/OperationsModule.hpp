#pragma once

#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

namespace kernel::test::FloatPackage {

TARWI_MODULE(OperationsModule)
{
        TARWI_SET_NAME("OperationsModule");

        TARWI_UNIT(unitAddition)
        {
                float result = 2.0f + 5.4f;
                TARWI_EXPECT(result == 7.4f);
        }

        TARWI_MODULE_MAIN()
        {
                TARWI_CALL_UNIT(unitAddition);
        }
};

} /* namespace kernel::test::FloatPackage */