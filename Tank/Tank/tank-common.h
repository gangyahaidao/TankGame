#pragma once
// 主要是用来表达一些玩家坦克与敌方坦克共同使用的一些功能，如：坦克位置盒子标记、子弹结构、子弹爆炸、坦克爆炸、玩家/敌机四角星闪烁、道具类等

#include "common.h"

// 四角星闪烁，敌机或者玩家的四角星闪烁返回类型
enum Star_State {
	Star_Timing,		// 只用于敌机, 表示当前还没有出现四角星, 还在计时之中
	Star_Failed,		// 用于敌机, 表示当前位置敌机出现失败
	Star_Out,			// 四角星刚刚出现
	Star_Showing,		// 正在闪烁四角星
	Star_Stop,			// 刚刚停止闪烁
	Tank_Out			// 坦克已经出现
};


typedef struct {
	IMAGE mStarImage[4]; // 四张四角星图片
	byte mStarIndex; // 数组下标
	int mStarCounter; // 计数，多长次循环变更一次图像
	Star_State starState;				// 四角星消失的时候坦克出现, 停止播放四角星闪烁图
}StarFourPoint;