#pragma once

#include <lib/string.hpp>
#include <lib/vector.hpp>

namespace kernel::fs {

class Path {
private:
        const lib::String &path;
        lib::Vector<lib::String> parts;

public:
        Path(const lib::String &path);

        bool is_valid(this const Path &self);
        void parse(this Path &self);

        const lib::String &get(this const Path &self);
        const lib::Vector<lib::String> &get_parts(this const Path &self);
};

} /* namespace kernel::fs */