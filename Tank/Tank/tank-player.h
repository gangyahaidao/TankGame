#pragma once
#include "common.h"
#include "tank-common.h"
#include "game-clock.h"

enum TANK_DIR {
	DIR_LEFT,
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN
};

// 玩家坦克出生保护环
typedef struct {
	IMAGE protecImage[2]; // 保护环图片
	bool needShow; // 是否需要显示保护环
	int imageIndex; // 图片下标
	int timerCount; // 计时器次数
}ProtecCircle;

struct TankPlayer {
	bool mDied; // 玩家生命值是否用完
	byte playerId; // 玩家ID：0/1
	byte mTankLevel; // 坦克等级[0-3]
	IMAGE mTankImage[4][4][2]; // 四种级别，四个方向，每个方向两个样式实现坦克履带转动的效果
	byte mTankImageDirIndex = 0; // 同一个方向的不同图像下标
	bool mTankMoving; // 坦克是否在移动，用于获取同一个方向不同张坦克图片，行走时进行切换，具有动态效果
	int tankPlayerX, tankPlayerY; // 坦克中心点坐标
	int tankWidth, tankHeight; // 坦克的长和宽
	TANK_DIR tankDir; // 坦克方向0-左、1-上、2-右、3-下，跟资源图片对应

	int mPlayerLife; // 玩家默认生命值为3
	int mPlayerIconLife_x, 						// 图标坐标
		mPlayerIconLife_y;

	struct StarFourPoint mStar;			// 四角星闪烁类
	ProtecCircle mProtecCircle;			// 保护圈类，四角星出现之后加载保护圈
	BulletStruct mBullet;				// 炮弹类
	BombStruct	mBombStruct;			// 爆炸类

	int mMoveSpeedDev[4] = {21, 19, 17, 15};	// 四个级别坦克移动时间间隔
	int mBulletSpeedDev[4] = {17, 16, 15, 14 };	// 不同级别子弹时间间隔速度
	TimeClock mTankMoveTimer;		// 坦克移动速度
	TimeClock mBulletTimer;		// 子弹移动速度
};

/**
	对玩家坦克资源进行初始化
*/
void tank_player_init(TankPlayer* tankPlayer, int playerID,
	int tankX, int tankY,
	int iconLife_x, int iconLife_y);

/**
	玩家坦克四角星闪烁，只有在进入新关卡第一次时才会出现四角星，游戏中重生只会出现保护圈
*/
void tank_player_show_star(TankPlayer* tankPlayer);

/**
	绘制玩家坦克
*/
void tank_player_draw_tank(TankPlayer* tankPlayer);

/**
	玩家坦克基于定时器移动
*/
void tank_player_move_by_tanktimer(TankPlayer* tankPlayer);

/**
	判断玩家坦克是否可以通过某一个区域
	tankX和tankY是坦克下一步要移动的坐标，如果不能同行则坐标不变
*/
bool check_tank_can_pass(int tankX, int tankY);

/**
	发射炮弹撞击障碍物，遇到瓷砖则只爆炸，遇到红砖则爆炸并消除
*/
void check_bullet_to_obstacle(TankPlayer* tankPlayer);