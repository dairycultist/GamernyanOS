# GamernyanOS (stupid working title)

I'm gonna be working with the [AVR architecture](https://en.wikipedia.org/wiki/Atmel_AVR_instruction_set), cuz I've used it before, it's easily emulated with QEMU, has a simple instruction set, and is used by Arduinos (which are easy to get your hands on).

either I make the lamest general purpose OS imagineable or make a "gaming OS" like the software you'd find loaded on a [3DS](https://en.wikipedia.org/wiki/Nintendo_3DS_system_software) or whatever

## Features

- Reading from and writing to serial connection (via [UART](https://www.appelsiini.net/2011/simple-usart-with-avr-libc/))
- (TODO) Simple shell commands (and code separation...)
- (TODO) Non-volatile file system (Arduino write to memory in raw sd card since Arduino doesn't come with a filesystem/\<stdio.h> implementation)

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
avr-gcc -mmcu=atmega328p -Os -o test.elf test.c
```

2. Create virtual SD card.

3. Run `test.elf` in a virtual environment. (not using -nographic because then you can't terminate the session for some reason)

```
qemu-system-avr -machine uno -bios test.elf -display none -serial stdio
```

> [!TIP]
> Run `qemu-system-avr -machine help` to see QEMU-provided machines that run AVR architecture (`atmega328p` is aliased as `uno`).

## References

QEMU provides support for emulating SD card controllers and attaching images to represent the SD card's storage (a peripheral of an arduino) `qemu-img create -f raw sdcard.img 256K` and add `-drive file=sdcard.img,format=raw` to the run command

`-device sd-card,drive=sdcard0 -drive id=sdcard0,if=sd,format=raw,file=sdcard.img`

```
-device sd-card,drive=sdcard0:

    -device: This option is used to add a virtual device to the emulated machine.
    sd-card: This specifies that the device being added is an SD card controller.
    drive=sdcard0: This connects the SD card device to a drive with the ID sdcard0 which is defined later in the command.

-drive id=sdcard0,if=sd,format=raw,file=sdcard.img:

    -drive: This option configures a virtual drive.
    id=sdcard0: This gives a unique ID to this drive, allowing it to be referenced by devices like the SD card controller.
    if=sd: This indicates the interface type for the drive, which is an SD card interface in this case.
    format=raw: This specifies the format of the disk image. raw means it's a raw disk image, directly representing the data without any additional metadata.
    file=sdcard.img: This defines the file on your host system that serves as the backing store for the virtual SD card. This file will store all data written to the emulated SD card, providing the persistence you're looking for
```

Arduinos (what I'm programming for, 8-bit AVR microcontrollers) don't really have software, they have modifiable firmware. This means my OS isn't a piece of software started up by the BIOS (firmware), it IS the BIOS. So I guess I don't have to think of .iso images, boot sectors/bootloaders, or whatever...

bootloader written in assembly (.S) that sets up the stack (and maybe some other stuff) and calls the C main function. startup/entrypoint

kernel - manages resources, connects software and hardware, I/O, device drivers

`-DF_CPU=16000000UL` may be added to avr-gcc command to define CPU frequency if omitting `#define F_CPU 16000000UL` from script

https://qemu-project.gitlab.io/qemu/system/target-avr.html

https://dmitry.gr/?r=05.Projects&proj=07.%20Linux%20on%208bit

https://cstdspace.quora.com/How-to-start-making-a-simple-operating-system-in-C
