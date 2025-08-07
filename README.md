# GamernyanOS (stupid working title)

I'm gonna be working with the [AVR architecture](https://en.wikipedia.org/wiki/Atmel_AVR_instruction_set), cuz I've used it before, it's easily emulated with QEMU, has a simple instruction set, and is used by Arduinos (which are easy to get your hands on).

## Features

- Reading from and writing to serial connection (via [UART](https://www.appelsiini.net/2011/simple-usart-with-avr-libc/))
- (TODO) Simple shell commands (and code separation...)
- (TODO) Non-volatile file system

## Run

### Prerequisites

`brew tap osx-cross/avr && brew install avr-gcc` install AVR-GCC

`brew install qemu` install [QEMU](https://www.qemu.org/) emulator

### Compiling and running

1. `avr-gcc -mmcu=atmega328p -Os -o test.elf test.c` compile C code to AVR gcc
- - `-Os` tells the compiler to optimize for code size
  - `-DF_CPU=16000000UL` may be added to define CPU frequency if omitting `#define F_CPU 16000000UL` from script
2. `qemu-system-avr -machine uno -bios test.elf -display none -serial stdio` run test.elf (not using -nographic because then you can't terminate the session for some reason)

> [!TIP]
> Run `qemu-system-avr -machine help` to see QEMU-provided machines that run AVR architecture (`atmega328p` is aliased as `uno`).

## References

An OS is built from a .iso file (disk image), but I currently only have a .elf (executable). [learning how a computer interprets a .iso is gonna be a thing I need to do](https://stackoverflow.com/questions/59593720/how-to-make-a-bootable-disc-iso-that-loads-a-kernel-elf)

bootloader written in assembly (.S) that sets up the stack (and maybe some other stuff) and calls the C main function. startup/entrypoint

https://qemu-project.gitlab.io/qemu/system/target-avr.html

https://dmitry.gr/?r=05.Projects&proj=07.%20Linux%20on%208bit

kernel - manages resources, connects software and hardware, I/O, device drivers

https://cstdspace.quora.com/How-to-start-making-a-simple-operating-system-in-C
