/*------------------------------------------------------------------------------
  spi.c --
  
  Description:
    Provides useful definitions for manipulating the relevant SPI
    module of the ATxmega128A1U. 
  
  Author(s): Dr. Eric M. Schwartz, Christopher Crary, Wesley Piard
  Last modified by: Dr. Eric M. Schwartz
  Last modified on: 28 Feb 2024
------------------------------------------------------------------------------*/

/********************************DEPENDENCIES**********************************/

#include <avr/io.h>
#include "spi.h"

/*****************************END OF DEPENDENCIES******************************/


/*****************************FUNCTION DEFINITIONS*****************************/


void spi_init(void)
{
	// use falling edge, LSB,mode 2
  /* Initialize the relevant SPI output signals to be in an "idle" state. -> make /SS high 
   * Refer to the relevant timing diagram within the LSM6DSL datasheet.
   * (You may wish to utilize the macros defined in `spi.h`.) */

  PORTF.OUTSET = (MOSI_bm | SCK_bm); // MOSI and SCK are high -> Mosi is on, SCK is off

  /* Configure the pin direction of relevant SPI signals. */
  // 0b10110000
  PORTF.DIRSET = (SS_bm | MOSI_bm | SCK_bm);
  // 0b01000000
  PORTF.DIRCLR = MISO_bm;
	
  /* Set the other relevant SPI configurations. */
  // CLK2X, Enable, DORD, MASTER, MODE[1:0], PRESCALER[1:0]
  // Use Mode 3 since the IMU samples data at the rising edge and it is active in a low state
  SPIF.CTRL	=	SPI_PRESCALER_DIV16_gc		    |
					    SPI_MODE_3_gc         |
					    SPI_ENABLE_bm | 
						SPI_MASTER_bm |
						SPI_DORD_bm; // LSB

// no interrupt
  SPIF.INTCTRL = SPI_INTLVL_OFF_gc;
}

void spi_write(uint8_t data)
{
	/* Write to the relevant DATA register. */
	SPIF.DATA = data;
	
	/* Wait for relevant transfer to complete. */
	while(!(SPIF.STATUS & SPI_IF_bm));
	

  /* In general, it is probably wise to ensure that the relevant flag is
   * cleared at this point, but, for our contexts, this will occur the 
   * next time we call the `spi_write` (or `spi_read`) routine. 
   * Really, because of how the flag must be cleared within
   * ATxmega128A1U, it would probably make more sense to have some single 
   * function, say `spi_transceive`, that both writes and reads 
   * data, rather than have two functions `spi_write` and `spi_read`,
   * but we will not concern ourselves with this possibility
   * during this semester of the course. */
}

uint8_t spi_read(void)
{
  /* Write some arbitrary data to initiate a transfer. */
  SPIF.DATA = 0x37;

  /* Wait for relevant transfer to be complete. */
  while(!(SPIF.STATUS & SPI_IF_bm));

  /* After the transmission, return the data that was received. */
  return SPIF.DATA;
}

/***************************END OF FUNCTION DEFINITIONS************************/
