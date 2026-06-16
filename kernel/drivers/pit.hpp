#pragma once

#include <lib/typing.hpp>

namespace kernel::drivers::pit {

void init();
void sleep(lib::u64 ms);        // time in miliseconds
lib::u64 get_tics();
lib::u64 get_seconds();
void set_tics(lib::u64 n);
void set_seconds(lib::u64 n);
bool is_sleeping();
bool consumed_tick();
void consume_tick(bool state);

} /* namespace kernel::drivers::pit */