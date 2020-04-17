#include <avr/io.h>

void configure_reflectance_sensors(){
	DDRC|=(1<<5);
	PORTC|=(1<<5);
	//pullup resistors
	DDRC&=~((1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0));
	PORTC|=((1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0));
}

uint8_t read_reflectance_sensors(){
	uint8_t sensors=0;
	
	if(PINC=(1<<4))
	sensors+=16;
	if(PINC=(1<<3))
	sensors+=8;
	if(PINC=(1<<2))
	sensors+=4;
	if(PINC=(1<<1))
	sensors+=2;
	if(PINC=(1<<0))
	sensors+=1;

	return sensors;
}