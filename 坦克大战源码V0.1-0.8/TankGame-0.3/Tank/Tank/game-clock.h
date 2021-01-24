#pragma once
#include "common.h"

typedef struct {
	LARGE_INTEGER clock_obj; // ���ڴ洢��ȡ��ʱ����Ϣ
	LONGLONG time_start; // ���ڻ�ȡ��ʼ�ĵδ�ֵ
	LONGLONG time_stop; // ���ڻ�ȡ�����ĵδ�ֵ
	double time_freq; // ʼ��Ƶ�ʣ�������ʱ��value = (time_start-time_stop)*1000/time_freq;
	double time_ms; // ���õĵ���ʱʱ�䳤��ms
}TimeClock;

/**
	��ʼ��ʱ��ֵ
*/
void clock_init(TimeClock* clock_struct, double t);

/**
	�ж��Ƿ񵽴�ָ���Ķ�ʱʱ��
*/
bool clock_is_timeout(TimeClock* clock_struct);