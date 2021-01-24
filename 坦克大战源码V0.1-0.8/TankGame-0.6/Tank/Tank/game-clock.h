#pragma once
#include "common.h"

typedef struct {
	LARGE_INTEGER clock_obj; // 用于存储获取的时间信息
	LONGLONG time_start; // 用于获取开始的滴答值
	LONGLONG time_stop; // 用于获取结束的滴答值
	double time_freq; // 始终频率，经历的时间value = (time_start-time_stop)*1000/time_freq;
	double time_ms; // 设置的倒计时时间长度ms
}TimeClock;

/**
	初始化时间值
*/
void clock_init(TimeClock* clock_struct, double t);

/**
	判断是否到达指定的定时时间
*/
bool clock_is_timeout(TimeClock* clock_struct);