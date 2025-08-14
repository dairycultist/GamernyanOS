# GamernyanOS (stupid working title)

either I make:
- the lamest general purpose OS imagineable
- a console's OS like the software you'd find loaded on a [3DS](https://en.wikipedia.org/wiki/Nintendo_3DS_system_software) or whatever

I want
- I/O (hardware serial connection with UART?)
- Non-volatile storage
- something something 512kb

## Background

`atmega328p` doesn't have a BIOS; instead, all hardware initialization/abstraction is handled by the bootloader (such as [Optiboot](https://github.com/Optiboot/optiboot)). While an Arduino board does come prepackaged with a bootloader (like a BIOS would), it is modifiable (unlike a BIOS). The `-bios` argument is used to pass our bootloader to QEMU to be loaded at machine start, initializing the hardware before locating and loading the operating system kernel from a storage device.

I'm gonna steal Optiboot for now so I can focus on testing a kernel software written in C, writing my own bootloader in AVR assembly later.

## Run

might look into [Microchip Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio) for AVR emulation given the limitations QEMU has with AVR

### Prerequisites

Install `avr-gcc` (compiler used to compile C for AVR architectures), `objcopy` (for converting .elf to .bin), and [QEMU](https://www.qemu.org/) (an emulator which also provides disk image utilities).

```
brew tap osx-cross/avr && brew install avr-gcc
brew install binutils && export PATH="/usr/local/opt/binutils/bin:$PATH"
brew install qemu
```

### Emulating

1. Assemble `bootloader.S` to a binary.

```
avr-gcc -mmcu=atmega328p -Os -c bootloader.S -o bootloader.elf
objcopy -O binary bootloader.elf bootloader.bin
```

(TODO Create a disk image to act as our SD card. Then load the kernel onto it.)

2. Boot in `atmega328p` virtual environment (`atmega328p` is aliased as `uno`). Use `control+option+2` to switch to serial view (optionally, instead add `-display none -serial stdio` to pipe serial output to the console).

```
qemu-system-avr -machine uno -bios bootloader.bin
```

## Notes

`qemu-img create -f raw hard_disk_drive.img 10M` create disk image

`qemu-system-avr -machine uno -drive file=hard_disk_drive.img,format=raw` run machine using `hard_disk_drive.img` as backing file (snapshot to load virtual drive from). not very useful for arduino until we can load from a backing file into a virtual SD card

SD cards are block-addressed, meaning data is written and read in fixed-size blocks (typically 512 bytes), as opposed to arbitrary addresses. You must send specific commands to the SD card to initialize it, select the desired block address, and then send the data for writing. These commands are part of the SD card's SPI or SDIO interface protocol. This is __raw data__, any filesystem implementation (for creating and locating allocations) must be done yourself

.img is also a disk image type but it's for emulating floppies (smaller storage basically) and while technically usable it's not as widely supported/easily sharable so

embedded systems programming :pensive:

`-DF_CPU=16000000UL` may be added to avr-gcc command to define CPU frequency if omitting `#define F_CPU 16000000UL` from script

`-device sd-card,drive=my_sdcard -drive id=my_sdcard,if=sd,format=raw,file=sdcard.img` Tell QEMU to emulate an SD card controller and attach our SD card image: Add SD card image as a virtual _drive_, which is referenced by a virtual _device_ (the SD card controller).

put images into program memory with QEMU's [generic loader](https://qemu-project.gitlab.io/qemu/system/generic-loader.html).

`qemu-img create -f raw sdcard.img 256K`

```
// spi.c

// SD card transfer protocol (SPI, Serial Peripheral Interface) implementation
// References: http://www.rjhcoding.com/avrc-sd-interface-1.php
//             https://en.wikipedia.org/wiki/Serial_Peripheral_Interface

// four channels, SS, SCLK, MOSI, MISO

// Initialize the SD card: Send the necessary commands (e.g., CMD0, CMD8, CMD55, ACMD41) to put the SD card into SPI mode and initialize it for operation.
// Select the block address: Send a command to specify the starting block address where you want to write your data.
// Send the write command: Send the appropriate command for single-block or multi-block write operations.
// Transmit the data: Send the raw data in 512-byte blocks to the SD card.
// Handle CRC: If enabled, ensure correct CRC calculations and handling for data integrity.
// Wait for completion: Monitor the SD card's status to confirm the write operation has completed.
```

https://dmitry.gr/?r=05.Projects&proj=07.%20Linux%20on%208bit

https://cstdspace.quora.com/How-to-start-making-a-simple-operating-system-in-C

Upon computer startup, the **Basic Input/Output System (BIOS)**, a piece of hardware-specific firmware (burnt into the hardware), initializes and tests hardware components, and provides hardware-abstracted services to the OS for boot-time hardware initialization, including establishing I/O and disk access. This differs from a **Hardware Abstraction Layer (HAL)**, which is compiled together with the (hardware-agnostic) OS logic for the target hardware, and provides additional hardware-abstracted services not provided by the BIOS.

The BIOS then loads and executes the **bootloader** (placed in the boot sector of the disk image), a piece of software that is part of the OS and which is responsible for setting up basic functionalities (such as the stack) before loading and executing the **kernel**.

The kernel is the root program of the OS that loads and executes all other programs, manages their resources (memory allocations, file pointers), and connects software and hardware (I/O, peripherals/device drivers). The kernel processes **system calls** that request hardware-specific operations so that software may remain hardware-agnostic. For example, the implementation of `scanf()` from `<stdio.h>` involves system calls to the kernel, such that software executing `scanf()` need not consider what devices are sending data to `stdin` or how, only that data _is_ being sent.
