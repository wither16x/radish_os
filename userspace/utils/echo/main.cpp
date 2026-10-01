#include <melon/print.hpp>

using namespace Melon;

int main(int argc, char **argv)
{
        if (argc < 2) {
                Print::println("");
                return 0;
        }

	bool print_newline = true;

        for (int arg = 1; arg < argc; arg++) {
		if (strcmp(argv[arg], "-h") == 0 or strcmp(argv[arg], "--help") == 0) {
			Print::print(
				"echo -- display a message\n\n"
				"Usage: echo [arguments]\n"
				"Options:\n"
				"\t--help, -h: display this message\n"
				"\t--no-newline, -n: do not print a newline at the end"
			);
		} else if (strcmp(argv[arg], "-n") == 0 or strcmp(argv[arg], "--no-newline") == 0) {
			print_newline = false;
		} else {
			Print::print("{}", argv[arg]);

			if (arg < argc - 1)
				Print::print(" ");
		}
        }

	if (print_newline)
        	Print::println("");

        return 0;
}