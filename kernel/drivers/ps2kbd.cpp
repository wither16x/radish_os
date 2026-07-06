#include <cpu/io.hpp>
#include <drivers/ps2kbd.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>

using kernel::lib::log::logger;
using kernel::lib::u8, kernel::lib::u16;

namespace kernel::drivers::ps2kbd {

namespace {

// The following definitions are for the PS/2 controller

enum ControllerCommand : u8 {
        CMD_READ_CONFIG         = 0x20,
        CMD_READ_BYTE           = 0x21,
        CMD_WRITE_CONFIG        = 0x60,
        CMD_WRITE_NEXT_BYTE     = 0x61,
        CMD_DISABLE_PORT_2      = 0xa7,         // only if 2nd port is supported
        CMD_ENABLE_PORT_2       = 0xa8,         // only if 2nd port is supported
        CMD_TEST_PORT_2         = 0xa9,         // same as above
        CMD_TEST_CONTROLLER     = 0xaa,
        CMD_TEST_PORT_1         = 0xab,
        CMD_READ_ALL            = 0xac,
        CMD_DISABLE_PORT_1      = 0xad,
        CMD_ENABLE_PORT_1       = 0xae,
        CMD_READ_INPUT_PORT     = 0xc0,
        CMD_CP_INPUT_03_STATUS  = 0xc1,         // copy bits 0-3 of input port to status bits 4-7
        CMD_CP_INPUT_47_STATUS  = 0xc2,         // copy bits 4-7 of input port to status bits 4-7
        CMD_READ_OUTPUT_PORT    = 0xd0,
        CMD_WRITE_NEXT_BYTE_OUT = 0xd1,
        CMD_WRITE_NEXT_BYTE_OUT1= 0xd2,         // only if 2 ports supported
        CMD_WRITE_NEXT_BYTE_OUT2= 0xd3,         // same as above
        CMD_WRITE_NEXT_BYTE_IN2 = 0xd4,         // same as above
        CMD_PULSE_OUTPUT_LINE   = 0xf0
};

enum ControllerConfigurationBit : u8 {
        CFG_PORT1_INTERRUPT = 1 << 0,
        CFG_PORT2_INTERRUPT = 1 << 1,
        CFG_SYSTEM_FLAG = 1 << 2,
        CFG_ZERO0 = 1 << 3,                              // should be zero
        CFG_PORT1_CLOCK = 1 << 4,
        CFG_PORT2_CLOCK = 1 << 5,
        CFG_PORT1_TRANSLATION = 1 << 6,
        CFG_ZERO1 = 1 << 7                               // should be zero
};

enum ControllerOutputPort : u8 {
        OUT_SYSTEM_RESET = 1 << 0,
        OUT_A20_GATE = 1 << 1,
        OUT_PORT2_CLOCK = 1 << 2,
        OUT_PORT2_DATA = 1 << 3,
        OUT_PORT1_FULL = 1 << 4,
        OUT_PORT2_FULL = 1 << 5,
        OUT_PORT1_CLOCK = 1 << 6,
        OUT_PORT1_DATA = 1 << 7
};

// The following definitions are for the PS/2 keyboard

enum KeyboardCommand : u8 {
        KBD_SET_LEDS            = 0xed,
        KBD_ECHO                = 0xee,
        KBD_MANAGE_SCANCODE_SET = 0xf0,
        KBD_IDENTIFY            = 0xf2,
        KBD_SET_TYPEMATIC_RATE  = 0xf3,
        KBD_ENABLE_SCANNING     = 0xf4,
        KBD_DISABLE_SCANNING    = 0xf5,
        KBD_SET_DEFAULT_PARAMS  = 0xf6,
        KBD_SET_ALL_KEYS_TM_AR  = 0xf7, // TypeMatic/AutoRepeat, scancode set 3 only
        KBD_SET_ALL_KEYS_MK_RL  = 0xf8, // MaKe/ReLease, scancode set 3 only
        KBD_SET_ALL_KETS_MK     = 0xf9, // guess for which scancode it is
        KBD_SET_ALL_KEYS_TO_ALL = 0xfa, // guess again
        KBD_SET_KEY_TM_AR       = 0xfb, // scancode 3 has so much features man
        KBD_SET_KEY_MK_RL       = 0xfc, // this is not fair for the other scancode sets
        KBD_SET_KEY_MK          = 0xfd, // they must be soooo sad rn
        KBD_RESEND_LAST_BYTE    = 0xfe,
        KBD_SELF_TEST           = 0xff
};

enum KeyboardResponse : u8 {
        KBD_RSP_ACK                             = 0xfa,
        KBD_RSP_RESEND                          = 0xfe,
        KBD_RSP_ECHO                            = 0xee,
        KBD_RSP_KEY_DETECTION_ERROR0            = 0,
        KBD_RSP_INTERNAL_BUFFER_OVERRUN0        = 0,
        KBD_RSP_SELF_TEST_PASSED                = 0xaa,
        KBD_RSP_SELF_TEST_FAILED0               = 0xfc,
        KBD_RSP_SELF_TEST_FAILED1               = 0xfd,
        KBD_RSP_KEY_DETECTION_ERROR1            = 0xff,
        KBD_RSP_INTERNAL_BUFFER_OVERRUN1        = 0xff
};

int controller_perform_self_test()
{
        send_command(ControllerCommand::CMD_TEST_CONTROLLER);

        u8 self_test_res = read();
        if (self_test_res != 0x55)
                return -1;

        return 0;
}

u8 keyboard_perform_self_test()
{
        write(KeyboardCommand::KBD_SELF_TEST);
        u8 kbd_self_test_res = read();
        if (kbd_self_test_res != KeyboardResponse::KBD_RSP_ACK)
                return kbd_self_test_res;

        u8 kbd_self_test_res2 = read();
        if (kbd_self_test_res2 != KeyboardResponse::KBD_RSP_SELF_TEST_PASSED)
                return kbd_self_test_res2;

        return KeyboardResponse::KBD_RSP_SELF_TEST_PASSED;
}

void write_config()
{
        send_command(ControllerCommand::CMD_READ_CONFIG);
        u8 conf = read();
        
        conf |= ControllerConfigurationBit::CFG_PORT1_INTERRUPT;
        conf &= ~ControllerConfigurationBit::CFG_PORT1_TRANSLATION;

        send_command(ControllerCommand::CMD_WRITE_CONFIG);
        write(conf);
}

u8 set_scancode_set(int n)
{
        write(KeyboardCommand::KBD_MANAGE_SCANCODE_SET);
        u8 ack1 = read();
        if (ack1 != KeyboardResponse::KBD_RSP_ACK)
                return ack1;

        write(n);
        u8 ack2 = read();
        if (ack2 != KeyboardResponse::KBD_RSP_ACK)
                return ack2;

        return KeyboardResponse::KBD_RSP_ACK;
}

u8 enable_scanning()
{
        write(KeyboardCommand::KBD_ENABLE_SCANNING);
        u8 ack3 = read();
        return ack3;
}

} /* anonymous namespace */

// --------------------------------------------------
void init()
{
        send_command(ControllerCommand::CMD_DISABLE_PORT_1);
        send_command(ControllerCommand::CMD_DISABLE_PORT_2);

        flush_output_buffer();

        int controller_self_test_res = controller_perform_self_test();
        if (controller_self_test_res < 0) {
                logger.err("ps/2 controller self test failed with value 0x%x", controller_self_test_res);
                return;
        }

        write_config();

        send_command(ControllerCommand::CMD_ENABLE_PORT_1);

        flush_output_buffer();
        send_command(ControllerCommand::CMD_TEST_PORT_1);
        u8 port1_test_res = read();
        if (port1_test_res != 0) {
                logger.err("ps/2 port 1 test failed with value 0x%x", port1_test_res);
                return;
        }

        u8 kbd_self_test_res = keyboard_perform_self_test();
        if (kbd_self_test_res != KeyboardResponse::KBD_RSP_SELF_TEST_PASSED) {
                logger.err("ps/2 keyboard self test failed with value 0x%x", kbd_self_test_res);
                return;
        }

        u8 set_scancode_set_res = set_scancode_set(2);
        if (set_scancode_set_res != KeyboardResponse::KBD_RSP_ACK) {
                logger.err("ps/2 keyboard scancode set failed with value 0x%x", set_scancode_set_res);
                return;
        }

        u8 enable_scan_res = enable_scanning();
        if (enable_scan_res != KeyboardResponse::KBD_RSP_ACK) {
                logger.err("ps/2 keyboard enable scanning failed with value 0x%x", enable_scan_res);
                return;
        }

        logger.ok("initialized ps/2 controller and keyboard");
}
// --------------------------------------------------

u8 read()
{
        wait_output_ready();
        return cpu::input_byte_port(ControllerPort::PORT_DATA);
}

void write(u8 byte)
{
        wait_input_ready();
        cpu::output_byte_port(ControllerPort::PORT_DATA, byte);
}

void flush_output_buffer()
{
        while (cpu::input_byte_port(PORT_STATUS) & ST_OUTPUT_BUFFER_STATUS)
                cpu::input_byte_port(ControllerPort::PORT_DATA);
}

} /* namespace kernel::drivers::ps2kbd */