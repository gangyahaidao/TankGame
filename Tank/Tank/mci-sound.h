#pragma once

#include <stdbool.h>

#define S_START		0
#define S_SHOOT0	100
#define S_SHOOT1	101
#define S_SHOOT2	102
#define S_SHOOT3	103
#define S_BIN		2
#define S_BK		3
#define S_GETPROP	4 // 获取道具
#define S_PROPOUT	5 // 失去道具
#define	S_ENEMY_BOMB 6 // 敌机爆炸
#define S_CAMP_BOMB	7 // 基地爆炸
#define S_SCOREPANEL_DI	8 // 分数面板滴滴声
#define S_BOUNS1000		9 // 分数达到1000奖励声
#define S_ADDLIFE	10 // 吃到增加寿命道具
#define S_WIN		11 // 胜利
#define S_FAIL		12 // 失败
#define S_PLAYER_BOMB	13 // 队友爆炸
#define S_PLAYER_MOVE	14 // 玩家坦克移动
#define S_PLAYER_STOP_MOVE 15 // 玩家坦克停止移动

/**
	初始化音效资源
*/
void InitSounds(); 

/**
	释放音效资源
*/
void CloseSounds();

/**
	播放指定编号的音效
*/
void PlaySounds(int kind);