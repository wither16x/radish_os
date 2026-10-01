#include <command_line.hpp>
#include <print.hpp>

namespace Melon::CommandLine
{
	Cli::Cli(const String::String &description)
		: description(description)
	{}

	void Cli::setUsage(this Cli &self, const String::String &usage)
	{
		self.usage_msg = usage;
	}

	void Cli::addOption(this Cli &self, const Vector::Vector<String::String> &names, const String::String &description)
	{
		Option opt;
		opt.names = names;
		opt.description = description;
		self.options.pushBack(opt);
	}

	void Cli::setArgs(this Cli &self, int argc, char **argv)
	{
		for (int i = 1; i < argc; i++) {
			if (argv[i])
				self.args.pushBack(argv[i]);
		}
	}

	bool Cli::tryHelp(this const Cli &self, const String::String &arg)
	{
		if (arg != "-h" and arg != "--help")
			return false;

		Print::println("{}", self.description);
		Print::println("Usage: {}", self.usage_msg);
		Print::println("Options:");
		for (auto &opt : self.options) {
			String::String joined;
			for (auto &name : opt.names) {
				if (joined != "")
					joined += ", ";
				joined += name;
			}
			Print::println("\t{}: {}", joined, opt.description);
		}
		return true;
	}

	bool Cli::hasArguments(this const Cli &self)
	{
		return not self.args.isEmpty();
	}

	const Vector::Vector<String::String> &Cli::arguments(this const Cli &self)
	{
		return self.args;
	}
} // namespace Melon::CommandLine