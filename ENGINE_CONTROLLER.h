#include <inttypes.h>

#define _BV(bit) \
	(1 << (bit)) 


#define NUM_ENG		5 //����� ���������� � �������
//int TR[7]; //��������� ���������

/*
* ����� ������ ���������� ������ ������� � ���������
* �������������� ���������� ��� (PID) ���������
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
*	���� ����������
* 	���� ������������ ��� ��������������� ���������������� ��������� ������
*	
*
*
*
*----------------------------------------------------------------------------------
*/


class ENGINE_CONTROLLER_8
{
	//������� ��������� �����
	uint8_t		engines;	
	public:
	uint8_t  ENGINE_FW;
	uint8_t  ENGINE_BW;

	uint8_t  StartFw;
	uint8_t  StartBw;

	uint8_t  OUT_PWM_ENG;
	uint8_t  OUT_PWM;

	uint8_t		AutoManSel; //������� - 1 ������ - 0
	uint8_t		PWMmodeOnOff;

	/*
	* ���������� ������� ���������� 
	* ������� ������
	*/
	uint8_t		StartFwRS232;
	uint8_t		StartBwRS232;

	uint8_t		StartFwAuto;
	uint8_t		StartBwAuto;


	uint8_t		EnginePW[NUM_ENG]; 
	uint8_t 	PWM_setRs232[NUM_ENG];
	uint8_t 	PWM_setIntern[NUM_ENG];	


	uint8_t MIN_POW_EN; //����������� ������� ���


	int reg_er[NUM_ENG];     	//������ �������������	

	#define def_min_lev		110
	#define def_max_lev		210

	int OutPIDLev[8];   //����������� �����������
 
	int Kcoef[8];

	#define	Koef_lev	4

	int AUTOMAT_st[NUM_ENG];
	int TintegrCoef[NUM_ENG];
	int TdifCoef[NUM_ENG];
	int variable;
	uint8_t Start_reg,end_reg;
	uint8_t FwHis,BwHis;
	
	int EnginePosSet[NUM_ENG]; //�������� ��������
	int EnginePos[NUM_ENG];
	public:
			ENGINE_CONTROLLER_8(uint8_t Num);
			//���� ���������� �����������
			void controller_Prg(void);
///	private:
			void ENGINE_contr(void);
			void EN_contrSigSel(void);
			void PWMmodeSw(void);
			void PWMSelector(void);
			void Modulcomp(void);
			void AUTO_block();

};


