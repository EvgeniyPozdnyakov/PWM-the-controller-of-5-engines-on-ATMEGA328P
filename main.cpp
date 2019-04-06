//
//
//
#include <avr/io.h>
#include <avr/interrupt.h>
#include "start.h"




int main()
{
	setup();
	sei();
	for(;;)
	{
		loop();
	}
	return 0;
}
