// please do not read this file
// I'm ashamed of it
// gonna improve it
// later...

#include <fs/tmpfs.hpp>
#include <lib/memory.hpp>
#include <lib/vector.hpp>

using kernel::lib::Vector;
using kernel::lib::strcpy, kernel::lib::strcmp;

namespace kernel::fs::tmpfs {

namespace {

Vector<File> root;

} /* anonymous namespace */

void create_file(const char *path, const char *data)
{
        File f;
        strcpy(path, f.path);
        strcpy(data, f.data);
        root.push_back(f);
}

Vector<File> &get_root()
{
        return root;
}

char *read_file(const char *path)
{
        for (File &f : root) {
                if (strcmp(f.path, path) == 0)
                        return f.data;
        }

        return nullptr;
}

} /* namespace kernel::fs::tmpfs */