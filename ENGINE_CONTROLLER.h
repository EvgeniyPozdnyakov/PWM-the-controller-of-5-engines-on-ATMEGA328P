#include <inttypes.h>

#define _BV(bit) \
	(1 << (bit)) 


#define NUM_ENG		5 //Число двигателей в системе
//int TR[7]; //Параметры гироскопа

/*
* Выбор режима управления ручной внешний с терминала
* Автоматический внутренний ПИД (PID) регулятор
*/


/*
static uint8_t EngineFeedBk,EngineFeedBk1;

void ImpIntegr(void)
{
	for (int i = 0 ; i < 8 ; i++)
		{
			if ((TST_BIT(~EngineFeedBk,i))&&(TST_BIT(EngineFeedBk1,i)))
			{
		
				if (TST_BIT(ENGINE_BW,i)) EnginePos[i]++;
				if (TST_BIT(ENGINE_FW,i)) EnginePos[i]--;
			}
	
		}
	EngineFeedBk1 = EngineFeedBk;
}
*/


/*
*----------------------------------------------------------------------------------
*	Блок автоматики
* 	Блок предназначен для автоматического позиционирования механизма робота
*	
*
*
*
*----------------------------------------------------------------------------------
*/


class ENGINE_CONTROLLER_8
{
	//Базовые параметры блока
	uint8_t		engines;	
	public:
	uint8_t  ENGINE_FW;
	uint8_t  ENGINE_BW;

	uint8_t  StartFw;
	uint8_t  StartBw;

	uint8_t  OUT_PWM_ENG;
	uint8_t  OUT_PWM;

	uint8_t		AutoManSel; //Автомат - 1 Ручной - 0
	uint8_t		PWMmodeOnOff;

	/*
	* Дискретные команды управления 
	* ручного режима
	*/
	uint8_t		StartFwRS232;
	uint8_t		StartBwRS232;

	uint8_t		StartFwAuto;
	uint8_t		StartBwAuto;


	uint8_t		EnginePW[NUM_ENG]; 
	uint8_t 	PWM_setRs232[NUM_ENG];
	uint8_t 	PWM_setIntern[NUM_ENG];	


	uint8_t MIN_POW_EN; //Минимальный уровень ШИМ


	int reg_er[NUM_ENG];     	//Ошибка регулирования	

	#define def_min_lev		110
	#define def_max_lev		210

	int OutPIDLev[8];   //Управляющее воздействие
 
	int Kcoef[8];

	#define	Koef_lev	4

	int AUTOMAT_st[NUM_ENG];
	int TintegrCoef[NUM_ENG];
	int TdifCoef[NUM_ENG];
	int variable;
	uint8_t Start_reg,end_reg;
	uint8_t FwHis,BwHis;
	
	int EnginePosSet[NUM_ENG]; //Заданное значение
	int EnginePos[NUM_ENG];
	public:
			ENGINE_CONTROLLER_8(uint8_t Num);
			//Блок управления двигателями
			void controller_Prg(void);
///	private:
			void ENGINE_contr(void);
			void EN_contrSigSel(void);
			void PWMmodeSw(void);
			void PWMSelector(void);
			void Modulcomp(void);
			void AUTO_block();

};


