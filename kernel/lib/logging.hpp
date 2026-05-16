#pragma once

namespace kernel::lib::log {

// How the three functions below are supposed to be used:
// status("doing stuff");
// do_stuff();
// ok(); / failed();
// Note that they should only be used for early logs
// (my kernel is so early that I do not have a complete
// logging system now).
void status(const char *fmt, ...);
void ok();
void failed();

} /* namespace kernel::lib::log */