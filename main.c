#include <avr/io.h>

typedef unsigned char byte;

#include "storage.c"
#include "uart.c"

int main() {

	uart_init();

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
