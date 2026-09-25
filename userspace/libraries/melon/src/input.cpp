#include <input.hpp>
#include <print.hpp>
#include <internal/filesystem/stdstreams.hpp>

namespace Melon::Input
{
        String::String input(const String::String &str)
        {
                Print::print("{}", str);

                String::String answer = FileSystem::stdIn->readLine(0);
                return answer;
        }
} // namespace Melon::Input