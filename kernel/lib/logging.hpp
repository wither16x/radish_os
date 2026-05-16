#pragma once

namespace kernel {

namespace lib {

namespace log {

void status(const char *fmt, ...);
void ok();
void failed();

} /* namespace log */

} /* namespace lib */

} /* namespace kernel */