#pragma once

namespace kernel::lib {

/// Different status of an operation.
/// NOTE: since is enum is only used once/twice, it might be
/// removed.
enum class Status : int {
        Err,
        Ok
};

} /* namespace kernel::lib */