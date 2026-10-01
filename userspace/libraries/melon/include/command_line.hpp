#pragma once

#include "string.hpp"
#include "vector.hpp"

/// @brief Contains various utilities to build a command line interface.
namespace Melon::CommandLine
{
	struct Option
	{
		Vector::Vector<String::String> names;
		String::String description;
	};

	class Cli
	{
		String::String description;
		String::String usage_msg;
		Vector::Vector<Option> options;
		Vector::Vector<String::String> args;

	public:
		Cli(const String::String &description);

		void setUsage(this Cli &self, const String::String &usage);
		void addOption(this Cli &self, const Vector::Vector<String::String> &names, const String::String &description);
		void setArgs(this Cli &self, int argc, char **argv);
		bool tryHelp(this const Cli &self, const String::String &arg);

		bool hasArguments(this const Cli &self);
		const Vector::Vector<String::String> &arguments(this const Cli &self);
	};
} // namespace Melon::Argparse