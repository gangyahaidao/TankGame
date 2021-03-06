#include "tank-player.h"
#include "game-clock.h"
#include "mci-sound.h"

extern HDC center_hdc; // 中间游戏区域，分开绘制方便进行更新
extern char map26x26[26][26]; // 地图数据

/**
	对玩家坦克资源进行初始化
*/
void tank_player_init(TankPlayer* tankPlayer, int playerID, 
	int tankX, int tankY, 
	int iconLife_x, int iconLife_y) 
{	
	tankPlayer->mDied = false;
	tankPlayer->playerId = playerID;
	tankPlayer->mTankLevel = 0; // 从0开始
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

	// 加载爆炸效果图片
	for (int i = 0; i < 3; i++)
	{
		_stprintf_s(buf, _T("./res/big/bumb%d.gif"), i+1);
		loadimage(&tankPlayer->mBombStruct.bombImage[i], buf);
	}
	tankPlayer->mBombStruct.bombCounter = 0;
	tankPlayer->mBombStruct.showBomb = false;
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

	// 检查炮弹是否遇到障碍物
	check_bullet_to_obstacle(tankPlayer);
	// 炮弹遇到障碍物产生爆炸效果
	if (tankPlayer->mBombStruct.showBomb) { // 需要显示爆炸效果
		int index[6] = { 0,1,1,2,2,1 };	// 图片下标切换顺序
		if (tankPlayer->mBombStruct.bombCounter >= 6) {
			tankPlayer->mBombStruct.showBomb = false;
			tankPlayer->mBombStruct.bombCounter = 0;
		} else {
			TransparentBlt(center_hdc,
				tankPlayer->mBombStruct.mBombX - BOX_SIZE, tankPlayer->mBombStruct.mBombY - BOX_SIZE,
				BOX_SIZE * 2, BOX_SIZE * 2,
				GetImageHDC(&tankPlayer->mBombStruct.bombImage[index[tankPlayer->mBombStruct.bombCounter++]]),
				0, 0,
				BOX_SIZE * 2, BOX_SIZE * 2,
				0x000000);
		}
	}

	// 炮弹是否需要绘制
	if(tankPlayer->mBullet.needDraw) {
		// 绘制玩家坦克炮弹
		int dir = tankPlayer->mBullet.dir; // 炮弹方向
		int bulletX = tankPlayer->mBullet.posX + tankPlayer->mBullet.bullet_bias[dir][0]; // 炮弹当前左上角的坐标
		int bulletY = tankPlayer->mBullet.posY + tankPlayer->mBullet.bullet_bias[dir][1];
		TransparentBlt(center_hdc,
			bulletX, bulletY,
			tankPlayer->mBullet.bulletSize[dir][0], tankPlayer->mBullet.bulletSize[dir][1],
			GetImageHDC(&tankPlayer->mBullet.mImage[tankPlayer->mBullet.dir]),
			0, 0,
			tankPlayer->mBullet.bulletSize[dir][0], tankPlayer->mBullet.bulletSize[dir][1],
			0x000000);
	}
}

/**
	玩家坦克基于定时器移动
*/
void tank_player_move_by_tanktimer(TankPlayer* tankPlayer) {
	int x = tankPlayer->tankPlayerX;
	int y = tankPlayer->tankPlayerY;
	bool result = false;

	if (clock_is_timeout(&tankPlayer->mTankMoveTimer)) { // 坦克定时器时间到，才开始移动
		switch (tankPlayer->tankDir) {
		case DIR_LEFT:
			x += -1;
			result = check_tank_can_pass(x, y); // 判断将要运动的位置是否可以继续前进
			if (result) {
				tankPlayer->tankPlayerX += -1;
			}			
			break;
		case DIR_UP:
			y += -1;
			result = check_tank_can_pass(x, y);
			if (result) {
				tankPlayer->tankPlayerY += -1;
			}			
			break;
		case DIR_RIGHT:
			x += 1;
			result = check_tank_can_pass(x, y);
			if (result) {
				tankPlayer->tankPlayerX += 1;
			}			
			break;
		case DIR_DOWN:
			y += 1;
			result = check_tank_can_pass(x, y);
			if (result) {
				tankPlayer->tankPlayerY += 1;
			}			
			break;
		default:
			break;
		}
	}
}

/**
	判断玩家坦克是否可以通过某一个区域
	tankX和tankY是坦克下一步要移动的坐标，如果不能同行则坐标不变
*/
bool check_tank_can_pass(int tankX, int tankY) {
	int x1 = tankX - BOX_SIZE;
	int x2 = tankX + BOX_SIZE;
	int y1 = tankY - BOX_SIZE;
	int y2 = tankY + BOX_SIZE;

	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			if (map26x26[i][j] == _WALL ||
				map26x26[i][j] == _RIVER ||
				map26x26[i][j] == _STONE) { // 如果是墙、河流、石头
				int t_x1 = j * BOX_SIZE;
				int t_x2 = j * BOX_SIZE + BOX_SIZE;
				int t_y1 = i * BOX_SIZE;
				int t_y2 = i * BOX_SIZE + BOX_SIZE;
				// 是否地图的矩形和坦克的矩形满足不相交的条件（左右上下）:https://zhuanlan.zhihu.com/p/29704064
				bool nonIntersect = (x2 <= t_x1+1) ||
					(x1 >= t_x2-1) ||
					(y2 <= t_y1+1) ||
					(y1 >= t_y2-1);
				if (nonIntersect == false) {
					return false; // 说明两个矩形会相交
				}				
			}
		}
	}
	
	// 判断是否超过游戏地图边界
	if (x1 < 0 || x2 > CENTER_WIDTH || y1 < 0 || y2 > CENTER_HEIGHT) {
		return false;
	}
	return true;
}

/**
	发射炮弹撞击障碍物，遇到瓷砖则只爆炸，遇到红砖则爆炸并消除
*/
void check_bullet_to_obstacle(TankPlayer* tankPlayer) {
	if (tankPlayer->mBullet.needDraw == false) { // 如果没发射子弹不需要判断
		return;
	}
	int bulletSize[4][2] = { {4, 3}, {3, 4}, {4, 3}, {3, 4} }; // 左右：4*3   上下：3*4   

	int dir = tankPlayer->mBullet.dir;
	int x1 = tankPlayer->mBullet.posX;
	int y1 = tankPlayer->mBullet.posY;
	int x2 = x1 + bulletSize[dir][0];
	int y2 = y1 + bulletSize[dir][1];

	bool nonIntersect = false;
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			if (map26x26[i][j] == _WALL ||
				map26x26[i][j] == _STONE) { // 如果是墙、石头	
				int t_x1 = j * BOX_SIZE;
				int t_x2 = j * BOX_SIZE + BOX_SIZE;
				int t_y1 = i * BOX_SIZE;
				int t_y2 = i * BOX_SIZE + BOX_SIZE;
				// 是否地图的矩形和炮弹的矩形满足不相交的条件（左右上下）:https://zhuanlan.zhihu.com/p/29704064
				nonIntersect = (x2 <= t_x1) ||
					(x1 >= t_x2) ||
					(y2 <= t_y1) ||
					(y1 >= t_y2); // ==true说明不相交  ==false说明相交
				
				if (nonIntersect == false) {
					tankPlayer->mBombStruct.showBomb = true;
					tankPlayer->mBullet.needDraw = false; // 炮弹定时器停止计数
					tankPlayer->mBombStruct.mBombX = (x1 + x2) / 2;
					tankPlayer->mBombStruct.mBombY = (y1 + y2) / 2;
					if (map26x26[i][j] == _WALL) { // 爆炸并设置清除标志
						map26x26[i][j] = _EMPTY; // 设置为空地
					}
					return;
				}
			}
		}
	}	
}