#pragma once
#include "common.h"
#include "tank-common.h"
#include "game-clock.h"

#define MAX_TANK_ENEMY 20 // 最大敌机数量
#define TANK_ENEMY_BULLET_SPEED 30 // 敌机坦克炮弹运行间隔时长ms，经过指定的时长更新坐标
#define TANK_ENEMY_POS_SPEED 3 // 敌机炮弹每次更新坐标的像素值

enum ENE_TANK_DIR {
	ENE_DIR_LEFT,
	ENE_DIR_UP,
	ENE_DIR_RIGHT,
	ENE_DIR_DOWN
};

typedef struct tank_enemy {
	int bornPosX[3] = { BOX_SIZE, BOX_SIZE * 13, BOX_SIZE * 25 }; // 敌机坦克随机出现的三个位置X坐标	
	int mTankX;
	int mTankY; // 坦克中心点坐标
	ENE_TANK_DIR dir = ENE_DIR_DOWN; // 坦克方向

	byte tankModel; // 坦克类型，分为道具坦克和普通坦克两种
	bool mDied; // 是否挂了
	bool mBorned; // 是否已经出生，用于绘制遍历
	bool mTankMoving; // 坦克是否在移动
	int mMoveStep; // 当前方向定时移动的随机次数，一定次数后或者遇到障碍物则变换方向，并重新计算
		
	int mMoveSpeed[4] = { 1, 1, 1, 1 }; // 坦克每次定时移动的像素点
	int mMoveSpeedDev[4] = { 30, 15, 27, 25 };	// 四个级别坦克移动时间间隔	
	byte mTankLevel; // 坦克级别[0-3]
	IMAGE mTankImage[4][2]; // 给坦克随机分配一个型号，每个型号四个方向，每个方向两个图像
	int mTankImageDirIndex; // 用于坦克行走图片下标切换

	IMAGE mScoreImage[4]; // 敌机爆炸之后显示的分数，100、200、300、400

	struct StarFourPoint mStar; // 出生四角星
	BulletStruct mBullet; // 子弹结构体
	BombStruct mBombStruct; // 爆炸效果
	BlastStruct mBlastStruct; // 被击中大爆炸效果

	TimeClock mBulletTimer; // 控制子弹移动速度
	TimeClock mTankMoveTimer; // 坦克移动速度
	TimeClock mShootTimer; // 坦克发射子弹的频率

}TankEnemy;

/**
	增加一个敌机坦克
*/
void tank_enemy_add();

/**
	显示敌机坦克四角星
*/
void tank_enemy_show_star();

/**
	绘制敌机坦克和炮弹
*/
void tank_enemy_draw_tank();

/**
	敌机坦克重新调整方向
*/
void tank_enemy_rejust_direction(TankEnemy* pTankEnemy);

/**
	判断敌机坦克是否可以通过某一个区域
	tankX和tankY是坦克下一步要移动的坐标，如果不能通行则坐标不变
*/
bool check_tank_enemy_can_pass(int dir, int tankX, int tankY);

/**
	发射炮弹撞击障碍物，遇到瓷砖则只爆炸，遇到红砖则爆炸并消除
*/
void check_enemy_bullet_to_obstacle(TankEnemy* pTankEnemy);