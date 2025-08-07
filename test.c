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

	// CLI
	byte buffer[256] = {0};
	int cursor = 0;
	byte in;

	uart_putstring("> ");

	while (1) {

		in = uart_getbyte();

		if (in == 127) { // delete

			if (cursor) {
				buffer[--cursor] = '\0';
				uart_putstring("\b \b");
			}

		} else if (in == '\r') { // enter

			uart_putbyte('\n');
			uart_putstring(buffer);
			uart_putstring("\n> ");

			while (cursor) {
				buffer[--cursor] = '\0';
			}

		} else if (in == 27) { // ansii escape codes

			// cursor up down left right are \[cA - \[cD
			uart_putbyte('\\');

		} else {

			buffer[cursor++] = in;
			uart_putbyte(in);
		}
	}

    return 0;
}
