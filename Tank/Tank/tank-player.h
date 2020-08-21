#pragma once
#include "common.h"
#include "tank-common.h"
#include "game-clock.h"

#define BOMB_SPEED 20 // 炮弹爆炸速度

typedef enum {
	LEFT,
	UP,
	RIGHT,
	DOWN
}TANK_DIDIR;

typedef struct {
	bool mDied; // 玩家生命值是否用完
	byte playerId; // 玩家ID：0/1
	byte mTankLevel; // 坦克等级[1-4]
	IMAGE mTankImage[4][4][2]; // 四种级别四个方向，每个方向两个样式实现坦克履带转动的效果
	int tankPlayerX, tankPlayerY; // 坦克中心点坐标
	int tankWidth, tankHeight; // 坦克的长和宽
	TANK_DIDIR tankDir; // 坦克方向0-左、1-上、2-右、3-下，跟资源图片对应

	int mPlayerLife; // 玩家默认生命值为3
	int mPlayerIconLife_x, 						// 图标坐标
		mPlayerIconLife_y;

	StarFourPoint mStar;			// 四角星闪烁类
	// RingClass mRing;				// 保护圈类，四角星出现之后加载保护圈
	// PropClass mProp;				// 道具类

	int mMoveSpeedDev[4] = {21, 19, 17, 15};	// 四个级别坦克移动时间间隔
	int mBulletSpeedDev[4] = {17, 16, 15, 14 };	// 不同级别子弹时间间隔速度
	TimeClock mTankMoveTimer;		// 坦克移动速度
	TimeClock mBulletTimer;		// 子弹移动速度
	TimeClock mBombTimer;		// 子弹爆炸速度	

}TankPlayer;

/**
	对玩家坦克资源进行初始化
*/
void tank_player_init(TankPlayer* tankPlayer, int playerID,
	int tankX, int tankY,
	int iconLife_x, int iconLife_y);