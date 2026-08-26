#pragma once

namespace Kiwi::Proc
{
        /// Wait until any of the current process' children dies.
        int wait();
} // namespace Kiwi::Proc