#include <melon/print.hpp>
#include <melon/command_line.hpp>

using namespace Melon;

int main(int argc, char **argv)
{
	CommandLine::Cli interface("echo -- display a message");
	interface.setUsage("echo [arguments]");
	interface.addOption({{"-h", "--help"}}, "display a help message");
	interface.addOption({{"-n", "--no-newline"}}, "do not print a newline at the end");
	interface.setArgs(argc, argv);

	if (not interface.hasArguments()) {
		Print::println("");
		return 0;
	}

	const auto &args = interface.arguments();

	for (const auto &arg : args) {
		if (interface.tryHelp(arg))
			return 0;
	}

	bool print_newline = true;
	bool first = true;

	for (const auto &arg : args) {
		if (arg == "-n" or arg == "--no-newline") {
			print_newline = false;
			continue;
		}

		if (not first)
			Print::print(" ");
		Print::print("{}", arg);
		first = false;
	}

	if (print_newline)
		Print::println("");

	return 0;
}