#include "game-clock.h"

TimeClock mainTimer; // ���������ڳ���Ľ��涨ʱ����

/**
	��ʼ��ʱ��ֵ
*/
void clock_init(TimeClock* clock_struct, double t) {
	QueryPerformanceCounter(&clock_struct->clock_obj); // ��ȡ��ǰϵͳ�ĵδ�ֵ
	clock_struct->time_start = clock_struct->clock_obj.QuadPart;
	clock_struct->time_stop = clock_struct->clock_obj.QuadPart;

	QueryPerformanceFrequency(&clock_struct->clock_obj); // ��ȡ��ǰϵͳ��ʼ��Ƶ��
	clock_struct->time_freq = clock_struct->clock_obj.QuadPart;

	clock_struct->time_ms = t; // ���ö�ʱ��ʱ��
}

/**
	�ж��Ƿ񵽴�ָ���Ķ�ʱʱ��
*/
bool clock_is_timeout(TimeClock* clock_struct) {
	QueryPerformanceCounter(&clock_struct->clock_obj);
	clock_struct->time_stop = clock_struct->clock_obj.QuadPart; // ��ȡ��ǰ��ʱ��

	if ((double)(clock_struct->time_stop - clock_struct->time_start) * 1000 / clock_struct->time_freq > clock_struct->time_ms) {
		clock_struct->time_start = clock_struct->time_stop; // ����ʼʱ�����ֹʱ��δ�ֵ����Ϊһ��
		return true;
	}

	return false;
}
