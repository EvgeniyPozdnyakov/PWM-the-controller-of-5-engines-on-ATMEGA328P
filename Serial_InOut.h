/*
/
/
/ библиотека последовательного порта
/
*/


/*
* Посылка сообщения
*/

//#define 	UART_BAUD 9600
//#define 	F_CPU 16000000UL


#define		RX_BUFSIZE  100
#define		TX_BUFSIZE  100


#define  trans_Bs  			0 //Передатчик занят
#define  RessMess  			1 //Поступило сообщение
#define  MES_ERORR 			2 //Ошибка передачи
//#define  Command_mode 		3 //режим консольных строковых комманд
//#define  Code_comm_mode		4 //Режим укороченных комманд
//#define  Byte_map_dat_md	5 //Режим прямой передачи упр. сигналов

class USART_PORT
{
public:
		uint8_t Flag_trans;
		char bre[RX_BUFSIZE];
		char btr[TX_BUFSIZE];
		uint8_t br;
		uint8_t tr;
		bool flag_rCh;
		bool wifi_send;
 public:
 		USART_PORT(int bound);	
		void    RD_ress();
	//	USART_PORT();
	//	void	begin(int bound);
		int		Send_Mess(char* A);
		char	Get_serial_st();	
};

//extern USART_PORT serial1;




