// SD card transfer protocol (SPI, Serial Peripheral Interface) implementation
// Reference: http://www.rjhcoding.com/avrc-sd-interface-1.php

// Initialize the SD card: Send the necessary commands (e.g., CMD0, CMD8, CMD55, ACMD41) to put the SD card into SPI mode and initialize it for operation.
// Select the block address: Send a command to specify the starting block address where you want to write your data.
// Send the write command: Send the appropriate command for single-block or multi-block write operations.
// Transmit the data: Send the raw data in 512-byte blocks to the SD card.
// Handle CRC: If enabled, ensure correct CRC calculations and handling for data integrity.
// Wait for completion: Monitor the SD card's status to confirm the write operation has completed.
