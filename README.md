# GamernyanOS (stupid working title)

I'm gonna be working with the [AVR architecture](https://en.wikipedia.org/wiki/Atmel_AVR_instruction_set), cuz I've used it before, it's easily emulated with QEMU, has a simple instruction set, and is used by Arduinos (which are easy to get your hands on).

## Run

### Prerequisites

`brew tap osx-cross/avr && brew install avr-gcc` install AVR-GCC

`brew install qemu` install [QEMU](https://www.qemu.org/) emulator

### Compiling and running

`avr-gcc -mmcu=atmega328p -Os -o test.elf test.c` compile C code to AVR gcc

`qemu-system-avr -machine uno -bios test.elf -display none -serial stdio` run test.elf (not using -nographic because then you can't terminate the session for some reason)

> [!TIP]
> Run `qemu-system-avr -machine help` to see QEMU-provided machines that run AVR architecture (`atmega328p` is aliased as `uno`).

## References

https://qemu-project.gitlab.io/qemu/system/target-avr.html

https://dmitry.gr/?r=05.Projects&proj=07.%20Linux%20on%208bit

[serial communication with USART](https://www.appelsiini.net/2011/simple-usart-with-avr-libc/)

kernel - manages resources, connects software and hardware, I/O, device drivers

https://wiki.osdev.org/Assembly

QEMU wants a .iso file (disk image)

gcc links everything together and builds a binary

- [bootloader](https://github.com/memtest86plus/memtest86plus/blob/main/boot/x86/startup64.S) written in assembly (.S) that sets up the stack (and maybe some other stuff) and calls the C main function. startup/entrypoint
- [C code](https://github.com/memtest86plus/memtest86plus/blob/main/app/main.c)!!

https://cstdspace.quora.com/How-to-start-making-a-simple-operating-system-in-C
