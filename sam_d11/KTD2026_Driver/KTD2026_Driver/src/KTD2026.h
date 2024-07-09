/*
 * KTD2026.h
 *
 * Created: 7/3/2024 10:47:05 AM
 *  Author: jatan
 */ 


#ifndef KTD2026_H_
#define KTD2026_H_



#define KTD2026_DEVICE_ADDRESS			0x30		// pg. 17 of Data sheet
#define DATA_LENGTH_FOR_JOB_X			8
#define REGISTER_ADDR					0x32



// http://blog.janjongboom.com/2015/12/01/asf-i2c.html


static uint8_t WriteBuffer_FOR_JOB_X[DATA_LENGTH_FOR_JOB_X]; // WriteBuffer_FOR_JOB_X = {0xREGISTER_TO_BE_WRITTEN, 0xDATA}























/*

Write Condition:



Start - Device Addr -         '0'		  - Ack (SDA LOW) - Register Address - Ack (SDA LOW) -	  Data -  Ack (SDA LOW) - Stop
		<7 bits>	  <Write command bit>						<8 bits>						<8 bits>


*/




void function_to_do_something_by_writing_data_to_a_register(void);









/*

R Condition:



Start - Device Addr -         '1'		  - Ack (SDA LOW) - Register Address - Ack (SDA LOW) -	  Data -  Ack (SDA HIGH) - Stop
		<7 bits>	  <Read command bit>						<8 bits>						<8 bits>	<ignore>


*/







/*

Power Saving / Sleep mode

When the KTD2026/KTD2027 is not driving LEDs, for example when all LEDs are off, the driver current
consumption can be set to “zero current” by putting the device into shutdown or sleep mode.

For the KTD2026/KTD2027, it is possible to force the device in sleep mode as long as there is no activity on the
SDA line by writing the Enable Control mode register Reg0[4:3] = 01 to select the “SCL=High & SDA Toggling”
mode

To put to sleep : write 01 on Reg0[4:3]		// Register 0, bits 4 and 3
to wake up: write 00 on Reg0[4:3]		


The following sequence shows an example where LED3 is flashing initially, then the device is set to sleep mode,
then the part is restarted.

LED3 (blue) is flashing. KTD2026 VIN pin current (IIN) = 260µA typical.
To enter sleep mode: Write Enable Control mode register Reg0[4:3] = 01, for mode “SCL=High & SDA Toggling”.
LEDs are now off. IIN = “zero” when there is no activity on the SDA line.

To restart the driver: Write Enable Control mode register Reg0[4:3] = 00, for mode “SCL & SDA High”. LED3
(blue) is flashing. IIN = 260µA

*/












#endif /* KTD2026_H_ */