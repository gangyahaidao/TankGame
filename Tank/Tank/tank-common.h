#pragma once
// 主要是用来表达一些玩家坦克与敌方坦克共同使用的一些功能，如：坦克位置盒子标记、子弹结构、子弹爆炸、坦克爆炸、玩家/敌机四角星闪烁、道具类等

#include "common.h"

// 四角星闪烁，敌机或者玩家的四角星闪烁返回类型
enum Star_State {
	Star_Begin,			// 四角星刚刚出现
	Star_End			// 坦克已经出现
};

struct StarFourPoint {
	IMAGE mStarImage[4]; // 四张四角星图片
	int mStarIndex; // 数组下标
	int mStarIndexDir; // 数字下标变化方向：1/-1，1是由小变大，-1是由大变小
	int mStarCounter; // 计数，多长次循环变更一次图像
	Star_State starState;				// 四角星消失的时候坦克出现, 停止播放四角星闪烁图
};

typedef struct bullet_struct{
	int posX, posY; // 炮弹当前坐标
	int dir; // 炮弹方向
	int speed[4] = {2, 3, 3, 4}; // 坦克不同级别对用不同的子弹速度，但是每次移动不跨过4个像素点，不然会有击中BUG
	int mKillID; // 记录击中的玩家/敌机ID
	IMAGE mImage[4]; // 炮弹图片资源
	int bulletSize[4][2] = { {4, 3}, {3, 4}, {4, 3}, {3, 4} }; // 左右：4*3   上下：3*4   
	int bullet_bias[4][2] = { {-BOX_SIZE, -1}, {-2, -BOX_SIZE}, {BOX_SIZE, -1}, {-2, BOX_SIZE} }; // 炮弹中心相对于坦克中心个方向的偏移量，用来绘制炮弹路径
	bool needDraw; // 是否需要在指定坐标绘制炮弹
}BulletStruct;