#include<avr/io.h>

void configure_left_motor(){
	DDRD|=(1<<5)|(1<<6);
}

void configure_right_motor(){
	DDRB|=(1<<3);
	DDRD|=(1<<3);
}

void configure_motors(){
	configure_left_motor();
	configure_right_motor();
}

void left_motor_on(){
	PORTD|=(1<<3);
	PORTD&=~(1<<3);
}

void brake_left_motor(){
	PORTD|=(1<<5)|(1<<6);
}

void right_motor_on(){
	PORTB|=(1<<3);
	PORTD&=~(1<<3);
}

void brake_right_motor(){
	PORTB|=(1<<3);
	PORTD|=(1<<3);
}

void pwm_left_motor(uint8_t duty_cycle){
	uint8_t counter = 0;
	uint8_t state = 0;
	
	while(counter<99){
		switch(state){
			case 0:
			if(counter < duty_cycle){
				left_motor_on();
				state = 0;
				break;
			} else {
				brake_left_motor();
				state = 1;
				break;
			}
			
			case 1:
			if(counter >= duty_cycle){
				brake_left_motor();
				state = 1;
				break;
			} else {
				left_motor_on();
				state = 0;
				break;
			}
			default:
				brake_left_motor();
				state = 1;
				break;
		}
		
		counter++;
	}
	
}

void pwm_right_motor(uint8_t duty_cycle){
	uint8_t counter = 0;
	uint8_t state = 0;
	
	while(counter<99){
		switch(state){
			case 0:
			if(counter < duty_cycle){
				right_motor_on();
				state = 0;
				break;
			} else {
				brake_right_motor();
				state = 1;
				break;
			}
			
			case 1:
			if(counter >= duty_cycle){
				brake_right_motor();
				state = 1;
				break;
			} else {
				right_motor_on();
				state = 0;
				break;
			}
			default:
			brake_right_motor();
			state = 1;
			break;
		}
		
		counter++;
	}
	
}