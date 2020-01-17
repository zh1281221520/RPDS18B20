#include <wiringPi.h>
#include <stdio.h>  

// LED Pin - wiringPi pin 0 �� BCM_GPIO 17��
//���� wiringPiSetupSys ���г�ʼ��ʱ����ʹ�� BCM ���
//ѡ������ pin ���ʱ����ͬʱʹ�� BCM ���
//���� Property Pages - Build Events - Remote Post-Build Event ����
//��ʹ�� gpio �������� wiringPiSetupSys ������
#define	DQ_OUT	17
#define DQ_IN   18

int ds_flag = 0;

void Init_DS18B20(void)
{
	unsigned int var = 0;


	digitalWrite(DQ_OUT, HIGH);	  //��
	delayMicroseconds(1); // ΢��

	digitalWrite(DQ_OUT, LOW);	  //��
	delayMicroseconds(500);
	digitalWrite(DQ_OUT, HIGH);	  //��


	delayMicroseconds(60);
	var = digitalRead(DQ_IN);
	if (0 == var)
	{
		ds_flag = 1;
		delayMicroseconds(250);

	}
	else
	{
		ds_flag = 0;
	}

	digitalWrite(DQ_OUT, HIGH);	  //��

}

void WriteOneChar(unsigned char data)
{
	unsigned char i = 0, j = 0;
	unsigned int temp = 0;


	digitalWrite(DQ_OUT, HIGH);	  //�� // Write 1
	delayMicroseconds(61);

	for (j = 1;j <= 8;j++)
	{
		digitalWrite(DQ_OUT, LOW);	  //�� // Write 1
		delayMicroseconds(15);

		temp = data & 0x01;
		data = data >> 1;

		digitalWrite(DQ_OUT, temp);	  //�� // Write 1
		delayMicroseconds(45);
		digitalWrite(DQ_OUT, HIGH);	  //�� // Write 1
		delayMicroseconds(2);


	}
	digitalWrite(DQ_OUT, HIGH);	  //�� // Write 1


}

unsigned char ReadOneChar(void)
{
	unsigned char i = 0;
	unsigned char var = 0;
	unsigned char vara = 0;


	digitalWrite(DQ_OUT, HIGH);	  //�� // Write 1
	delayMicroseconds(2);
	digitalWrite(DQ_OUT, LOW);	  //��
	delayMicroseconds(3);

	digitalWrite(DQ_OUT, HIGH);	  //�� // Write 1
	delayMicroseconds(3);


	var = digitalRead(DQ_IN);
	delayMicroseconds(60); // ΢��


	return var;
}


unsigned char DS18B20_Read_Byte(void)
{
	unsigned char i, j, data;
	data = 0;
	for (i = 1;i <= 8;i++)
	{
		j = ReadOneChar();
		data = (j << 7) | (data >> 1);
	}
	return data;
}
//��������������������������������
//��Ȩ����������ΪCSDN������MIPSA����ԭ�����£���ѭ CC 4.0 BY - SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
//ԭ�����ӣ�https ://blog.csdn.net/m0_37105371/article/details/88565308

unsigned int ReadTempetature(void)
{
	unsigned int data[3] = { 0,0,0 };
	unsigned char i = 0;

	Init_DS18B20();
	while (0 == ds_flag)
	{
		Init_DS18B20();
	}

	WriteOneChar(0xCC);
	WriteOneChar(0x44);
	delay(500); // ����
	Init_DS18B20();


	WriteOneChar(0xCC);
	WriteOneChar(0xBE);
	data[0] = DS18B20_Read_Byte();
	data[1] = DS18B20_Read_Byte();
	data[2] = data[1] << 8 | data[0];
	return data[2];

}

int main(void)
{
	unsigned int a = 0;
	wiringPiSetupSys();

	pinMode(DQ_OUT, OUTPUT);
	pinMode(DQ_IN, INPUT);


	while (true)
	{
		a = ReadTempetature();
		printf("temp=%d\n", a);
	}
	return 0;
}


