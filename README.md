# GamernyanOS (stupid working title)

either I make the lamest general purpose OS imagineable or make a "gaming OS" like the software you'd find loaded on a [3DS](https://en.wikipedia.org/wiki/Nintendo_3DS_system_software) or whatever

## Features

- Reading from and writing to serial connection (via [UART](https://www.appelsiini.net/2011/simple-usart-with-avr-libc/))
- (TODO) Simple shell commands (and code separation...)
- (TODO) Custom non-volatile file system (Arduino write to memory in raw sd card since Arduino doesn't come with a filesystem/\<stdio.h> implementation)

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

```
qemu-system-avr -machine uno -bios main.elf -display none -serial stdio
```

> [!TIP]
> Run `qemu-system-avr -machine help` to see QEMU-provided machines that run AVR architecture (`atmega328p` is aliased as `uno`).

## References

Uses the [AVR architecture](https://en.wikipedia.org/wiki/Atmel_AVR_instruction_set), cuz I've used it before, it's easily emulated with QEMU, has a simple instruction set, and is used by Arduinos (which are easy to get your hands on). Arduinos (8-bit AVR microcontrollers) don't really have software, they have modifiable firmware. This means my OS isn't a piece of software started up by the BIOS (firmware), it IS the BIOS. So I guess I don't have to think of .iso images, boot sectors/bootloaders, or whatever...

- [Running an AVR program with QEMU](https://qemu-project.gitlab.io/qemu/system/target-avr.html)
- [Transmit data over serial connection with UART](https://www.appelsiini.net/2011/simple-usart-with-avr-libc/)
- [AVR initialize connected SD card](http://www.rjhcoding.com/avrc-sd-interface-1.php)

SD cards are block-addressed, meaning data is written and read in fixed-size blocks (typically 512 bytes), as opposed to arbitrary addresses. You must send specific commands to the SD card to initialize it, select the desired block address, and then send the data for writing. These commands are part of the SD card's SPI or SDIO interface protocol. This is __raw data__, any filesystem implementation (for creating and locating allocations) must be done yourself

```
Initialize the SD card: Send the necessary commands (e.g., CMD0, CMD8, CMD55, ACMD41) to put the SD card into SPI mode and initialize it for operation.
Select the block address: Send a command to specify the starting block address where you want to write your data.
Send the write command: Send the appropriate command for single-block or multi-block write operations.
Transmit the data: Send the raw data in 512-byte blocks to the SD card.
Handle CRC: If enabled, ensure correct CRC calculations and handling for data integrity.
Wait for completion: Monitor the SD card's status to confirm the write operation has completed.
```

 add `-drive file=sdcard.img,format=raw` to the run command

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

bootloader written in assembly (.S) that sets up the stack (and maybe some other stuff) and calls the C main function. startup/entrypoint

kernel - manages resources, connects software and hardware, I/O, device drivers

`-DF_CPU=16000000UL` may be added to avr-gcc command to define CPU frequency if omitting `#define F_CPU 16000000UL` from script

https://dmitry.gr/?r=05.Projects&proj=07.%20Linux%20on%208bit

https://cstdspace.quora.com/How-to-start-making-a-simple-operating-system-in-C
