/*------------------------------------------------------------------------------
  lsm6dsl.c --
  
  Description:
    Brief description of file.
	  
	  Extended description, if appropriate.
  
-------------------------------------*/

/********************************DEPENDENCIES**********************************/

#include <avr/io.h>
#include "lsm6dsl.h"
#include "lsm6dsl_registers.h"
#include "usart.h"

/*****************************END OF DEPENDENCIES******************************/


/*****************************FUNCTION DEFINITIONS*****************************/

/* INSERT YOUR LSM6DSL FUNCTION DEFINITIONS BELOW. */

/*------------------------------------------------------------------------------
 LSM_write -- 
  
  Description:Writes a
  single byte of data, `data`, to the address `reg_addr`, which is
  meant to be associated with an LSM register.
    
  Input(s): N/A
  Output(s): N/A
---------------------------------------------------------------------------*/

/* 
 Write requires two SPI transmissions
 > First with R/~W first bit low for write and the next 7 bits the register address
 that you want to write to
 > For the second, write the data you want to send to that register
 */
void LSM_write(uint8_t reg_addr, uint8_t data){
	PORTF.OUTCLR = CS_bm; // set student select low
	// write requires two SPI transmissions
	// first write with first bit low to correspond write and next 7 bits the register address
	spi_write(reg_addr | LSM6DSL_SPI_WRITE_STROBE_bm);
	spi_write(data); 
	
	PORTF.OUTSET = CS_bm; // disable IMU
	
	
	
}

/*------------------------------------------------------------------------------
 LSM_read -- 
  
  Description: – Returns a single byte of
  data from an LSM register associated with the address
  `reg_addr`.
    
  Input(s): N/A
  Output(s): N/A
------------------------------------------------------------------------------*/

// also requires two SPI transmissions
// first send byte with first bit low for a read
uint8_t LSM_read(uint8_t reg_addr){
	uint8_t num;
	// change SS signal based on mode to initiate read
	PORTF.OUTCLR = CS_bm; // set student select low
	
	//Write the register address to tell the IMU which LSM register you want to read.
	spi_write(reg_addr | LSM6DSL_SPI_READ_STROBE_bm);
	
	//Read the data.
	num = spi_read();
	
	//Change the SS signal to stop the protocol.
	PORTF.OUTSET = CS_bm; // disable IMU
	
	return num;
	
	
  
	
	
	
}


/*------------------------------------------------------------------------------
 LSM_init -- 
  
  Description: – Initialize the IMU to do the following:
  1. perform software reset via CTRL3_C register 
  2. configure CTRL1_XL, CTRL9_XL, and INT1_CTRL
  3. configure an I/O port interrupt to trigger upon the accelerometer completing a measurement
    
  Input(s): N/A
  Output(s): N/A
------------------------------------------------------------------------------*/

void LSM_init(void){
	
	
	LSM_write(CTRL3_C, 0b00000101); // bit0 corresponds to SW_RESET bit. IF_INC = bit 2
	
	LSM_write(CTRL9_XL, 0b11110000); // bit4 is DEN_XL_G, rest are default values
	
	// ctrl1_XL full scale selection is automatically defaulted to +/- 2 g
	// change output data rate to 0101 which corresponds to 208 Hz
	LSM_write(CTRL1_XL, 0b01010000);
	
	LSM_write(INT1_CTRL, bit(0)); // enable accelerometer data ready interrupt
	
	// configure portC interrupt to trigger upon accelerometer completing a measurement
	PORTC.INT0MASK = bit(6); // select bit6 = INT1 as the interrupt source
	PORTC.DIRCLR = bit(6); // interrupt pin int1 is an input
	PORTC.INTCTRL = bit(1); // medium priority
	
	PMIC_CTRL = bit(1); // turn on medium level interrupts
	CPU_SREG = bit(7); // enable global interrupts
	
	
}

/*
Name: transmitToUSB
Description: Transmit a stream of sensor data via your USB Serial Port,
in the correct order according to the Simple Binary format

*/
void transmitToUSB(){
	usartd0_out_char(LSM_read(OUTX_L_XL));
	usartd0_out_char(LSM_read(OUTX_H_XL));
	usartd0_out_char(LSM_read(OUTY_L_XL));
	usartd0_out_char(LSM_read(OUTY_H_XL));
	usartd0_out_char(LSM_read(OUTZ_L_XL));
	usartd0_out_char(LSM_read(OUTZ_H_XL));
		
}


// INTERRUPT SERVICE ROUTINE
/* *********************************************************
* Name:     ISR(PORTD_INT1_vectt)
* Purpose:  Interrupt routine triggered during a accelerometer measurement completion on IMU.
*      
* Inputs:
* Output:
*************************************************************/
ISR(PORTC_INT0_vect){
	accel_flag = 1;		
}

/***************************END OF FUNCTION DEFINITIONS************************/
