#include <avr/io.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_RATE ((F_CPU / (16UL * BAUD)) - 1)

typedef unsigned char byte;

void uart_putbyte(byte data) {

    // wait for empty register
    while (!(UCSR0A & _BV(UDRE0)));

    // put data into register (queuing it for transmission)
    UDR0 = data;
}

void uart_putstring(byte *data) {

	while (*data) {

		uart_putbyte(*data);
		data++;
	}
}

byte uart_getbyte() {

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

	uart_putstring("hello world");

	while (1) {
		uart_putbyte(uart_getbyte());
	}

    return 0;
}
