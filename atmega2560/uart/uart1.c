/*
 * uart1.c
 * ================================================
 * Denise Ratasich
 * 15.11.2012 (copied from uart0.c)
 *
 * asynchronous mode, 8 bit, 1 stop bit
 */

#include <avr/interrupt.h>
#include "uart1.h"

// baudrate register values from datasheet:
#define UBRRH_9600	0
#define UBRRL_9600	207
#define UBRRH_19200	0
#define UBRRL_19200	103	
#define UBRRH_57600	0
#define UBRRL_57600	34	
#define UBRRH_115200	0
#define UBRRL_115200	16

static volatile uint8_t uart1_receive_flag = 0;
static volatile char uart1_receive_data;

void uart1_init() {
  cli();

  // set baud rate
  UBRR1H = UBRRH_19200;
  UBRR1L = UBRRL_19200;
	
  // double speed, buffer ready to be written
  UCSR1A |= (1<<U2X1) | (1<<UDRE1); 

  // already default value of UART register C:
  //UCSR1C &= ~(1<<UMSEL10) & ~(1<<UMSEL11);	// asynchronous mode
  //UCSR1C &= ~(1<<UPM10) & ~(1<<UPM11);	// no parity
  //UCSR1C &= ~(1<<USBS1);			// 1 stopbit
  //UCSR1B &= ~(1<<UCSZ12);
  //UCSR1C |= (1<<UCSZ11) | (1<<UCSZ10);	// 8-bit

  // enable transmit/receive, receive-interrupt
  UCSR1B |= (1<<TXEN1) | (1<<RXEN1) | (1<<RXCIE1);

  sei();
}

void uart1_putc(char myData) {
  // wait for empty transmit buffer
  while ( !( UCSR1A & (1<<UDRE1)) );
  UDR1 = myData;
}

void uart1_print(char* string) {
  while(*string != '\0') {
    uart1_putc(*string);
    string++;
  }
}

void uart1_println(char* string) {
  while(*string != '\0') {
    uart1_putc(*string);
    string++;
  }

  uart1_putc(0x0A); //'\n');
  uart1_putc(0x0D); //'\r');
}

// 0 ... 255
void uart1_printUInt8(uint8_t value) {
  uart1_putc(value/100 + '0');
  value %= 100;
  uart1_putc(value/10 + '0');
  uart1_putc(value%10 + '0');
}

// - 128 ... 127
void uart1_printInt8(int8_t value) {
  if (value < 0) {
    uart1_putc('-');
    value = - value;
  } else
    uart1_putc(' ');

  uart1_printUInt8((uint8_t) value);
}

// 0 ... 65536
void uart1_printUInt16(uint16_t value) {
  uart1_putc(value/10000 + '0');
  value %= 10000;
  uart1_putc(value/1000 + '0');
  value %= 1000;
  uart1_putc(value/100 + '0');
  value %= 100;
  uart1_putc(value/10 + '0');
  uart1_putc(value%10 + '0');
}

// -32768 ... 32767
void uart1_printInt16(int16_t value) {
  if (value < 0) {
    uart1_putc('-');
    value = - value;
  } else
    uart1_putc(' ');

  uart1_printUInt16((uint16_t) value);
}

// receive complete
ISR(USART1_RX_vect) {
  uart1_receive_data = UDR1;
  uart1_receive_flag = 1;
}

uint8_t uart1_getc(char* data) {
  if(uart1_receive_flag) {
    *data = uart1_receive_data;
    uart1_receive_flag = 0;
    return 1;
  }

  return 0;
}

int8_t uart1_getUInt16(uint16_t* res) {
  uint8_t i;
  uint16_t fact;
  char c;

  *res = 0;
  fact = 10000;

  for (i = 0; i < 5; i++) 
  {
    while (!uart1_getc(&c));
    uart1_putc(c);

    if (c < '0' || c > '9')
      return -1;

    (*res) += (c-'0')*fact;
    fact /= 10;
  }

  return 0;
}

float uart1_getFloat2d3(void) {
  uint8_t i;
  char c;
  float fact, res = 0;

  // receive 2 digits before comma
  fact = 10;
  for (i = 0; i < 2; i++) 
  {
    while (!uart1_getc(&c));
    uart1_putc(c);

    if (c < '0' || c > '9')
      return -1;
    res += (c-'0')*fact;
    fact /= 10;
  }
  
  // receive comma
  while (!uart1_getc(&c));
  uart1_putc(c);

  if (c != '.')
      return -1;

  // receive 3 digits after comma
  for (i = 0; i < 3; i++) 
  {
    while (!uart1_getc(&c));
    uart1_putc(c);

    if (c < '0' || c > '9')
      return -1;
    res += (c-'0')*fact;
    fact /= 10;
  }

  return res;
}
