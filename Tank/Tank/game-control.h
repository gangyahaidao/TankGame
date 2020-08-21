#pragma once

#include "mci-sound.h"

enum GameResult { Victory, Fail };		// 每一关胜利\失败

/**
	游戏界面资源初始化
*/
void game_control_init();

/**
	进入游戏关卡的预热画面
*/
void game_control_show_stage();

/**
	开始游戏循环体：进行界面玩家、子弹、敌机、其他所有信息的更新
*/
GameResult game_control_start_game();

/**
	更新右边的分数关卡等数据
*/
void game_control_right_panel();

/**
	加载地图进行绘制
*/
void game_control_center_panel();

/**
	开始游戏循环体
*/
void game_control_loop();