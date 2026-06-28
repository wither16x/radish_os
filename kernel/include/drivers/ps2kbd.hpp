#pragma once

#include <lib/typing.hpp>

namespace kernel::drivers::ps2kbd {

enum ControllerPort : lib::u16 {
        PORT_DATA       = 0x60,         // RW
        PORT_STATUS     = 0x64,         // R
        PORT_CMD        = 0x64          // W
};

/// These flags are contained by the status register, represented here by
/// `Port::PORT_STATUS`.
enum ControllerStatus : lib::u8 {
        ST_OUTPUT_BUFFER_STATUS = 1 << 0,
        ST_INPUT_BUFFER_STATUS = 1 << 1,
        ST_SYSTEM_FLAG = 1 << 2,
        ST_CMD_DATA = 1 << 3,
        ST_UNKNOWN0 = 1 << 4,                    // chipset specific
        ST_UNKNOWN1 = 1 << 5,                    // chipset specific
        ST_TIMEOUT_ERROR = 1 << 6,
        ST_PARITY_ERROR = 1 << 7
};

void init();

} /* namespace kernel::drivers::ps2kbd */