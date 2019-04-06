
#define DATA_TRANSM		 	0
#define DATA_END_TRANSM		1
#define DATA_ERROR_TRANSM	2
#define DATA_READED			3





class TWIinterface
{
public:
	uint8_t TWI_ST_FLAG; //Состояние TWI модуля
	uint8_t TWI_ST; //Состояние TWI модуля
	uint8_t TWI_WRITE_DATA_LEN; //Число передаваемых  звгружаемых байт 
	uint8_t* LPDATA; //Указатель местоположения данных
	uint8_t to_sla; //Адрес ведомого устройства
	uint8_t Len_BYTE; //Длинна сообщения для ведомого устройства
public:
	TWIinterface(bool type, uint8_t addres, uint8_t len_b);

	void TWI_StartMaster(void);//Инициализация TWI интерфейса
	void TWI_StartSlave(uint8_t Adress);

	void SendTWIMaster(uint8_t Adress, uint8_t* DATA, uint8_t length); //Отправка данных

	void TWI_Slave_Transm_mode(void);
	void TWI_Mast_Ress_mode(void);
	void TWI_Mast_Transm_mode(void);
	void TWI_Slave_Ress_mode(void);
};

/*
STAT	//Начало приема данных
Int	ret
SLA+W  //Адрес приемника
Int ASK //Преравание подтверждение приемника -->Ответ
//----------------Цыкл передачи данных начало----------------------------
DATA //Отправка N-байтов данных
Int ASK //Преравание подтверждение приема байта данных -->Ответ
//----------------Цыкл передачи данных конец----------------------------
STOP	//Завершение приема данных

*/


/*
TWSR //Регистр статуса

TWDR //Регистр данных

TWA //Адресный регистр ведомого
*/



