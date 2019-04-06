#include <avr/io.h>
#include <util/twi.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include "bitMacros.h"
#include "TWI_text_lib.h"



void SendTWIMaster(uint8_t Adress, uint8_t* DATA,uint8_t length)
{
	if (!TST_BIT(TWI_ST_FLAG,DATA_TRANSM))
	{
		SET_BIT(TWI_ST_FLAG,DATA_TRANSM);
		TWI_WRITE_DATA_LEN = length;
		to_sla = Adress << 1;
		LPDATA = DATA;	
		TWCR = _BV(TWINT) |_BV(TWSTA) | _BV(TWEN) |_BV(TWIE); /* send start condition */	
	}
}

void TWI_StartMaster(void) //Инициализация TWI интерфейса в режиме мастера
{

  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
#if defined(TWPS0)
  /* has prescaler (mega128 & newer) */
  TWSR = 0;
#endif

#if F_CPU < 3600000UL
  TWBR = 10;			/* smallest TWBR value, see note [5] */
#else
  TWBR = (F_CPU / 100000UL - 16) / 2;
#endif
//TWCR = _BV(TWIE);
}

void TWI_StartSlave(uint8_t Adress) //Инициализация TWI интерфейса режиме ведомого
{

  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
#if defined(TWPS0)
  /* has prescaler (mega128 & newer) */
  TWSR = 0;
#endif

#if F_CPU < 3600000UL
  TWBR = 10;			/* smallest TWBR value, see note [5] */
#else
  TWBR = (F_CPU / 100000UL - 16) / 2;
#endif
TWAR = Adress << 1;
TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}


void TWI_Mast_Transm_mode(void)
{
PORTD = TWI_ST;
switch (TWI_ST)
	{
	  case TW_START: //Подтверждение старта 0x08
	    //Запись адреса
		 TWDR = to_sla | TW_WRITE;
		 //CLR_BIT(TWCR,TWINT);
		 TWCR = _BV(TWINT)|_BV(TWEN) | _BV(TWEN) | _BV(TWIE) ;
		  break;
	  case TW_MT_SLA_ACK: //подтверждение приема адреса 0x18
	    //Запись и последующая отправка данных
			TWDR = *LPDATA;
			LPDATA++;		
			TWI_WRITE_DATA_LEN--;
			TWCR =  _BV(TWINT)|_BV(TWEN) | _BV(TWIE) ;			
	    break;
	  case TW_MT_SLA_NACK: //Поддтверждение приема адреса не поступило 0x20
	  		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //Отправка СТОП сигнала	
			CLR_BIT(TWI_ST_FLAG,DATA_TRANSM);//Отправка данных ошибка
			SET_BIT(TWI_ST_FLAG,DATA_ERROR_TRANSM);
	  break;	
	  case TW_MT_DATA_NACK: //Посылка данных завершена прием не подтвержден 0x30
	  			TWCR =  (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	//Отправка СТОП сигнала
	    		CLR_BIT(TWI_ST_FLAG,DATA_ERROR_TRANSM);//Отправка данных завершена
	 
	  	break;  
	  case TW_MT_DATA_ACK: //подтверждение приема данных 0x28
	  		 	if (TWI_WRITE_DATA_LEN == 0)
				{
					TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWSTO)| _BV(TWIE); //Отправка СТОП сигнала	
					CLR_BIT(TWI_ST_FLAG,DATA_TRANSM);//Отправка данных ошибка
					SET_BIT(TWI_ST_FLAG,DATA_TRANSM);
				}
				else
				{
						TWDR = *LPDATA;
						LPDATA++;
						TWI_WRITE_DATA_LEN--;
						TWCR =  _BV(TWINT)|_BV(TWEN) | _BV(TWIE) ;
				}	  	
	    break;
	  case  TW_MT_ARB_LOST: //Потеря арбиража переходв из ведущего в ведомый   0x38 
	  	TWCR = (1<<TWEN)|(1<<TWSTO); //Отправка СТОП сигнала	
	  	break;
	  default:
	    TWCR = (1<<TWEN)|(1<<TWSTO); //Отправка СТОП сигнала	
	    break;
	}
}

void TWI_Mast_Ress_mode(void)
{

}

void TWI_Slave_Ress_mode(void)
{
PORTD = TWI_ST;
switch (TWI_ST)
	{
	  case   TW_SR_SLA_ACK : //Получен адрес данного устройства и требование чтения 0x60
	    TWI_WRITE_DATA_LEN = 6;
		TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);	
	    break;
	  case TW_SR_ARB_LOST_SLA_ACK: //подтверждение приема адреса 0x68
	   
	    break;
	  case   TW_SR_GCALL_ACK : //Поддтверждение приема адреса не поступило 0x70
	  	
	  break;	
	  case TW_SR_ARB_LOST_GCALL_ACK: //Посылка данных завершена прием не подтвержден 0x78
	  	
	  	break;  
	  case TW_SR_DATA_ACK: //подтверждение приема данных 0x80
	  	*LPDATA = TWDR;
		LPDATA++;
		TWI_WRITE_DATA_LEN--;
		if (TWI_WRITE_DATA_LEN==0)
			{TWCR =(1<<TWINT)|(1<<TWEN)|(1<<TWIE);}
		else 
		{TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);}
		
	    break;
	  case  TW_SR_DATA_NACK: //Потеря арбиража переходв из ведущего в ведомый   0x88 
	  
	   	break;
		case  TW_SR_GCALL_DATA_ACK:  // 0x90

		break; 
		case  TW_SR_GCALL_DATA_NACK: //  0x98 

		break;
		case TW_SR_STOP: //  0xA0 
		TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
		break;
	  default:
	  
	    break;
	}

}

void TWI_Slave_Transm_mode(void)
{
/*
switch (TWI_ST)
	{
	  case TW_START: //Подтверждение старта 0x08
	    //Запись адреса
		 TWDR = to_sla | TW_WRITE;
		 //CLR_BIT(TWCR,TWINT);
		 TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE) ;
		 SET_BIT(PORTD,0);
	    break;
	  case TW_MT_SLA_ACK: //подтверждение приема адреса 0x18
	    //Запись и последующая отправка данных
			TWDR = &LPDATA;
			LPDATA++;
			TWI_WRITE_DATA_LEN--;
			SET_BIT(PORTD,1);
	    break;
	  case TW_MT_SLA_NACK: //Поддтверждение приема адреса не поступило 0x20
	  		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //Отправка СТОП сигнала	
			CLR_BIT(TWI_ST_FLAG,DATA_TRANSM);//Отправка данных ошибка
			SET_BIT(TWI_ST_FLAG,DATA_TRANSM);
	  break;	
	  case TW_MT_DATA_NACK: //Посылка данных завершена прием не подтвержден 0x30
	  	if (TWI_WRITE_DATA_LEN == 0)
		{
			TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //Отправка СТОП сигнала	
			CLR_BIT(TWI_ST_FLAG,DATA_TRANSM);//Отправка данных ошибка
			SET_BIT(TWI_ST_FLAG,DATA_TRANSM);
		}
		else
		{
				TWDR = &LPDATA;
				LPDATA++;
				TWI_WRITE_DATA_LEN--;
		}
	  	break;  
	  case TW_MT_DATA_ACK: //подтверждение приема данных 0x28
	  		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	//Отправка СТОП сигнала
	    	CLR_BIT(TWI_ST_FLAG,DATA_TRANSM);//Отправка данных завершена
	    break;
	  case  TW_MT_ARB_LOST: //Потеря арбиража переходв из ведущего в ведомый   0x38 
	  	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //Отправка СТОП сигнала	
	  	break;
	  default:
	    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //Отправка СТОП сигнала	
	    break;
	}
	*/

}

#define Master

ISR(TWI_vect,ISR_BLOCK)
{
TWI_ST = TWSR;
#ifdef Master
TWI_Mast_Transm_mode();
#else
TWI_Slave_Ress_mode();
#endif
	
};

