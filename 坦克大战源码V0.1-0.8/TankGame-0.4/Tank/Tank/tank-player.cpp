#include "tank-player.h"
#include "game-clock.h"

extern HDC center_hdc; // 中间游戏区域，分开绘制方便进行更新

/**
	对玩家坦克资源进行初始化
*/
void tank_player_init(TankPlayer* tankPlayer, int playerID, 
	int tankX, int tankY, 
	int iconLife_x, int iconLife_y) 
{	
	tankPlayer->mDied = false;
	tankPlayer->playerId = playerID;
	tankPlayer->mTankLevel = 1;
	tankPlayer->mTankMoving = false;

	// 填充不同级别坦克、方向对应的图片资源
	int level, dir;
	TCHAR c[50] = { 0 };
	switch (playerID)
	{
	case 0:
	case 1:		
		for (level = 0; level < 4; level++)
		{
			for (dir = 0; dir < 4; dir++) // 表示方向左上右下
			{
				_stprintf_s(c, L"./res/big/%dPlayer/m%d-%d-1.gif", playerID, level, dir); // 同一方向图像1
				loadimage(&tankPlayer->mTankImage[level][dir][0], c);
				_stprintf_s(c, L"./res/big/%dPlayer/m%d-%d-2.gif", playerID, level, dir); // 同一方向图像2
				loadimage(&tankPlayer->mTankImage[level][dir][1], c);
			}
		}
		break;
	default:
		break;
	}

	// 设置初始坐标点和长宽
	tankPlayer->tankPlayerX = tankX;
	tankPlayer->tankPlayerY = tankY;
	tankPlayer->tankWidth = TANK_WIDTH;
	tankPlayer->tankHeight = TANK_HEIGHT;
	tankPlayer->tankDir = DIR_UP;

	tankPlayer->mPlayerLife = 3; // 默认生命值
	tankPlayer->mPlayerIconLife_x = iconLife_x;
	tankPlayer->mPlayerIconLife_y = iconLife_y;

	// 初始化四角星资源与状态
	TCHAR buf[100] = {0};
	// 出生四角星闪烁
	for (int i = 0; i < 4; i++)
	{
		_stprintf_s(buf, _T("./res/big/star%d.gif"), i);
		loadimage(&tankPlayer->mStar.mStarImage[i], buf);
	}
	tankPlayer->mStar.starState = Star_Begin; // 四角星刚出现
	tankPlayer->mStar.mStarCounter = 0;
	tankPlayer->mStar.mStarIndex = 0;
	tankPlayer->mStar.mStarIndexDir = 1; // 初始下标是由小变大

	// 初始化保护环
	TCHAR cir_buf[100];
	for (int i = 0; i < 2; i++)
	{
		_stprintf_s(cir_buf, _T("./res/big/ring%d.gif"), i);
		loadimage(&tankPlayer->mProtecCircle.protecImage[i], cir_buf);
	}	
	tankPlayer->mProtecCircle.needShow = true;
	tankPlayer->mProtecCircle.imageIndex = 0;
	tankPlayer->mProtecCircle.timerCount = 0;

	// 初始化坦克移动、子弹速度、爆炸速度计数器
	clock_init(&tankPlayer->mTankMoveTimer, tankPlayer->mMoveSpeedDev[tankPlayer->mTankLevel]);
	clock_init(&tankPlayer->mBulletTimer, tankPlayer->mBulletSpeedDev[tankPlayer->mTankLevel]);
	clock_init(&tankPlayer->mBombTimer, BOMB_SPEED);

	// 加载炮弹图片资源
	TCHAR bulletBuf[100];
	for (int i = 0; i < 4; i++)
	{
		_stprintf_s(bulletBuf, _T("./res/big/bullet-%d.gif"), i);
		loadimage(&tankPlayer->mBullet.mImage[i], bulletBuf);
	}
	tankPlayer->mBullet.needDraw = false;
}

/**
	玩家坦克四角星闪烁，只有在进入新关卡第一次时才会出现四角星，游戏中重生只会出现保护圈
*/
void tank_player_show_star(TankPlayer* tankPlayer) {
	if (tankPlayer->mStar.starState == Star_End) { // 如果坦克已经出现则不显示四角星
		return;
	}

	if (tankPlayer->mStar.mStarCounter % 2 == 0) { // 主循环如果是偶数次计数，则切四角星图片
		if (tankPlayer->mStar.mStarIndex > 3) {
			tankPlayer->mStar.mStarIndexDir = -1; // 开始由大变小
		}
		else if (tankPlayer->mStar.mStarIndex < 0) {
			tankPlayer->mStar.mStarIndexDir = 1; // 开始由小变大
		}

		tankPlayer->mStar.mStarIndex += tankPlayer->mStar.mStarIndexDir;
	}
	if (tankPlayer->mStar.mStarCounter++ > 25) {
		tankPlayer->mStar.starState = Star_End; // 四角星闪烁完成
		return;
	}

	// 按照计算的下标进行四角星图片绘制
	TransparentBlt(center_hdc, 
		tankPlayer->tankPlayerX - BOX_SIZE, tankPlayer->tankPlayerY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE*2,
		GetImageHDC(&tankPlayer->mStar.mStarImage[tankPlayer->mStar.mStarIndex]),
		0, 0, BOX_SIZE * 2, BOX_SIZE * 2,
		0x000000);
}

/**
	绘制玩家坦克
*/
void tank_player_draw_tank(TankPlayer* tankPlayer) {
	if (tankPlayer->mStar.starState != Star_End) { // 如果还在绘制四角星则返回
		return;
	}

	// 先绘制玩家坦克，再绘制保护环
	if (tankPlayer->mTankMoving) { // 如果坦克在移动，则进行同一方向图片切换实现动态履带效果
		tankPlayer->mTankImageDirIndex = tankPlayer->mTankImageDirIndex == 0 ? 1 : 0;
	}
	IMAGE tankImg = tankPlayer->mTankImage[tankPlayer->mTankLevel][tankPlayer->tankDir][tankPlayer->mTankImageDirIndex];
	TransparentBlt(center_hdc, tankPlayer->tankPlayerX - BOX_SIZE, tankPlayer->tankPlayerY - BOX_SIZE,
		BOX_SIZE * 2, BOX_SIZE * 2,
		GetImageHDC(&tankImg),
		0, 0,
		BOX_SIZE * 2, BOX_SIZE * 2,
		0x000000);

	// 判断是否显示出生保护环
	if (tankPlayer->mProtecCircle.needShow) {
		tankPlayer->mProtecCircle.timerCount++;
		if (tankPlayer->mProtecCircle.timerCount > 215) { // 保护圈有效时间到15*215ms
			tankPlayer->mProtecCircle.needShow = false;
			tankPlayer->mProtecCircle.timerCount = 0;
		}
		else {
			tankPlayer->mProtecCircle.imageIndex = tankPlayer->mProtecCircle.timerCount / 4 % 2; // 每隔一定时间再进行图片切换，相当于每循环四个周期进行切换
			TransparentBlt(center_hdc, tankPlayer->tankPlayerX - BOX_SIZE, tankPlayer->tankPlayerY - BOX_SIZE,
				BOX_SIZE * 2, BOX_SIZE * 2,
				GetImageHDC(&tankPlayer->mProtecCircle.protecImage[tankPlayer->mProtecCircle.imageIndex]),
				0, 0,
				BOX_SIZE * 2, BOX_SIZE * 2,
				0x000000);
		}		
	}

	if (tankPlayer->mBullet.needDraw) { // 判断是否需要绘制炮弹

	}
}

/**
	玩家坦克基于定时器移动
*/
void tank_player_move_by_tanktimer(TankPlayer* tankPlayer) {
	if (clock_is_timeout(&tankPlayer->mTankMoveTimer)) { // 坦克定时器时间到，才开始移动
		switch (tankPlayer->tankDir) {
		case DIR_LEFT:
			tankPlayer->tankPlayerX += -1;
			break;
		case DIR_UP:
			tankPlayer->tankPlayerY += -1;
			break;
		case DIR_RIGHT:
			tankPlayer->tankPlayerX += 1;
			break;
		case DIR_DOWN:
			tankPlayer->tankPlayerY += 1;
			break;
		default:
			break;
		}
	}
}