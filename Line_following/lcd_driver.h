/*
 * Interface definition of driver for the LCD Display on the 3Pi 
 * Robotic Platform
 * See lcd_driver.c for implementation
 * Author: Doug Summerville, Binghamton University <dsummer@binghamton.edu>
 * Last updated Feb 11, 2020
*/

#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <stdint.h>

typedef enum { 	TURN_OFF_DISPLAY	=0x00, 
		CLEAR_DISPLAY		=0x01,
	       	MOVE_CURSOR_HOME 	=0x02,
		TURN_ON_DISPLAY		=0x0C,
	       	TURN_ON_STEADY_CURSOR  	=0x0E,
	       	TURN_ON_BLINKING_CURSOR	=0x0F
} lcd_command_t;

void initialize_LCD_driver(); 
void LCD_print_string(const char *str);
void LCD_move_cursor_to_col_row(uint8_t column, uint8_t row);
void LCD_execute_command( lcd_command_t);
void LCD_print_hex4( uint8_t);
void LCD_print_hex8( uint8_t);
void LCD_print_hex16( uint16_t);
void LCD_print_hex32( uint32_t);
void LCD_putchar( char );
#endif