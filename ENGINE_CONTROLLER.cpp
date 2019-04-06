#include <avr/interrupt.h>
#include <avr/io.h>
#include <math.h>

#include "ENGINE_CONTROLLER.h"
#include "bitMacros.h"


int TR[7]; //Параметры гироскопа

int8_t	FW_BK = 1; //Флаг прямого или обратного счета 1 - прямой 0 - обратный

#define ENGINE_FR  20 //Частота импульсов в Гц
#define LEN_reg 8
#define Ndel 1   //Коэффицент деления
#define OCRReg  F_CPU/(Ndel*pow(2,(LEN_reg+2))*ENGINE_FR)-1

ENGINE_CONTROLLER_8::ENGINE_CONTROLLER_8(uint8_t Num)
{
	engines = Num;
	OCR2A = OCRReg;
	TCCR2B = _BV(CS20);
	TIMSK2 |= _BV(OCIE2A);
	FW_BK = 1;
	/*
	OCR1AH = 0;
	OCR1AL = 10;
	TCCR1B = _BV(CS10) | _BV(CS11);
	TIMSK |= _BV(OCIE1A);
	*/
}

uint8_t		INTEGRATOR;


void ENGINE_CONTROLLER_8::ENGINE_contr(void)
{
	ENGINE_FW = (~ENGINE_BW) & StartFw & OUT_PWM_ENG;
	ENGINE_BW = (~ENGINE_FW) & StartBw & OUT_PWM_ENG;
}

void ENGINE_CONTROLLER_8::EN_contrSigSel(void)
{
	StartFw = ((~AutoManSel) & StartFwRS232) | (AutoManSel & StartFwAuto);
	StartBw = ((~AutoManSel) & StartBwRS232) | (AutoManSel & StartBwAuto);
	FwHis = (StartFw)|(~BwHis);	
	BwHis = (StartBw)|(~FwHis);

}

void ENGINE_CONTROLLER_8::PWMmodeSw(void)
{
	OUT_PWM_ENG = ((~PWMmodeOnOff) & 0xFF) | (PWMmodeOnOff & OUT_PWM);
}

void ENGINE_CONTROLLER_8::PWMSelector(void)
{
	for (int i = 0; i < NUM_ENG; i++)
	{
		if (TST_BIT(AutoManSel,i))
		{
			 EnginePW[i]=PWM_setIntern[i];
		}
		else
		{
			EnginePW[i]=PWM_setRs232[i];
		}	
	}	
}

void ENGINE_CONTROLLER_8::Modulcomp(void)
{
	for (int i = 0; i < NUM_ENG; i++)
		{
			if (EnginePW[i] > INTEGRATOR)
			{
				SET_BIT(OUT_PWM,i);
			}
			else
			{
				CLR_BIT(OUT_PWM,i);
			}
		}
}

void ENGINE_CONTROLLER_8::AUTO_block()
{
#ifdef  P_REG
	for (int i = 0 ; i < NUM_ENG ; i++)
		{
			if (TST_BIT(AutoManSel,i))
			{
			
					if (i == 5 )
					{
						reg_er[i] = TR[1]>>Koef_lev;
					}
					else
					{
						reg_er[i]=EnginePos[i]-EnginePosSet[i];
					}
					// логическое-релейное регулирование
					/*
					if (reg_er[i]==0) {CLR_BIT(StartFwAuto,i); CLR_BIT(StartBwAuto,i);}
					if (reg_er[i]>0)  {SET_BIT(StartFwAuto,i); CLR_BIT(StartBwAuto,i);}
					if (reg_er[i]<0)  {SET_BIT(StartBwAuto,i); CLR_BIT(StartFwAuto,i);}
					*/
					//PID функция
					OutPIDLev[i] = Kcoef[i]*reg_er[i];

				
					if (OutPIDLev[i]<0) variable = -OutPIDLev[i];
					else variable = OutPIDLev[i];

					if (variable < def_min_lev) 
					{
						variable = def_min_lev;
					}
					if (variable > def_max_lev	) variable = def_max_lev;				
					//
					PWM_setIntern[i] = variable;

					if (OutPIDLev[i]==0) {CLR_BIT(StartFwAuto,i); CLR_BIT(StartBwAuto,i);}
					if (OutPIDLev[i]>0)  {SET_BIT(StartFwAuto,i); CLR_BIT(StartBwAuto,i);}
					if (OutPIDLev[i]<0)  {SET_BIT(StartBwAuto,i); CLR_BIT(StartFwAuto,i);}				
			}
		}
#else
	
	for (int i = 0 ; i < NUM_ENG ; i++)
		{
			if (i == 5)
			{
					reg_er[i] = TR[1]>>Koef_lev;
					OutPIDLev[i] = Kcoef[i]*reg_er[i];

				
					if (OutPIDLev[i]<0) variable = -OutPIDLev[i];
					else variable = OutPIDLev[i];

					if (variable < def_min_lev) 
					{
						variable = def_min_lev;
					}
					if (variable > def_max_lev	) variable = def_max_lev;				
					//
					PWM_setIntern[i] = variable;

					if (OutPIDLev[i]==0) {CLR_BIT(StartFwAuto,i); CLR_BIT(StartBwAuto,i);}
					if (OutPIDLev[i]>0)  {SET_BIT(StartFwAuto,i); CLR_BIT(StartBwAuto,i);}
					if (OutPIDLev[i]<0)  {SET_BIT(StartBwAuto,i); CLR_BIT(StartFwAuto,i);}		
			}
			else
			{
				switch (AUTOMAT_st[i])
				{				
					case 0:
							if (TST_BIT(AutoManSel,i))	
							{		
													
								reg_er[i]=EnginePos[i]-EnginePosSet[i];											
								if (reg_er[i]==0) {CLR_BIT(StartFwAuto,i); CLR_BIT(StartBwAuto,i);
								AUTOMAT_st[i]=5;}
								if (reg_er[i]>0)  {SET_BIT(StartFwAuto,i); CLR_BIT(StartBwAuto,i);
								PWM_setIntern[i] =  def_max_lev;
								AUTOMAT_st[i]=1;
								}
								if (reg_er[i]<0)  {SET_BIT(StartBwAuto,i); CLR_BIT(StartFwAuto,i);
								PWM_setIntern[i] =  def_max_lev;	
								AUTOMAT_st[i]=2;
								}
							}
							else
							{
								AUTOMAT_st[i]=5;
							}
						break;	
					case 1:							
							reg_er[i]=EnginePos[i]-EnginePosSet[i];
						
								if (reg_er[i]<=0) 
								{
									CLR_BIT(StartFwAuto,i); CLR_BIT(StartBwAuto,i);								 
									AUTOMAT_st[i]=5;
								}
						break;				
					case 2:								
								reg_er[i]=EnginePos[i]-EnginePosSet[i];
						
								if (reg_er[i]>=0) 
								{
									CLR_BIT(StartFwAuto,i); CLR_BIT(StartBwAuto,i);
									AUTOMAT_st[i]=5;
								}
						
						break;
					case 3:
							
								reg_er[i]=EnginePos[i]-EnginePosSet[i];							
								if (reg_er[i]<=0) 
								{
									CLR_BIT(StartFwAuto,i); CLR_BIT(StartBwAuto,i);
									PWM_setIntern[i] = 0;
									AUTOMAT_st[i]=5;
								}					
						break;
					case 5:
						break;
					default:	
					break;
				}
			}
		}		

#endif

}

void ENGINE_CONTROLLER_8::controller_Prg(void)
{
// 		ImpIntegr();
		Modulcomp();
		PWMSelector();
		PWMmodeSw();
		ENGINE_contr();
		EN_contrSigSel();
		AUTO_block();	
}



ISR(TIMER2_COMPA_vect ,ISR_BLOCK)
{
	TCNT2 = 0;
	INTEGRATOR+=FW_BK;
	
	if (INTEGRATOR == 0x00) FW_BK = 1;
	if (INTEGRATOR == 0xFF) FW_BK = -1;


}
