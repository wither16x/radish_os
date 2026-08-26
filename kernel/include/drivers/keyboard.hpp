#pragma once

/// This is a "generic" (at least abstract) keyboard driver, used as an abstraction
/// over the other keyboard drivers.

#include <lib/typing.hpp>

namespace Kiwi::Drivers::Keyboard
{
        void init();
        Lib::u8 handleKeyPress();
        char scancodeToKey(Lib::u8 scancode);
        char read();
} // namespace Kiwi::Drivers::Keyboard