//Description: Communicating with the IMU


#include <xc.h>
#include "lsm6dsl.h"
#include "lsm6dsl_registers.h"
#include "spi.h"
#include "usart.h"


int main(void)
{
	// initialize the SPI
	spi_init();
	usartd0_init();
	LSM_init();

	while(1){
		if(accel_flag == 1){
			transmitToUSB();
			
			accel_flag = 0;
			
		
		
		}
	
	
	
	}
}



