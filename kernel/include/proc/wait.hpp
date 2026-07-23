#pragma once

namespace kernel::proc {

/// Wait until any of the current process' children dies.
int wait();

} /* namespace kernel::proc */