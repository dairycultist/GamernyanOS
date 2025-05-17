# GamernyanOS

Find one specific (popular) architecture that I can emulated with [QEMU](https://www.qemu.org/) and focus on that. First goal: Run assembly code that outputs _something_.

kernel - manages resources, connects software and hardware, I/O, device drivers

https://wiki.osdev.org/Assembly

QEMU wants a .iso file (disk image)

gcc links everything together and builds a binary

- [bootloader](https://github.com/memtest86plus/memtest86plus/blob/main/boot/x86/startup64.S) written in assembly (.S) that sets up the stack (and maybe some other stuff) and calls the C main function. startup/entrypoint
- [C code](https://github.com/memtest86plus/memtest86plus/blob/main/app/main.c)!!

https://cstdspace.quora.com/How-to-start-making-a-simple-operating-system-in-C
