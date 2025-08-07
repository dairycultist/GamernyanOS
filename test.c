// google gemini gave me this code lol

#include <avr/io.h>
#define F_CPU 16000000UL // Define CPU frequency, can also be done with -DF_CPU=16000000UL commandline argument to avr-gcc
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

void UART_init(void) {

    // Set baud rate
    UBRR0H = (unsigned char) (UBRR_VALUE >> 8);
    UBRR0L = (unsigned char) UBRR_VALUE;

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C = (3 << UCSZ00); // UCSZ00 and UCSZ01 bits for 8 data bits
}

void UART_transmit(unsigned char data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));

    // Put data into buffer (register UDR0), queuing it for transmission
    UDR0 = data;
}

int main(void) {
    UART_init();

	UART_transmit('H');
	UART_transmit('e');
	UART_transmit('l');
	UART_transmit('l');
	UART_transmit('o');
	UART_transmit('\n');

    return 0;
}
