#include <avr/io.h>
#include <avr/interrupt.h>
#include <setjmp.h>
#include <string.h>
#include <stdio.h>



#include "start.h"
#include "bitMacros.h"
#include "ENGINE_CONTROLLER.h"
#include "Serial_InOut.h"
#include "TWI_text_lib.h"
#include "RecCommand.h"
#include "WIFI_modul.h"


	ENGINE_CONTROLLER_8 engine_mod = ENGINE_CONTROLLER_8(5);
	USART_PORT serial_port = USART_PORT(9600);
	TWIinterface TWI = TWIinterface(false,0,0);
	REC_command Decode(&engine_mod,&serial_port,&TWI);
		
//	MemoryCl	buffer;

	
	

//Минимальный уровень ШИМ
#define St_PWM_LEW 150

//static int as;
char		OUT_portA;
char		OUT_portC;

void setup()
{
	
	DDRD = 0b11111100;
	DDRB = 0b00001111;
//	DDRC = 0xFF;	
	
	//engine_mod_r = ENGINE_CONTROLLER_8(6);
	set_serial(&serial_port);
	serial_port.Send_Mess("AT+RST\r\n");

	
 
	
	engine_mod.PWMmodeOnOff = 0xFF;
	engine_mod.PWM_setRs232[0] = St_PWM_LEW;
	engine_mod.PWM_setRs232[1] = St_PWM_LEW;
	engine_mod.PWM_setRs232[2] = St_PWM_LEW;
	engine_mod.PWM_setRs232[3] = St_PWM_LEW;
	engine_mod.PWM_setRs232[4] = St_PWM_LEW;
	/*
	engine_mod.PWM_setRs232[5] = St_PWM_LEW;
	engine_mod.PWM_setRs232[6] = St_PWM_LEW;
	engine_mod.PWM_setRs232[7] = St_PWM_LEW;
	*/

	engine_mod.Kcoef[0] = 100;
	engine_mod.Kcoef[1] = 100;
	engine_mod.Kcoef[2] = 20;
	engine_mod.Kcoef[3] = 20;
	engine_mod.Kcoef[4] = 40;
	/*
	engine_mod.Kcoef[5] = 100;
	engine_mod.Kcoef[6] = 150;
	engine_mod.Kcoef[7] = 100;
	*/
	
	//serial_port.Send_Mess("Start CLAW system \n\r");

}

void loop()
{
	if (TST_BIT(serial_port.Flag_trans,RessMess))
	{			
		 DATA_rec();
		// serial_port.Send_Mess("Start CLAW system \r\n");
		 serial_port.RD_ress();
	}

	engine_mod.controller_Prg();

    Decode.Control_message();	
	
	//		OUT_portA = 0;
	//		OUT_portC = 0;
			if (!TST_BIT(engine_mod.ENGINE_FW,0)) SET_BIT(PORTD,2); else CLR_BIT(PORTD,2); //Привод лев.
			if (!TST_BIT(engine_mod.ENGINE_BW,0)) SET_BIT(PORTD,3); else CLR_BIT(PORTD,3);
			if (!TST_BIT(engine_mod.ENGINE_FW,1)) SET_BIT(PORTD,4); else CLR_BIT(PORTD,4); //Привод прав.
			if (!TST_BIT(engine_mod.ENGINE_BW,1)) SET_BIT(PORTD,5); else CLR_BIT(PORTD,5);
			if (!TST_BIT(engine_mod.ENGINE_FW,2)) SET_BIT(PORTD,6); else CLR_BIT(PORTD,6);
			if (!TST_BIT(engine_mod.ENGINE_BW,2)) SET_BIT(PORTD,7); else CLR_BIT(PORTD,7);
   			if (!TST_BIT(engine_mod.ENGINE_FW,3)) SET_BIT(PORTB,0); else CLR_BIT(PORTB,0);
			if (!TST_BIT(engine_mod.ENGINE_BW,3)) SET_BIT(PORTB,1); else CLR_BIT(PORTB,1);
			if (!TST_BIT(engine_mod.ENGINE_FW,4)) SET_BIT(PORTB,2); else CLR_BIT(PORTB,2);
			if (!TST_BIT(engine_mod.ENGINE_BW,4)) SET_BIT(PORTB,3); else CLR_BIT(PORTB,3);
      
			/*
			if (TST_BIT(engine_mod.ENGINE_FW,1)) SET_BIT(OUT_portA,2);
			if (TST_BIT(engine_mod.ENGINE_BW,1)) SET_BIT(OUT_portA,3);
			if (TST_BIT(engine_mod.ENGINE_FW,2)) SET_BIT(OUT_portA,4);
			if (TST_BIT(engine_mod.ENGINE_BW,2)) SET_BIT(OUT_portA,5);
			if (TST_BIT(engine_mod.ENGINE_FW,3)) SET_BIT(OUT_portA,6);
			if (TST_BIT(engine_mod.ENGINE_BW,3)) SET_BIT(OUT_portA,7);

			if (TST_BIT(engine_mod.ENGINE_FW,4)) SET_BIT(OUT_portC,7);
			if (TST_BIT(engine_mod.ENGINE_BW,4)) SET_BIT(OUT_portC,6);
			if (TST_BIT(engine_mod.ENGINE_FW,5)) SET_BIT(OUT_portC,5);
			if (TST_BIT(engine_mod.ENGINE_BW,5)) SET_BIT(OUT_portC,4);
			if (TST_BIT(engine_mod.ENGINE_FW,6)) SET_BIT(OUT_portC,3);
			if (TST_BIT(engine_mod.ENGINE_BW,6)) SET_BIT(OUT_portC,2);
			if (TST_BIT(engine_mod.ENGINE_FW,7)) SET_BIT(OUT_portC,1);
			if (TST_BIT(engine_mod.ENGINE_BW,7)) SET_BIT(OUT_portC,0);
			*/
	
			
//			PORTA = OUT_portA;
//			PORTC = OUT_portC;
	
	//PORTA = engine_mod.StartFw;
}
