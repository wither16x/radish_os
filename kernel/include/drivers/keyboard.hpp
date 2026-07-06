#pragma once

/// This is a "generic" (at least abstract) keyboard driver, used as an abstraction
/// over the other keyboard drivers.

#include <lib/typing.hpp>

namespace kernel::drivers::keyboard {

void init();
lib::u8 handle_key_press();
char scancode_to_key(lib::u8 scancode);
char read();

} /* namespace kernel::drivers::keyboard */