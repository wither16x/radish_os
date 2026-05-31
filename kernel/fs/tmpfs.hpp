// please do not read this file
// I'm ashamed of it
// gonna improve it
// later...

#pragma once

#include <lib/vector.hpp>

namespace kernel::fs::tmpfs {

constexpr int MaxPathChars = 255;
constexpr int MaxDataChars = 1000;

// ahh file struct T-T
struct File {
        char path[MaxPathChars];
        char data[MaxDataChars];
};

void create_file(const char *path, const char *data);
char *read_file(const char *path);

lib::Vector<File> &get_root();

} /* namespace kernel::fs::tmpfs */