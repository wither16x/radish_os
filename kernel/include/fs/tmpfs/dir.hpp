#pragma once

#include <lib/vector.hpp>
#include <fs/tmpfs/decls.hpp>

namespace Kiwi::Fs::Tmpfs
{
        struct Dir
        {
                Lib::Vector<Node *> nodes;
        };
} // namespace Kiwi::Fs::Tmpfs