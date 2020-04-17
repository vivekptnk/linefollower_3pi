#include <avr/io.h>

void configure_left_motor();
void configure_right_motor();
void configure_motors();
void left_motor_on();
void brake_left_motor();
void right_motor_on();
void brake_right_motor();
void pwm_left_motor(uint8_t duty_cycle);
void pwm_right_motor(uint8_t duty_cycle);