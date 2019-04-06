////


#include <setjmp.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bitMacros.h"
#include "Serial_InOut.h"

#include "ENGINE_CONTROLLER.h"
#include "LibDecode.h"
#include "TWI_text_lib.h"
#include "RecCommand.h"

#define 	wifi

#ifdef wifi
#include "WIFI_modul.h"
#include "WIFIConverter.h"
#endif


#define Pos_lim		200
#ifdef wifi
REC_command::REC_command(ENGINE_CONTROLLER_8 *ControllerEN, USART_PORT *SERIAL_PORT, TWIinterface *TWI_port)
{
	ENGINE_control = ControllerEN;	
	SERIAL_PORT_IN = SERIAL_PORT;
	TWI = TWI_port;		
}
#else
REC_command::REC_command(ENGINE_CONTROLLER_8 *ControllerEN, USART_PORT *SERIAL_PORT, TWIinterface *TWI_port)
{
	ENGINE_control = ControllerEN;
	SERIAL_PORT_IN = SERIAL_PORT;
	TWI = TWI_port;		
}
#endif


void REC_command::MessageOK()
{
	
	#ifdef wifi
		send_toIp("Command executed \r\n\0",0);
	#else
		SERIAL_PORT_IN->Send_Mess("Command executed \n\r\0");
	#endif

	//Mem_buff->MEM_init();
}
void REC_command::MessageERR()
{
	#ifdef wifi
		send_toIp("Command is not identified \r\n\0",0);
	#else
		SERIAL_PORT_IN->Send_Mess("Command is not identified \n\r\0");
	#endif
}

char ReturnStr[10];
char NunStr[10];
uint8_t ks[7] = "123456";
char p;

void(* resetFunc) (void) = 0;

int REC_command::DECODE_SERIAL(void)
{	
#ifdef wifi
		Comp = decode(get_outStr());
#else
		Comp = decode(SERIAL_PORT_IN->bre);
#endif
		if (Comp == 0)
		{
			MessageERR();
#ifdef wifi

#else			
			SERIAL_PORT_IN->RD_ress();
#endif
			return 0;
		}
		switch (Comp)
		{
			case 0x45: //Запрос позиции оси двигателя GeP,N; Get engine position
				if (Get_last_Nums()==1)
				{					
					if(get_variable(0)< NUM_ENG)
					{ 
					strcat(ReturnStr,"Pos,");
					itoa(ENGINE_control->EnginePos[get_variable(0)],NunStr,10);
					strcat(ReturnStr,NunStr);
					strcat(ReturnStr,";");
#ifdef wifi
					send_toIp(ReturnStr,0);
#else
					SERIAL_PORT_IN->Send_Mess(ReturnStr);
#endif
					}
					else
					MessageERR();				
				}
				else 	MessageERR();
			break;
			case 0x46: //задать позицию оси двигателя SeP,N,V; Set engine position
				if (Get_last_Nums()==2)
				{					
					if((get_variable(0)< NUM_ENG)&&(abs(get_variable(1))<=Pos_lim))
					{ 
						ENGINE_control->EnginePosSet[get_variable(0)] = get_variable(1);
						MessageOK();
					}
					else
					MessageERR();
				}
				else 	MessageERR();			
			break;
			case 0x47: //Обнулить позицию двигателя ZeP,N; Zerro engine position
				if (Get_last_Nums()==1)
				{					
					if((get_variable(0)< NUM_ENG))
					{ 
						ENGINE_control->EnginePosSet[get_variable(0)] = ENGINE_control->EnginePos[get_variable(0)];
						MessageOK();
					}
					else
					MessageERR();
					
				}
				else 	MessageERR();
			break;
			case 0x48: //Задать уровень ШИМ SPM,N,V; Set pulse width modulation
				if (Get_last_Nums()==2)
				{					
					if((get_variable(0)< NUM_ENG)&&(get_variable(1)<=255)&&(get_variable(1)>=0))
					{ 
						ENGINE_control->PWM_setRs232[get_variable(0)] = get_variable(1);
						MessageOK();
					}
					else
					MessageERR();
				}
				else 	MessageERR();		
			break;
			case 0x49: //Включить ШИМ PWH,N;
				if (Get_last_Nums()==1)
				{					
					if((get_variable(0)< NUM_ENG))
					{ 
						SET_BIT(ENGINE_control->PWMmodeOnOff,get_variable(0));
						MessageOK();
					}
					else
					MessageERR();
				}
				else 	MessageERR();
			break;
			case 0x50: //Выключить ШИМ PWL,N;
				if (Get_last_Nums()==1)
				{					
					if((get_variable(0)< NUM_ENG))
					{ 
						CLR_BIT(ENGINE_control->PWMmodeOnOff,get_variable(0));
						MessageOK();
					}
					else
					MessageERR();
				}
				else 	MessageERR();
			break;
			case 0x51: //пуск двигателя вперед SeF,N;
				if (Get_last_Nums()==1)
				{					
					if((get_variable(0)< NUM_ENG))
					{ 						
						CLR_BIT(ENGINE_control->StartBwRS232,get_variable(0));
						SET_BIT(ENGINE_control->StartFwRS232,get_variable(0));
						MessageOK();
					}
					else
					MessageERR();
				}
				else 	MessageERR();
			break;
			case 0x52: //пуск двигателя назад  SeB,N;
				if (Get_last_Nums()==1)
				{					
					if((get_variable(0)< NUM_ENG))
					{ 					
						CLR_BIT(ENGINE_control->StartFwRS232,get_variable(0));
						SET_BIT(ENGINE_control->StartBwRS232,get_variable(0));
						MessageOK();
					}
					else
					MessageERR();
				}
				else 	MessageERR();
			break;
			case 0x53: //остановка двигателя StE,N;
				if (Get_last_Nums()==1)
				{					
					if((get_variable(0)< NUM_ENG))
					{ 					
						CLR_BIT(ENGINE_control->StartFwRS232,get_variable(0));
						CLR_BIT(ENGINE_control->StartBwRS232,get_variable(0));
						MessageOK();
					}
					else
					MessageERR();
				}
				else 	MessageERR();
			break;
			case 0x54: //остановка двигателя Mmd,N;
				if (Get_last_Nums()==1)
				{					
					if((get_variable(0)< NUM_ENG))
					{ 						
						CLR_BIT(ENGINE_control->AutoManSel,get_variable(0));
						MessageOK();
					}
					else
					MessageERR();
				}
				else 	MessageERR();
			break;
			case 0x55: //остановка двигателя Amd,N;
				if (Get_last_Nums()==1)
				{					
					if((get_variable(0)< NUM_ENG))
					{ 					
						SET_BIT(ENGINE_control->AutoManSel,get_variable(0));						
						MessageOK();
					}
					else
					MessageERR();
				}
				else 	MessageERR();
			break;
			case 0x56: //Перезапуск контроллера rCP;
				resetFunc;		
			break;
			case 0x57: //Перезапуск контроллера rCP;
				resetFunc;		
			break;
			case 0x58: //Перезапуск контроллера rCP;
				resetFunc;		
			break;
			case 0x59: //Перезапуск контроллера rCP;
				resetFunc;		
			break;
			case 0x61: //Перезапуск контроллера TWI;
				if ((Get_last_Nums()<=6)&&(Get_last_Nums()>=0))
				{					
					
					memset(ks,0x00,6);
					for ( char jk = 0;  jk < 6; jk++)
					{
						
						p = get_variableBYTE(jk);
						
						if ((p>=0)&&(p<=0xFF))
						{						
							ks[jk] = p;
							
						}
						else
						{
						MessageERR();
						break;
						}	
					

					}					
					TWI->SendTWIMaster(0x12, ks,6);
					MessageOK();
				}
				else 	MessageERR();		
			break;
		}
		/*
							"GeP",0x45,		//1
							"SeP",0x46,		//2
							"ZeP",0x47,		//3
							"SPW",0x48,		//4
							"PWH",0x49,		//5
							"PWL",0x50,		//6
							"SeF",0x51,		//7
							"SeB",0x52,		//8
							"StE",0x53,		//9
							"Mmd",0x54,		//10
							"Amd",0x55,		//11
							"rCP",0x56,		//12
							"MFw",0x57,		//13
							"MBk",0x58,		//14
							"MRp",0x59,		//15
							"MLp",0x59,		//15
							"TWI",0x60		//16
	 */
		
	#ifndef wifi
		SERIAL_PORT_IN->RD_ress();
	#endif
}
int REC_command::DECODE_TWI(void)
{
	return 0;
}

int REC_command::Control_message(void)
{
#ifdef wifi	
	if(Get_flag())
	{
		clear_mess();
		this->DECODE_SERIAL();
	}
#else
	if(TST_BIT(SERIAL_PORT_IN->Flag_trans,RessMess))
	{
		CLR_BIT(SERIAL_PORT_IN->Flag_trans,RessMess);
		this->DECODE_SERIAL();	
	}
#endif
	if(TST_BIT(TWI->TWI_ST_FLAG,DATA_READED))
	{
		CLR_BIT(TWI->TWI_ST_FLAG,DATA_READED);
		this->DECODE_TWI();	
		
	
	}	
	return 0;
}



