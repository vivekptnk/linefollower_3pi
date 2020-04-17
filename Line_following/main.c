/* Line_following - an application for the Pololu 3pi Robot
 * Created: 4/17/2020 2:49:10 PM
 *  Author: vivek
 */

#include "lcd_driver.h"
#include "pwm_driver.h"
#include "reflect_driver.h"
#include <avr/io.h>


int main()
{
	configure_motors();
	configure_reflectance_sensors();
	
	while(1){
		configure_reflectance_sensors();
		
		if (read_reflectance_sensors() > 16) {
			pwm_right_motor(5);
			pwm_left_motor(35);
		} else if (read_reflectance_sensors()<2){
			pwm_right_motor(35);
			pwm_left_motor(5);
		} else {
			pwm_right_motor(25);
			pwm_left_motor(25);
		}
	}
	
	return 0;

}
