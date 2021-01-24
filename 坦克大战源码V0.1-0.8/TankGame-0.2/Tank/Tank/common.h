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
#define CENTER_WIDTH	208		// 中间黑色游戏区域
#define CENTER_HEIGHT	208
#define BLACK_NUMBER_SIZE 7		// 关卡黑色数字大小
#define ENEMY_TANK_ICO_SIZE 7	// 面板右边敌机图标大小
#define FLAG_ICO_SIZE_X 16		// 旗子图标的宽
#define FLAG_ICO_SIZE_Y 15		// 旗子的高
#define CENTER_X		16		// 黑色游戏区域相对左上角的坐标
#define CENTER_Y		9
#define PLAYER_12_ICO_SIZE_X		14		// 1P/2P右侧玩家图标大小
#define PLAYER_12_ICO_SIZE_Y		7
#define PLAYER_TANK_ICO_SIZE_X		7		// 玩家图标大小
#define PLAYER_TANK_ICO_SIZE_Y		8

#define BOX_SIZE		8		// 中间画布被划分成26*26的格子，每个格子是8像素
#define _CLEAR		-1		// 标记障碍物被消除的格子, 用于扫描时与 0 区分开, 然后绘制黑色图片, 避免对 0 也进行多余操作
#define _EMPTY		0		// 空地
#define _FOREST		1		// 森林
#define _ICE		2		// 冰
// ------ 以上 < 3 坦克可以穿行
#define _WALL		3		// 墙
#define _RIVER		4		// 河流
#define _STONE		5		// 石头