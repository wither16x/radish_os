void _start()
{
        const char *msg = "Hello from syscall 0 in userspace!\r\n";

        __asm__ volatile (
                "mov $0, %%rax\n"
                "int $0x80"
                ::
                "b"(msg)
        );

        while (1);
}