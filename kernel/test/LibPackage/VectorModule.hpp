#pragma once

#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/vector.hpp>

using kernel::lib::Vector;

namespace kernel::test::LibPackage {

TARWI_MODULE(VectorModule) {
        TARWI_SET_NAME("VectorModule");

        TARWI_UNIT(unitSimpleAlloc)
        {
                Vector<int> vec;
                
                for (int i = 0; i < 5; i++)
                        vec.push_back(i * 5);

                TARWI_EXPECT(
                        vec[0] == 0 and
                        vec[1] == 5 and
                        vec[2] == 10 and
                        vec[3] == 15 and
                        vec[4] == 20
                );
        }

        TARWI_MODULE_MAIN()
        {
                TARWI_CALL_UNIT(unitSimpleAlloc);
        }
};

} /* namespace kernel::test::LibPackage */