#define F_CPU 16000000
#define BAUD  9600
#define BRC   ((F_CPU/16/BAUD)-1)

#include <avr/io.h>
#include <util/delay.h>

void USART_Init(void);
void serialChar(uint8_t c);
void serialString(char *str);
unsigned char ReceiveByte(void);
void printMilliseconds(uint16_t value);
void USART_Init()
{
	UBRR0L = BRC;
	UBRR0H = (BRC >> 8);
	
	UCSR0B = (1 << TXEN0)|(1 << RXEN0);
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

void serialChar(uint8_t c)
{
	while(!(UCSR0A & (1 << UDRE0 )));
	UDR0 = c ;
	
}
void serialString(char *str)
{
	while(*str)
	{
		serialChar(*str++);
	}
	_delay_ms(1000);
}
unsigned char ReceiveByte(void){
	while(!(UCSR0A & (1 << RXC0 )));
	return UDR0;
}
void printMilliseconds(uint16_t value) {
  /* Given a value in milliseconds, prints out how many seconds
     you took over the serial port.  Does ascii conversion, prints
     decimal point, and drops extra leading zeros.
   */
  uint8_t digit;

  serialString("\r\nYou took ");
                                               /* add up ten-thousands */
  digit = 0;
  while (value >= 10000) {
    digit++;
    value -= 10000;
  }
  if (digit) {
    serialChar('0' + digit);
  }
                                                   /* add up thousands */
  digit = 0;
  while (value >= 1000) {
    digit++;
    value -= 1000;
  }
  serialChar('0' + digit);

                            /* decimal point here b/c dividing by 1000 */
  serialChar('.');

                                                    /* add up hundreds */
  digit = 0;
  while (value >= 100) {
    digit++;
    value -= 100;
  }
  serialChar('0' + digit);
                                                        /* add up tens */
  digit = 0;
  while (value >= 10) {
    digit++;
    value -= 10;
  }
  serialChar('0' + digit);

  // Ones digit is easy.
  serialChar('0' + value);

  serialString(" seconds.\r\n");
}
