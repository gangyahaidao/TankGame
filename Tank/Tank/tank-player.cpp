#include "tank-player.h"
#include "game-clock.h"

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

	// 填充不同级别坦克、方向对应的图片资源
	int level, dir;
	switch (playerID)
	{
	case 0:
	case 1:
	{
		TCHAR c[50] = {0};
		for (level = 0; level < 4; level++) {		
		{
			for (dir = 0; dir < 4; dir++) // 表示方向左上右下
				_stprintf_s(c, L"./res/big/%dPlayer/m%d-%d-1.gif", playerID, level, dir); // 同一方向图像1
				loadimage(&tankPlayer->mTankImage[level][dir][0], c);
				_stprintf_s(c, L"./res/big/%dPlayer/m%d-%d-2.gif", playerID, level, dir); // 同一方向图像2
				loadimage(&tankPlayer->mTankImage[level][dir][1], c);
			}			
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
	tankPlayer->tankDir = UP;

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
	tankPlayer->mStar.starState = Star_Out; // 四角星刚出现

	// 初始化坦克移动、子弹速度、爆炸速度计数器
	clock_init(&tankPlayer->mTankMoveTimer, tankPlayer->mMoveSpeedDev[tankPlayer->mTankLevel]);
	clock_init(&tankPlayer->mBulletTimer, tankPlayer->mBulletSpeedDev[tankPlayer->mTankLevel]);
	clock_init(&tankPlayer->mBombTimer, BOMB_SPEED);

}