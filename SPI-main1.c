
#include <xc.h>
#include "spi.h"


int main(void)
{
	spi_init();
	uint8_t num = 0xCE;
	
	while(1)
	{
		// set student select low
		PORTF.OUTCLR = SS_bm;
		spi_write(num);
		// set student select high
		PORTF.OUTSET = SS_bm;
		
		
	}
}