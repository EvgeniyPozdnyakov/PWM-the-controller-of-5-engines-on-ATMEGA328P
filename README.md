# PWM-the-controller-of-5-engines-on-ATMEGA328P
The project consists from 2х controllers Arduino NANO the code of the head controller Is presented.
Файл Start.cpp

#include "start.h"
#include "bitMacros.h"

#include "ENGINE_CONTROLLER.h" //Библиотека головного алгоблока
Программно описаны основные главные модули автоматики 
#include "Serial_InOut.h" //Библиотека последовательного порта
#include "TWI_text_lib.h" // Библиотека двух проводного интерфейса
#include "RecCommand.h" //Библиотека распознавания команд
#include "WIFI_modul.h" //Библиотека WI-FI модуля



ENGINE_CONTROLLER_8 engine_mod = ENGINE_CONTROLLER_8(5);
USART_PORT serial_port = USART_PORT(9600);
TWIinterface TWI = TWIinterface(false,0,0);
REC_command Decode(&engine_mod,&serial_port,&TWI);

Ф-ция инициализации
void setup()
{
	//Инициализация портов В/В
	DDRD = 0b11111100;
	DDRB = 0b00001111;
	
	set_serial(&serial_port);

	serial_port.Send_Mess("AT+RST\r\n"); //Сброс wi-fi модуля

	
 
	Начальная установка сигналов головного алгоблока

	engine_mod.PWMmodeOnOff = 0xFF;
	…..
	engine_mod.PWM_setRs232[7] = St_PWM_LEW;
	}

void loop()
{
/////////////////////////Блок проверки поступления сообщений из блока коммуникаций		
  if (TST_BIT(serial_port.Flag_trans,RessMess))
	{			
		 DATA_rec(); //Автомат WI-FI модуля
		
		 serial_port.RD_ress();//Перевод порта в режим готовности для приема
		следующего пакета
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	engine_mod.controller_Prg();   Цикл головного алгоблока
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Decode.Control_message();	Декодирование сообщения и выполнение команды
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
/// Передача сигналов управления из головного алгоблока на выводы контроллера	
if (!TST_BIT(engine_mod.ENGINE_FW,0)) SET_BIT(PORTD,2); else CLR_BIT(PORTD,2);.
if (!TST_BIT(engine_mod.ENGINE_BW,0)) SET_BIT(PORTD,3); else CLR_BIT(PORTD,3);
………………………………………
if (!TST_BIT(engine_mod.ENGINE_BW,4)) SET_BIT(PORTB,3); else CLR_BIT(PORTB,3);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      
	
			
}


Модуль "WIFI_modul.h" "WIFI_modul.cpp"

bool send_toIp(char* DATA,char AT);  //Отправка сообщения по wi-fi каналу
посылка комманды "AT+CIPSEND=" перевод автомата в режим отправки сообщения Send_dataRD

bool transmeetIP(void);// автомат отправки сообщений
Автомат отправки сообщения
Текущее
состояние	Следующее состояние	Описание состояния
Send_dataRD	Send_data	Ожидание сообщения «OK\r\n»
Подтверждение готовности передатчика
Send_data	Wait_send_data	Отправка сообщения
Переход в режим подтверждения отправки сообщения
Wait_send_data		Ожидание сообщения «SEND OK\r\n»
Завершение работы автомата
Перевод автомата wi-fi передатчика в режим TransmeetData


void DATA_rec(void); //Автомат работы wi-fi передатчика WIFIESP8266
Состояния автомата WI-FI модуля функция DATA_rec();
Автомат реагирует на поступающие сообщения последовательного порта контроллера
Текущее
состояние	Следующее состояние	Описание состояния
0	1	Ожидание сообщения «WIFI CONNECTED\r\n»
1	2	Ожидание сообщения «WIFI GOT IP\r\n»
Действие: отправка команды «AT+CIPMUX=1\r\n»
2	3	Ожидание сообщения «OK\r\n»
Действие: отправка команды «AT+CIPSERVER=1,665\r\n»
Назначаем порт IP 665
3	4	Ожидание сообщения «OK\r\n»
Действие: отправка команды «AT+CIPSTO=300\r\n»
Назначаем порт время бездействия открытого порта до разрыва соединения 3мин
4	disconn	Ожидание сообщения «OK\r\n»
Переход в режим ожидания соединения
disconn	connected	Ожидание сообщения «0,CONNECT\r\n»
Отправка сообщения «hello»
connected	connected
или 
disconn	Ожидание сообщения «0,CLOSED\r\n» или «+IPD,» начало заголовка принимаемого сообщения
Действие: при сообщении «0,CLOSED\r\n» переход в состояние 
disconn
при «+IPD,…» преобразование сообщения в команду последовательного порта. Автомат остается в текущем состоянии.
TransmeetData		Перевод автомата в режим передачи сообщения
Производится функцией bool send_toIp(char* DATA,char AT)
Возвращается в состояние connected


Модуль "WIFIConverter.h" "WIFIConverter.cpp"
bool Convert(char ch[]) //Отчистить строку от заголовка поступившего сообщения
(преобразовать строку в комманду последовательного интерфейса)
char* get_outStr(void) //Вернуть последнюю преобразованную строку


Модуль "LibDecode.h" "LibDecode.cpp"
Для формата команды «ABC,1,2,3;»
ABC заголовок определяющий код команды 1,2,3 – аргументы комманды

#define CommCNT		17  //Количество команд в массиве команд
//массив комманд 1-трехзначный буквенный код 2-код коммнды
static COMMANDS Comm[CommCNT] = {	
		"GeP",0x45,		//1
		"SeP",0x46,		//2
………………………………………………….
		"TWI",0x61		//17

};

char decode(char* myString) //Получить код комманды из строки
char* get_cmd_val(void);
char Get_last_codeCmd(void); //Получить код последней декодированной комманды
char Get_last_Nums(void); //Получить число аргументов комманды
int get_variable(char num); Получить значение аргумента комманды
char get_variableBYTE(char num);
