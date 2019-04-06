#include <avr/io.h>
//#include <avr/interrupt.h>
#include <stdlib.h>
#include "bitMacros.h"
#include <string.h>
#include "Serial_InOut.h"
#include "WIFI_modul.h"
#include "WIFIConverter.h"

USART_PORT *LOCAL_PORT_p;

unsigned int FlagWiFi;
#define		TransmitData	0


unsigned 	int stateP,stateP_last;  //Автомтомат инициализации wifi модуля
unsigned	int StateSEND;

int Ln;
int statein;
char string[50];
char strAT[5];
char strLEN[10];
char data[50];
//char data1[50];

bool Ress_mess;

#define 	Ready_SEND_DATA		100  	//Готовность передавать данные по WIFI
#define 	Send_data			101		//Посылка данных
#define 	Send_dataRD			102
#define 	Wait_send_data		103		//Ожидание результатов отправки данных

#define 	disconn		  		500
#define 	connected	 		501
#define 	TransmeetData 		502

bool Get_flag(void)
{
	return Ress_mess;
}

void clear_mess(void)
{
	Ress_mess = false;
}

bool send_toIp(char* DATA,char AT)
{
			if(StateSEND!=Ready_SEND_DATA)
			return false;
				strcpy(data,DATA);
				Ln = strlen(DATA);				
				memset(string,0x00,sizeof(string));
				memset(strAT,0x00,sizeof(strAT));
				itoa(AT,strAT,10);
				itoa(Ln,strLEN,10);
				strcat(string,"AT+CIPSEND=");
				strcat(string,strAT);
				strcat(string,",");
				strcat(string,strLEN);
				strcat(string,"\r\n");
				LOCAL_PORT_p->RD_ress();//memset(StringInp,0,PortBytes); counter = 0;
				LOCAL_PORT_p->Send_Mess(string);
				StateSEND = Send_dataRD;
				stateP_last = stateP;
				stateP = TransmeetData;
			return true;
}

bool transmeetIP(void) //Автомат передачи сообщения по WI-FI каналу
{		
			switch (StateSEND)
			{
			case	Send_dataRD:
				if (strcmp(LOCAL_PORT_p->bre,"OK\r\n")==0)
				{
					LOCAL_PORT_p->wifi_send = false;
					StateSEND = Send_data;	
				}
			break;		
			case	Send_data:
					LOCAL_PORT_p->wifi_send = true;
					StateSEND = Wait_send_data;
					LOCAL_PORT_p->Send_Mess(data);			
				break;
			case	Wait_send_data:
				if (strcmp(LOCAL_PORT_p->bre,"SEND OK\r\n")==0)
				{
					StateSEND = Ready_SEND_DATA;
					stateP = stateP_last;					
					return true;					
				}
				break;
			default:
			break;
			}
			//memset(StringInp,0,PortBytes); counter = 0;
		
	
	return true;
}



void set_serial(USART_PORT *LOCAL_PORT_in)
{
	LOCAL_PORT_p = LOCAL_PORT_in;
	stateP=0;
	StateSEND=Ready_SEND_DATA;
	Ress_mess = false;
}



void DATA_rec(void) //Автомат работы модуля WIFIESP8266
{

		switch (stateP)
		{
			case 0:
				if (strcmp(LOCAL_PORT_p->bre,"WIFI CONNECTED\r\n")==0)
					{	
						
						stateP = 1;
					}
				break;
			case 1:
				if (strcmp(LOCAL_PORT_p->bre,"WIFI GOT IP\r\n")==0)
					{
						stateP = 2;						
						LOCAL_PORT_p->Send_Mess("AT+CIPMUX=1\r\n");
					}
				break;
			case 2:
				if (strcmp(LOCAL_PORT_p->bre,"OK\r\n")==0)
					{
						stateP = 3;						
						LOCAL_PORT_p->Send_Mess("AT+CIPSERVER=1,665\r\n");
					}
				break;
			case 3:
				if (strcmp(LOCAL_PORT_p->bre,"OK\r\n")==0)
					{
						stateP = 4;
						LOCAL_PORT_p->Send_Mess("AT+CIPSTO=300\r\n");
					}
				break;
			case 4:
				if (strcmp(LOCAL_PORT_p->bre,"OK\r\n")==0)
					{
						stateP = disconn;
						//LOCAL_PORT_p->Send_Mess("AT+SERVER=1,665\r\n");
					}
				break;
			case	connected:
				if ((strcmp(LOCAL_PORT_p->bre,"0,CLOSED\r\n")==0)||
							(strcmp(LOCAL_PORT_p->bre,"0,CONNECT FAIL\r\n")==0))
				{
					stateP = disconn;				
				}
				else
				{
					if (strncmp(LOCAL_PORT_p->bre,"+IPD,",5)==0)
					{
						//send_toIp("mess_ok",0);
						Convert(LOCAL_PORT_p->bre);
						Ress_mess = true;
					}
					//decode command					
					//send_toIp("mess_ok",0);
					//strcpy(data1,LOCAL_PORT_p->bre);
					//send_toIp(LOCAL_PORT_p->bre,0);
				}
				
				break;
			case	disconn:
				if (strcmp(LOCAL_PORT_p->bre,"0,CONNECT\r\n")==0)
				{
					stateP = connected;
					//SendData("AT+CIPSEND=0,665\r\n");
					//printf("connect system\r\n");
					send_toIp("hello",0);
				}				
				break;
			case TransmeetData:
				transmeetIP();
				break;	
			default:
				
				break;
		};
}
