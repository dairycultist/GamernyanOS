# GamernyanOS (stupid working title)

I'm gonna be working with the [AVR architecture](https://en.wikipedia.org/wiki/Atmel_AVR_instruction_set), cuz I've used it before, it's easily emulated with QEMU, has a simple instruction set, and is used by Arduinos (which are easy to get your hands on).

either I make the lamest general purpose OS imagineable or make a "gaming OS" like the software you'd find loaded on a [3DS](https://en.wikipedia.org/wiki/Nintendo_3DS_system_software) or whatever

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

QEMU provides support for emulating SD card controllers and attaching images to represent the SD card's storage (a peripheral of an arduino) `qemu-img create -f raw sdcard.img 256K` and add `-drive file=sdcard.img,format=raw` to the run command

Arduinos (what I'm programming for) don't really have software, they have modifiable firmware. This means my OS isn't a piece of software started up by the BIOS (firmware), it IS the BIOS. So I guess I don't have to think of .iso images, boot sectors/bootloaders, or whatever...

bootloader written in assembly (.S) that sets up the stack (and maybe some other stuff) and calls the C main function. startup/entrypoint

kernel - manages resources, connects software and hardware, I/O, device drivers

https://qemu-project.gitlab.io/qemu/system/target-avr.html

https://dmitry.gr/?r=05.Projects&proj=07.%20Linux%20on%208bit

https://cstdspace.quora.com/How-to-start-making-a-simple-operating-system-in-C
