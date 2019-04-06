/*
Коммуникационные прерывания
*/

//Передача сообщений
#include <avr/io.h>
#include <avr/interrupt.h>
#include "bitMacros.h"
#include <string.h>
#include <inttypes.h>
#include "Serial_InOut.h"


USART_PORT *LOCAL_PORT;

USART_PORT::USART_PORT(int bound)
{
	tr=0; br = 0;
#if F_CPU < 2000000UL && defined(U2X)
 	UCSR0A = _BV(U2X);             /* improve baud rate error by using 2x clk */

	UBRR0L = (F_CPU / (8UL * bound)) - 1;
#else
  	UBRR0L = (F_CPU / (16UL * bound)) - 1;
#endif
	UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0) | _BV(TXCIE0); /* tx/rx enable */
	LOCAL_PORT = this;
	wifi_send = true;
}

/*
void USART_PORT::begin(int bound)
{

}
*/

void    USART_PORT::RD_ress() //Перевод в режим готовности для приема следующего пакета
{
		LOCAL_PORT->br=0;
		memset(LOCAL_PORT->bre,0x00,100);
		CLR_BIT(LOCAL_PORT->Flag_trans,RessMess);	
}


//Посылка сообщения
int USART_PORT::Send_Mess(char* A)
{
	if (!TST_BIT(Flag_trans,trans_Bs))
	{
	strcpy(this->btr,A);
	SET_BIT(Flag_trans,trans_Bs);
	tr = 1;
	UDR0 = btr[0];
	}
	return 0;
}

//Отправка сообщений
//USART_TX_vect  USART_TXC_vect Atmega16
ISR(USART_TX_vect,ISR_BLOCK)
{
	if (LOCAL_PORT->btr[LOCAL_PORT->tr] == '\0')
	{
		CLR_BIT(LOCAL_PORT->Flag_trans,trans_Bs);
		memset(LOCAL_PORT->btr,0x00,LOCAL_PORT->tr);		
	}
	else
	{
		UDR0 = LOCAL_PORT->btr[LOCAL_PORT->tr];
		LOCAL_PORT->tr++;
	}
}

//Прием сообщений
//USART_RX_vect  USART_RXC_vect Atmega16
ISR(USART_RX_vect,ISR_BLOCK)
{
	if (!TST_BIT(LOCAL_PORT->Flag_trans,RessMess))
	{
		LOCAL_PORT->bre[LOCAL_PORT->br] = UDR0;
		if (LOCAL_PORT->wifi_send)
		{
			if (!LOCAL_PORT->flag_rCh)
			{
				if (LOCAL_PORT->bre[LOCAL_PORT->br] == '\r')
				{
					LOCAL_PORT->flag_rCh = true;
					//SET_BIT(LOCAL_PORT->Flag_trans,RessMess);
				}
			}
			else
			{
				if ((LOCAL_PORT->bre[LOCAL_PORT->br] == '\n'))
				{				
					SET_BIT(LOCAL_PORT->Flag_trans,RessMess);
				}
				LOCAL_PORT->flag_rCh = false;
			}
		}
		else
		{
			if (!LOCAL_PORT->flag_rCh)
			{
				if (LOCAL_PORT->bre[LOCAL_PORT->br] == '>')
				{
					LOCAL_PORT->flag_rCh = true;
					//SET_BIT(LOCAL_PORT->Flag_trans,RessMess);
				}
			}
			else
			{
				if ((LOCAL_PORT->bre[LOCAL_PORT->br] == ' '))
				{				
					SET_BIT(LOCAL_PORT->Flag_trans,RessMess);
				}
				LOCAL_PORT->flag_rCh = false;
			}
		}		
		LOCAL_PORT->br++;
		if (LOCAL_PORT->br>RX_BUFSIZE)
		{
			LOCAL_PORT->br= 0;
			memset(LOCAL_PORT->bre,0,RX_BUFSIZE);
		}
	}
	

}

