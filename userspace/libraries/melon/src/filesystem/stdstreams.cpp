#include <filesystem.hpp>

namespace Melon::FileSystem
{
        File *stdOut = new File(stdout);
        File *stdErr = new File(stderr);
        File *stdIn = new File(stdin);
} // namespace Melon::FileSystem