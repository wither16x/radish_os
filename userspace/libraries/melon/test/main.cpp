#include <Tarwi.hpp>

#include <MemoryPackage/Package.hpp>
#include <ExceptionsPackage/Package.hpp>
#include <VectorPackage/Package.hpp>
#include <StringPackage/Package.hpp>
#include <FileSystemPackage/Package.hpp>
#include <FmtPackage/Package.hpp>
#include <ConversionPackage/Package.hpp>
#include <PrintPackage/Package.hpp>
#include <ArgparsePackage/Package.hpp>
#include <InputPackage/Package.hpp>

int main()
{
        Melon::Test::MemoryPackage memory_package;
        TARWI_RUN_PACKAGE(memory_package);

        Melon::Test::ExceptionsPackage exceptions_package;
        TARWI_RUN_PACKAGE(exceptions_package);

        Melon::Test::VectorPackage vector_package;
        TARWI_RUN_PACKAGE(vector_package);

        Melon::Test::StringPackage string_package;
        TARWI_RUN_PACKAGE(string_package);

        Melon::Test::FileSystemPackage filesystem_package;
        TARWI_RUN_PACKAGE(filesystem_package);

        Melon::Test::FmtPackage fmt_package;
        TARWI_RUN_PACKAGE(fmt_package);

        Melon::Test::ConversionPackage conversion_package;
        TARWI_RUN_PACKAGE(conversion_package);

        Melon::Test::PrintPackage print_package;
        TARWI_RUN_PACKAGE(print_package);

        Melon::Test::ArgparsePackage argparse_package;
        TARWI_RUN_PACKAGE(argparse_package);

        Melon::Test::InputPackage input_package;
        TARWI_RUN_PACKAGE(input_package);

        return 0;
}