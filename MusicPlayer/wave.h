#pragma once

#include <fstream>
#include <math.h>
#include <cmath>

using namespace std;

struct Wave
{
	unsigned long file_size;        //�ļ���С
	unsigned short channel;         //ͨ����
	unsigned long frequency;        //����Ƶ��
	unsigned long Bps;              //Byte��
	unsigned short sample_num_bit;  //һ��������λ��
	unsigned long data_size;        //���ݴ�С
	double wav_time;				//��Ƶ��ʱ��
};