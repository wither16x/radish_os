#include <Melon/Print.hpp>

using namespace Melon;

int main(int argc, char **argv)
{
        if (argc < 2) {
                Print::println("");
                return 0;
        }

        for (int arg = 1; arg < argc; arg++) {
                Print::print("{}", argv[arg]);

                if (arg < argc - 1)
                        Print::print(" ");
        }

        Print::println("");

        return 0;
}