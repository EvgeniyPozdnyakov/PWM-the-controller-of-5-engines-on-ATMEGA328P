
#define DATA_TRANSM		 	0
#define DATA_END_TRANSM		1
#define DATA_ERROR_TRANSM	2
#define DATA_READED			3





class TWIinterface
{
public:
	uint8_t TWI_ST_FLAG; //��������� TWI ������
	uint8_t TWI_ST; //��������� TWI ������
	uint8_t TWI_WRITE_DATA_LEN; //����� ������������  ����������� ���� 
	uint8_t* LPDATA; //��������� �������������� ������
	uint8_t to_sla; //����� �������� ����������
	uint8_t Len_BYTE; //������ ��������� ��� �������� ����������
public:
	TWIinterface(bool type, uint8_t addres, uint8_t len_b);

	void TWI_StartMaster(void);//������������� TWI ����������
	void TWI_StartSlave(uint8_t Adress);

	void SendTWIMaster(uint8_t Adress, uint8_t* DATA, uint8_t length); //�������� ������

	void TWI_Slave_Transm_mode(void);
	void TWI_Mast_Ress_mode(void);
	void TWI_Mast_Transm_mode(void);
	void TWI_Slave_Ress_mode(void);
};

/*
STAT	//������ ������ ������
Int	ret
SLA+W  //����� ���������
Int ASK //���������� ������������� ��������� -->�����
//----------------���� �������� ������ ������----------------------------
DATA //�������� N-������ ������
Int ASK //���������� ������������� ������ ����� ������ -->�����
//----------------���� �������� ������ �����----------------------------
STOP	//���������� ������ ������

*/


/*
TWSR //������� �������

TWDR //������� ������

TWA //�������� ������� ��������
*/



