#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Fs::Ustar
{
        constexpr int BLOCK_SIZE = 512;

        void parseArchive(Lib::u8 *archive);
} // namespace Kiwi::Fs::Ustar