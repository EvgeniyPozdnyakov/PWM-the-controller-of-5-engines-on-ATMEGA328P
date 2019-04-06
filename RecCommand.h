

#define SET_SWITCH  0x10
#define OFF_SWITCH	0x20





class REC_command
{
		char Comp;
		USART_PORT *SERIAL_PORT_IN;
		ENGINE_CONTROLLER_8 *ENGINE_control;
		TWIinterface *TWI;
	public:	
		REC_command(ENGINE_CONTROLLER_8 *ControllerEN, USART_PORT *SERIAL_PORT, TWIinterface *TWI_port);		
		int Control_message(void);
		int DECODE_SERIAL(void);
		int DECODE_TWI(void);
		void MessageOK(); 
		void MessageERR();
};
