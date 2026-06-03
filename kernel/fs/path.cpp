#include <fs/path.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

using kernel::lib::String;
using kernel::lib::usize;
using kernel::lib::Vector;

namespace kernel::fs {

Path::Path(const String &path)
        : path(path)
{}

bool Path::is_valid(this const Path &self)
{
        return self.path[0] == '/';
}

void Path::parse(this Path &self)
{
        if (self.path[0] == '/' && self.path.length() <= 1) {
                self.parts.push_back("/");
                return;
        }
        self.parts.push_back("/");

        for (usize i = 0; i < self.path.length(); i++) {
                if (self.path[i] == '/') {
                        i++;
                        String part_buf;

                        while (i < self.path.length() && self.path[i] != '/') {
                                part_buf += self.path[i];
                                i++;
                        }

                        if (part_buf.length() > 0)
                                self.parts.push_back(part_buf);

                        i--;
                }
        }
}

const String &Path::get(this const Path &self)
{
        return self.path;
}

const Vector<String> &Path::get_parts(this const Path &self)
{
        return self.parts;
}

} /* namespace kernel::fs */