void stage2_main()
{
        unsigned short *vga = (unsigned short *)0xb8000;

        for (int col = 0; col < 80; col++) {
                for (int row = 0; row < 25; row++)
                        vga[row * 80 + col] = (0x0f << 8) | ' ';
        }

        vga[0] = (0x0f << 8) | 'B';
        vga[1] = (0x0f << 8) | 'O';
        vga[2] = (0x0f << 8) | 'O';
        vga[3] = (0x0f << 8) | 'T';

        while (true)
                __asm__ volatile ("hlt");
}