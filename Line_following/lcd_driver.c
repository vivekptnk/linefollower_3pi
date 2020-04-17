/*
 * Implementation of driver for the LCD Display on the 3Pi Robotic Platform
 * See lcd_driver.h for interface definition
 * Author: Doug Summerville, Binghamton University <dsummer@binghamton.edu>
 * Last updated Feb 11, 2020
*/
#include "lcd_driver.h"

#define DB0_PORT B
#define DB0_LOC 1
#define DB1_PORT B
#define DB1_LOC 4
#define DB2_PORT B
#define DB2_LOC 5
#define DB3_PORT D
#define DB3_LOC 7
#define RS_PORT D
#define RS_LOC 2
#define RW_PORT B
#define RW_LOC 0
#define E_PORT D
#define E_LOC 4
#define BF_LOC 7

#define CONCAT(A,B) A ## B
#define PORT(NAME) CONCAT(PORT,NAME)
#define PIN(NAME) CONCAT(PIN,NAME)
#define DDR(NAME) CONCAT(DDR,NAME)
#define PULLUP(NAME) PORT(NAME)
#define TOGGLE(NAME) PIN(NAME)

#ifndef F_CPU
#define F_CPU 20000000
#endif

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>


static void configure_DB_for_output();
static void configure_DB_for_input();
static void output_nibble_to_DB(uint8_t data);
static uint8_t input_nibble_from_DB();
static uint8_t do_bus_read_timing_sequence();
static void do_DB_write_timing_sequence(uint8_t data);
static void write_selected_LCD_register(uint8_t data);
static uint8_t read_selected_LCD_register();
static void send_command_to_LCD(uint8_t cmd);
static void send_data_to_LCD(uint8_t data);
static void wait_for_last_LCD_operation_to_finish();

#define set(x) PORT(x##_PORT) |= (1<<x##_LOC)
#define clr(x) PORT(x##_PORT) &= ~(1<<x##_LOC)

static void configure_DB_for_output()
{
	DDR(DB0_PORT) |= (1<<DB0_LOC);
	DDR(DB1_PORT) |= (1<<DB1_LOC);
	DDR(DB2_PORT) |= (1<<DB2_LOC);
	DDR(DB3_PORT) |= (1<<DB3_LOC);
}
static void configure_DB_for_input()
{
	DDR(DB0_PORT) &= ~(1<<DB0_LOC);
	DDR(DB1_PORT) &= ~(1<<DB1_LOC);
	DDR(DB2_PORT) &= ~(1<<DB2_LOC);
	DDR(DB3_PORT) &= ~(1<<DB3_LOC);
}
static void output_nibble_to_DB(uint8_t data)
{
	if( data & (1<<0) )
		set(DB0);
	else
		clr(DB0);
	if( data & (1<<1) )
		set(DB1);
	else
		clr(DB1);
	if( data & (1<<2) )
		set(DB2);
	else
		clr(DB2);
	if( data & (1<<3) )
		set(DB3);
	else
		clr(DB3);
}
static uint8_t input_nibble_from_DB()
{
	uint8_t data=0;
	if( PIN(DB3_PORT) & (1<<DB3_LOC))
		data |= 0x08;
	if( PIN(DB2_PORT) & (1<<DB2_LOC))
		 data |= 0x04;
	if( PIN(DB1_PORT) & (1<<DB1_LOC))
		 data |= 0x02;
	if( PIN(DB0_PORT) & (1<<DB0_LOC))
		 data |= 0x01;
	return data;
	
}
void initialize_LCD_driver()
{	
	clr(RW);
	DDR(RW_PORT) |= (1<<RW_LOC);
	clr(RS);
	DDR(RS_PORT) |= (1<<RS_LOC);
	clr(E);
	DDR(E_PORT) |= (1<<E_LOC);
	configure_DB_for_output();
	_delay_ms(15);
	do_DB_write_timing_sequence(0x3);	
	_delay_ms(6);	
	do_DB_write_timing_sequence(0x3);	
	_delay_ms(2);	
	do_DB_write_timing_sequence(0x3);	
	_delay_ms(2);	
	do_DB_write_timing_sequence(0x2);	
	wait_for_last_LCD_operation_to_finish();
	send_command_to_LCD(0x28);	
	send_command_to_LCD(0x08);	
	send_command_to_LCD(0x01);	
	send_command_to_LCD(0x06);	
}
static uint8_t do_bus_read_timing_sequence()
{
	uint8_t data;
	set(E);
	_delay_us(1);
	data=input_nibble_from_DB();
	clr(E);
	return data;
}
static uint8_t read_selected_LCD_register()
{
	uint8_t data;
	configure_DB_for_input();
	set(RW);
	data=do_bus_read_timing_sequence();
	return (data << 4)| do_bus_read_timing_sequence();
}
static void wait_for_last_LCD_operation_to_finish()
{
	uint8_t data;
	clr(RS);
	do
	{
		data=read_selected_LCD_register();
	}
	while ((data & (1<<BF_LOC)) );
}
static void do_DB_write_timing_sequence(uint8_t data)
{
	output_nibble_to_DB(data);
	set(E);
	_delay_us(1);
	clr(E);
}
static void write_selected_LCD_register(uint8_t data)
{	
	clr(RW);
	configure_DB_for_output();
	do_DB_write_timing_sequence(data >> 4);
	do_DB_write_timing_sequence(data & 0x0F);	
}
static void send_command_to_LCD(uint8_t cmd)
{
	wait_for_last_LCD_operation_to_finish();
	clr(RS);
	write_selected_LCD_register(cmd);
}
static void send_data_to_LCD(uint8_t data)
{
	wait_for_last_LCD_operation_to_finish();
	set(RS);
	write_selected_LCD_register(data);
}
void LCD_print_string(const char *str)
{
	while (*str != 0)
		send_data_to_LCD(*str++);
}
void LCD_move_cursor_to_col_row(uint8_t column, uint8_t row)
{
	uint8_t ddramaddress=(row << 6)|column;
	send_command_to_LCD( 0x80 | ddramaddress);
}
void LCD_execute_command( lcd_command_t command)
{
	send_command_to_LCD(command);
}
void LCD_putchar( char c)
{
	send_data_to_LCD(c);
}
void LCD_print_hex4( uint8_t val )
{
	val=val&0x0f;
	if( val < 10 )
		send_data_to_LCD('0'+val);
	else
		send_data_to_LCD('A'+val-10);
}
void LCD_print_hex8( uint8_t val )
{
	LCD_print_hex4(val>>4);
	LCD_print_hex4(val);
}
void LCD_print_hex16( uint16_t val )
{
	LCD_print_hex8( val>>8 );
	LCD_print_hex8( val );
}
void LCD_print_hex32( uint32_t val )
{
	LCD_print_hex16( val>>16 );
	LCD_print_hex16( val );
}