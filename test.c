#include <avr/io.h>

#define F_CPU 16000000UL // CPU frequency (can also be done with -DF_CPU=16000000UL commandline argument to avr-gcc)
#define BAUD 9600
#define BAUD_RATE ((F_CPU / (16UL * BAUD)) - 1)

typedef unsigned char byte;

void uart_putchar(byte data) {

    // wait for empty register
    while (!(UCSR0A & _BV(UDRE0)));

    // put data into register (queuing it for transmission)
    UDR0 = data;
}

byte uart_getchar() {

	// wait until data exists
	while (!(UCSR0A & _BV(RXC0)));

	// return data
    return UDR0;
}

int main() {

	// initialize UART (serial communication)
    UBRR0H = (byte) (BAUD_RATE >> 8);
    UBRR0L = (byte) BAUD_RATE;

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* set frame format: 8 data bits, 1 stop bit, no parity */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* enable receiver (RX) and transmitter (TX) */

	uart_putchar('H');
	uart_putchar('e');
	uart_putchar('l');
	uart_putchar('l');
	uart_putchar('o');
	uart_putchar('\n');

	while (1) {
		uart_putchar(uart_getchar());
	}

    return 0;
}
