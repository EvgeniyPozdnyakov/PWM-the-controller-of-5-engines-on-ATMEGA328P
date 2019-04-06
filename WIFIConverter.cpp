#include <string.h>
#include <stdlib.h>
#include "WIFIConverter.h"


char OutSt[100];
char Dig1[5];
char Dig2[5];
int Dg1,Dg2;



bool charIsDigit(char a)
{
	if ((a<=0x39)&&(a>=0x30)) return true;
	return false;
}

bool Convert(char ch[]) //Отчистить строку от заголовка поступившего сообщения
{
	if (strncmp(ch,"+IPD,",5)!=0)
			return false;

		memset(Dig1,0,sizeof(Dig1));
		memset(Dig2,0,sizeof(Dig2));
		int i = 5;
		int k = 0;
		int p = strlen(ch);
		while ((charIsDigit(ch[i]))&&(i<p))
		{
			Dig1[k] = ch[i];
			k++; i++;
			
		}
		if (ch[i]!=',')
		{
			return false;
		}
		i++;
		k = 0;
		while ((charIsDigit(ch[i]))&&(i<p))
		{
			Dig2[k] = ch[i];
			k++; i++;
			
		}
		if (ch[i]!=':')
		{
			return false;
		}
		i++;
		Dg1 = atoi(Dig1); Dg2 = atoi(Dig2);
		for (k=0;i<p;i++)
		{
			OutSt[k] = ch[i];
			k++;
		}
		return true;
}

char* get_outStr(void) //Вернуть последнюю преобразованную строку
{
	return OutSt;
}
