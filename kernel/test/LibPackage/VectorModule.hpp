#pragma once

#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <lib/vector.hpp>

namespace Kiwi::Test::LibPackage
{
        TARWI_MODULE(VectorModule)
        {
                TARWI_SET_NAME("VectorModule");

                TARWI_UNIT(unitSimpleAlloc)
                {
                        Lib::Vector<int> vec;
                        
                        for (int i = 0; i < 5; i++)
                                vec.pushBack(i * 5);

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
} // namespace Kiwi::Test::LibPackage