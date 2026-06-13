#include <fs/path.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

using kernel::lib::String;
using kernel::lib::usize;
using kernel::lib::Vector;

namespace kernel::fs {

Vector<String> parse_path(const String &path)
{
        Vector<String> parts;

        if (path == "/") {
                parts.push_back("/");
                return parts;
        }

        for (usize i = 0; i < path.length(); i++) {
                if (path[i] == '/') {
                        i++;
                        String part_buf;

                        while (i < path.length() && path[i] != '/') {
                                part_buf += path[i];
                                i++;
                        }

                        if (part_buf.length() > 0)
                                parts.push_back(part_buf);

                        i--;
                }
        }

        return parts;
}

} /* namespace kernel::fs */