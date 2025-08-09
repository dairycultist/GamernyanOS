// transmit data over serial connection with UART
// Reference: https://www.appelsiini.net/2011/simple-usart-with-avr-libc/

#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_RATE ((F_CPU / (16UL * BAUD)) - 1)

// initialize UART (serial communication)
void uart_init() {

    UBRR0H = (byte) (BAUD_RATE >> 8);
    UBRR0L = (byte) BAUD_RATE;

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   // enable receiver (RX) and transmitter (TX)
}

void uart_putbyte(byte data) {

    while (!(UCSR0A & _BV(UDRE0)));	// wait for empty register
    UDR0 = data;					// put data into register (queuing it for transmission)
}

void uart_putstring(byte *data) {

	while (*data)
		uart_putbyte(*data++);
}

byte uart_getbyte() {

	while (!(UCSR0A & _BV(RXC0)));	// wait until data exists
    return UDR0;					// return data
}
