#include <fs/path.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

namespace Kiwi::Fs
{
        Lib::Vector<Lib::String> parsePath(const Lib::String &path)
        {
                Lib::Vector<Lib::String> parts;

                if (path == "/") {
                        parts.pushBack("/");
                        return parts;
                }

                for (Lib::usize i = 0; i < path.length(); i++) {
                        if (path[i] == '/') {
                                ++i;
                                Lib::String part_buf;

                                while (i < path.length() and path[i] != '/') {
                                        part_buf += path[i];
                                        ++i;
                                }

                                if (part_buf.length() > 0)
                                        parts.pushBack(part_buf);

                                --i;
                        }
                }

                return parts;
        }
} // namespace Kiwi::Fs