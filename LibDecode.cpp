#include "LibDecode.h"
#include <string.h>
#include <stdlib.h>
char codeWR[2] = {'W','R'};
char codeRD[2] = {'R','D'};
char command_cd;
int num;
struct COMMANDS
{
	char comm[4];
	char code;
};
#define CommCNT		17
static COMMANDS Comm[CommCNT] = {	
							"GeP",0x45,		//1
							"SeP",0x46,		//2
							"ZeP",0x47,		//3
							"SPW",0x48,		//4
							"PWH",0x49,		//5
							"PWL",0x50,		//6
							"SeF",0x51,		//7
							"SeB",0x52,		//8
							"StE",0x53,		//9
							"Mmd",0x54,		//10
							"Amd",0x55,		//11
							"rCP",0x56,		//12
							"MFw",0x57,		//13
							"MBk",0x58,		//14
							"MRp",0x59,		//15
							"MLp",0x60,		//16
							"TWI",0x61		//17

};



char codeINP[CODES][STR_LEN];


char decode(char* myString)
{
	command_cd = 0x00;
	int cmd_cnt = 0;
	int i;
	num = 0;
	do
	{
		if (memcmp(myString,Comm[cmd_cnt].comm,3)==0)
		{
			command_cd	= Comm[cmd_cnt].code;
			i=3;
			goto Exit_comm;
		};
		cmd_cnt++;
	}
	while (cmd_cnt<CommCNT);
	if (memcmp(myString,codeWR,2)==0)
	{
		command_cd	= 0x01;
		
	};
	if (memcmp(myString,codeRD,2)==0)
	{
		command_cd	= 0x02;
	};

	if (command_cd == 0) return 0;
		i = 2;	
Exit_comm:	
		num = 0;
			
		if (myString[i] != ',')	
			return 0;
	
		do
		{		    
			memset(codeINP[num],0x00,10);
			int ps = 0;
			i++;
			while ((myString[i] != ',')&&(myString[i] != ';'))
			{
				if((myString[i]>=30)||(myString[i]<=39)||(myString[i] == '.'))
				{
					codeINP[num][ps] = myString[i]; ps++;
				}
				else
					return 0;
				if (ps>=STR_LEN) return 0;
				i++;
			}
			
			num++;
			if (num>=CODES) return 0;
		}while ((myString[i] != ';')&&(i<BUF_LEN));

	
	return command_cd;
}

char Get_last_codeCmd(void)
{
	return command_cd;
}

char Get_last_Nums(void)
{
	return num;
}

int get_variable(char num)
{
	return atoi(codeINP[num]);
}

char get_variableBYTE(char num)
{
	return atoi(codeINP[num]);
}

char* get_cmd_val(void)
{
	return &codeINP[0][0];
}
