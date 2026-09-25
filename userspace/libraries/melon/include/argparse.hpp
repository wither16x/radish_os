#pragma once

#include "vector.hpp"
#include "string.hpp"
#include "typing.hpp"

/// @brief Contains various utilities to parse command line arguments.
namespace Melon::Argparse
{
        struct Argument
        {
                Vector::Vector<String::String> names;
                String::String value;
                Typing::Function<void(void)> callback;
        };

        class ArgumentParser
        {
                Vector::Vector<String::String> raw_arguments;
                Vector::Vector<Argument> arguments;
                String::String description;

        public:
                ArgumentParser() = default;
                ArgumentParser(const String::String &description);

                void setArguments(this ArgumentParser &self, int argc, char **argv);
                void run(this ArgumentParser &self);
                void addArgument(this ArgumentParser &self, const Vector::Vector<String::String> &flags, const Typing::Function<void(void)> &callback);

                const Vector::Vector<String::String> &getArguments(this const ArgumentParser &self);
        };
} // namespace Melon::Argparse