#pragma once
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <tchar.h>

#include <easyx.h>			// 引用图形库头文件

#include <windows.h>
#include <Mmsystem.h>
#pragma comment ( lib, "Winmm.lib" )

// GDI
#include <wingdi.h>
#pragma comment (lib, "msimg32.lib")

#define WINDOW_WIDTH 512 // 定义窗口大小
#define WINDOW_HEIGHT 448
#define CANVAS_WIDTH	256		// 画布 image 大小，不会改变，左右绘图都在 image 上操作，然后一次性绘制到主窗口
#define CANVAS_HEIGHT	224