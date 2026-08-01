#pragma once

#include <Tarwi/Tarwi.hpp>
#include <Tarwi/Globals.hpp>

#include <FloatPackage/OperationsModule.hpp>

namespace kernel::test::FloatPackage {

TARWI_PACKAGE(FloatPackage)
{
        TARWI_SET_NAME("FloatPackage");

        OperationsModule operations_module;

        TARWI_PACKAGE_MAIN()
        {
                TARWI_RUN_MODULE(operations_module);

                TARWI_DISPLAY_RESULTS(
                        operations_module.successfull_tests,
                        operations_module.failed_tests,
                        operations_module.skipped_tests
                );
        }
};

} /* namespace kernel::test::FloatPackage */