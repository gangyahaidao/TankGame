#pragma once
#include "common.h"
#include "tank-common.h"
#include "game-clock.h"

typedef struct {
	bool mDied; // 玩家生命值是否用完
	byte playerId; // 玩家ID：0/1
	byte mTankLevel; // 坦克等级[1-4]
	IMAGE mTankImage[4][4][2]; // 四种级别四个方向，每个方向两个样式实现坦克履带转动的效果
	int tankPlayerX, tankPlayerY; // 坦克中心点坐标
	int tankWidth, tankHeight; // 坦克的长和宽
	byte tankDir; // 坦克方向0-上、1-右、2-下、3-左

	int mPlayerLife; // 玩家默认生命值为3
	int mPlayerLife_x, 						// 图标坐标
		mPlayerLife_y;

	StarFourPoint mStar;			// 四角星闪烁类
	// RingClass mRing;				// 保护圈类，四角星出现之后加载保护圈
	// PropClass mProp;				// 道具类

	TimeClock mTankTimer;		// 坦克移动速度
	TimeClock mBulletTimer;		// 子弹移动速度
	TimeClock mBombTimer;		// 子弹爆炸速度
	int mMoveSpeedDev[4];	// 四个级别坦克移动时间间隔
	int mBulletSpeedDev[4];	// 不同级别子弹时间间隔速度

	IMAGE mEmermyScoreImg[4];	// 100\200.. 分, 不同敌机爆炸位置显示的分数

	// 右侧信息榜
	IMAGE m12PImage;   // 1P\2P 图标
	IMAGE mPlayerTankIcoImage;
	IMAGE mPlayerTankIcoImage;				// 玩家坦克图标
	int   mPlayerTankIcoImage_x,			// 图标坐标
		mPlayerTankIcoImage_y;
	IMAGE mBlackNumberImage;				// 0123456789 黑色数字
}TankPlayer;