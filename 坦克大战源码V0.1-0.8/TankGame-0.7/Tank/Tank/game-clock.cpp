#include "game-clock.h"

TimeClock mainTimer; // 用于主窗口程序的界面定时绘制

/**
	初始化时间值
*/
void clock_init(TimeClock* clock_struct, double t) {
	QueryPerformanceCounter(&clock_struct->clock_obj); // 获取当前系统的滴答值
	clock_struct->time_start = clock_struct->clock_obj.QuadPart;
	clock_struct->time_stop = clock_struct->clock_obj.QuadPart;

	QueryPerformanceFrequency(&clock_struct->clock_obj); // 获取当前系统的始终频率
	clock_struct->time_freq = clock_struct->clock_obj.QuadPart;

	clock_struct->time_ms = t; // 设置定时器时长
}

/**
	判断是否到达指定的定时时间
*/
bool clock_is_timeout(TimeClock* clock_struct) {
	QueryPerformanceCounter(&clock_struct->clock_obj);
	clock_struct->time_stop = clock_struct->clock_obj.QuadPart; // 获取当前的时间

	if ((double)(clock_struct->time_stop - clock_struct->time_start) * 1000 / clock_struct->time_freq > clock_struct->time_ms) {
		clock_struct->time_start = clock_struct->time_stop; // 将开始时间与截止时间滴答值设置为一样
		return true;
	}

	return false;
}
