/* Line_following - an application for the Pololu 3pi Robot
 * Created: 4/17/2020 2:49:10 PM
 *  Author: vivek
 */

#include <pololu/3pi.h>

int main()
{
	play_from_program_space(PSTR(">g32>>c32"));  // Play welcoming notes.

	while(1)
	{
		// Print battery voltage (in mV) on the LCD.
		clear();
		print_long(read_battery_millivolts_3pi());

		red_led(1);     // Turn on the red LED.
		delay_ms(200);  // Wait for 200 ms.

		red_led(0);     // Turn off the red LED.
		delay_ms(200);  // Wait for 200 ms.
	}
}
