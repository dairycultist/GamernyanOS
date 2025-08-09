# GamernyanOS (stupid working title)

either I make the lamest general purpose OS imagineable or make a "gaming OS" like the software you'd find loaded on a [3DS](https://en.wikipedia.org/wiki/Nintendo_3DS_system_software) or whatever

## Features

- Reading from and writing to serial connection (via [UART](https://www.appelsiini.net/2011/simple-usart-with-avr-libc/))
- (TODO) Simple shell commands (and code separation...)
- (TODO) Non-volatile storage via SD card peripheral (Arduino doesn't come with a filesystem/\<stdio.h> implementation)

## Run

### Prerequisites

Install AVR-GCC and [QEMU](https://www.qemu.org/) emulator.

```
brew tap osx-cross/avr && brew install avr-gcc
brew install qemu
```

### Compiling and running

1. Compile C code to AVR assembly. `-Os` tells the compiler to optimize for code size.

```
avr-gcc -mmcu=atmega328p -Os -o main.elf main.c
```

2. Create virtual SD card image (which will be a peripheral and the storage of our emulated machine).

```
qemu-img create -f raw sdcard.img 256K
```

4. Run `test.elf` in a virtual environment. (not using -nographic because then you can't terminate the session for some reason)

gotta remember to tell QEMU to emulate an SD card controller and attach our SD card image

add `-drive file=sdcard.img,format=raw` to the run command

Add SD card image as a virtual _drive_, which is referenced by a virtual _device_ (the SD card controller).
`-device sd-card,drive=my_sdcard -drive id=my_sdcard,if=sd,format=raw,file=sdcard.img`

```
qemu-system-avr -machine uno -bios main.elf -display none -serial stdio
```

> [!TIP]
> Run `qemu-system-avr -machine help` to see QEMU-provided machines that run AVR architecture (`atmega328p` is aliased as `uno`).

## References

Uses the [AVR architecture](https://en.wikipedia.org/wiki/Atmel_AVR_instruction_set), cuz I've used it before, it's easily emulated with QEMU, has a simple instruction set, and is used by Arduinos (which are easy to get your hands on). Arduinos (8-bit AVR microcontrollers) don't really have software, they have modifiable firmware. This means my OS isn't a piece of software started up by the BIOS (firmware), it IS the BIOS. So I guess I don't have to think of .iso images, boot sectors/bootloaders, or whatever...

SD cards are block-addressed, meaning data is written and read in fixed-size blocks (typically 512 bytes), as opposed to arbitrary addresses. You must send specific commands to the SD card to initialize it, select the desired block address, and then send the data for writing. These commands are part of the SD card's SPI or SDIO interface protocol. This is __raw data__, any filesystem implementation (for creating and locating allocations) must be done yourself

- [Running an AVR program with QEMU](https://qemu-project.gitlab.io/qemu/system/target-avr.html)
- [Transmit data over serial connection with UART](https://www.appelsiini.net/2011/simple-usart-with-avr-libc/)
- [AVR initialize connected SD card](http://www.rjhcoding.com/avrc-sd-interface-1.php)

bootloader written in assembly (.S) that sets up the stack (and maybe some other stuff) and calls the C main function. startup/entrypoint

kernel - manages resources, connects software and hardware, I/O, device drivers

`-DF_CPU=16000000UL` may be added to avr-gcc command to define CPU frequency if omitting `#define F_CPU 16000000UL` from script

https://dmitry.gr/?r=05.Projects&proj=07.%20Linux%20on%208bit

https://cstdspace.quora.com/How-to-start-making-a-simple-operating-system-in-C
