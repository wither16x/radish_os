#include <argparse.hpp>
#include <print.hpp>
#include <typing.hpp>

namespace Melon::Argparse
{
        ArgumentParser::ArgumentParser(const String::String &description)
                : description(description)
        {}

        void ArgumentParser::setArguments(this ArgumentParser &self, int argc, char **argv)
        {
                for (int i = 0; i < argc; i++) {
                        if (argv[i])
                                self.raw_arguments.pushBack(argv[i]);
                }
        }

        void ArgumentParser::run(this ArgumentParser &self)
        {
                for (Typing::USize i = 0; i < self.raw_arguments.length(); i++) {
                        if (self.arguments[i].names.contains(self.raw_arguments[i])) {
                                self.arguments[i].callback();
                                return;
                        }
                }

                Print::println("{}", self.description);
        }

        void ArgumentParser::addArgument(this ArgumentParser &self, const Vector::Vector<String::String> &flags, const Typing::Function<void(void)> &callback)
        {
                Argument arg = {
                        .names = flags,
                        .value = "",
                        .callback = callback
                };

                self.arguments.pushBack(arg);
        }

        const Vector::Vector<String::String> &ArgumentParser::getArguments(this const ArgumentParser &self)
        {
                return self.raw_arguments;
        }
} // namespace Melon::Argparse