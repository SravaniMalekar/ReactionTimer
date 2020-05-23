/*
 * Timer1.c
 *
 * Created: 23-05-2020 17:31:19
 * Author : Sravani Malekar
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"
#define LED_DDR DDRB
#define BUTTON_PORT PORTD
#define BUTTON_PIN PIND 
#define LED_PORT PORTB
#define BUTTON 4


void init_Timer1(void){
	TCCR1B = (1 << CS12)|(1 << CS10); // Setting prescaler as 1024
	
}
void RandomDelay(void);
int main(void)
{
    char byte;
	uint16_t timerValue;
	
	init_Timer1();    // initializing timer1
	USART_Init();     //initializing uart 
	LED_DDR |= 0xFF;  //setting led port as output
	BUTTON_PORT |= (1 << BUTTON); //enable internal pull up
	
	serialString("\r\nReaction Timer:\r\n");
	serialString("Press any key to start:\r\n");
    while (1) 
    {
		byte = ReceiveByte();      // press any key
		serialString("\r\nGet Ready..");
		RandomDelay();
		
		serialString("\r\nGo!\r\n");
		LED_PORT |= 0XFF;
		TCNT1 =0;                 //reset counter
		if((BUTTON_PIN &(1 << BUTTON))==0){
			serialString("You're cheating.\r\n");
		}
		else{
			while(BUTTON_PIN & (1 << BUTTON));
			timerValue = TCNT1 >> 4;
			printMilliseconds(timerValue);
		}
		LED_PORT = 0x00;
		serialString("Press any key to try again.\r\n");
		
			
    }
	return 0;
}
void RandomDelay(void){
	uint8_t randomTime;
	
	_delay_ms(1000);
	randomTime = (uint8_t) TCNT1; // keeps only the least significant 8bits
	while(--randomTime){
		_delay_ms(10);
	}
}

