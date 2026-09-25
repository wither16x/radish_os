#pragma once

#include <lib/vector.hpp>
#include <lib/string.hpp>
#include <fs/ustar/decls.hpp>

namespace Kiwi::Fs::Ustar
{
        struct Dir
        {
                Lib::Vector<Node *> nodes;
        };

        Node *findDir(Node *parent, const Lib::String<> &name);
        Node *createDir(Node *parent, const Lib::String<> &name);
} // namespace Kiwi::Fs::Ustar