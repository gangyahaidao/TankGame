#include "tank-player.h"
#include "game-clock.h"
#include "mci-sound.h"

extern HDC center_hdc; // �м���Ϸ���򣬷ֿ����Ʒ�����и���
extern char map26x26[26][26]; // ��ͼ����

/**
	�����̹����Դ���г�ʼ��
*/
void tank_player_init(TankPlayer* tankPlayer, int playerID, 
	int tankX, int tankY, 
	int iconLife_x, int iconLife_y) 
{	
	tankPlayer->mDied = false;
	tankPlayer->playerId = playerID;
	tankPlayer->mTankLevel = 0; // ��0��ʼ
	tankPlayer->mTankMoving = false;

	// ��䲻ͬ����̹�ˡ������Ӧ��ͼƬ��Դ
	int level, dir;
	TCHAR c[50] = { 0 };
	switch (playerID)
	{
	case 0:
	case 1:		
		for (level = 0; level < 4; level++)
		{
			for (dir = 0; dir < 4; dir++) // ��ʾ������������
			{
				_stprintf_s(c, L"./res/big/%dPlayer/m%d-%d-1.gif", playerID, level, dir); // ͬһ����ͼ��1
				loadimage(&tankPlayer->mTankImage[level][dir][0], c);
				_stprintf_s(c, L"./res/big/%dPlayer/m%d-%d-2.gif", playerID, level, dir); // ͬһ����ͼ��2
				loadimage(&tankPlayer->mTankImage[level][dir][1], c);
			}
		}
		break;
	default:
		break;
	}

	// ���ó�ʼ�����ͳ���
	tankPlayer->tankPlayerX = tankX;
	tankPlayer->tankPlayerY = tankY;
	tankPlayer->tankWidth = TANK_WIDTH;
	tankPlayer->tankHeight = TANK_HEIGHT;
	tankPlayer->tankDir = DIR_UP;

	tankPlayer->mPlayerLife = 3; // Ĭ������ֵ
	tankPlayer->mPlayerIconLife_x = iconLife_x;
	tankPlayer->mPlayerIconLife_y = iconLife_y;

	// ��ʼ���Ľ�����Դ��״̬
	TCHAR buf[100] = {0};
	// �����Ľ�����˸
	for (int i = 0; i < 4; i++)
	{
		_stprintf_s(buf, _T("./res/big/star%d.gif"), i);
		loadimage(&tankPlayer->mStar.mStarImage[i], buf);
	}
	tankPlayer->mStar.starState = Star_Begin; // �Ľ��Ǹճ���
	tankPlayer->mStar.mStarCounter = 0;
	tankPlayer->mStar.mStarIndex = 0;
	tankPlayer->mStar.mStarIndexDir = 1; // ��ʼ�±�����С���

	// ��ʼ��������
	TCHAR cir_buf[100];
	for (int i = 0; i < 2; i++)
	{
		_stprintf_s(cir_buf, _T("./res/big/ring%d.gif"), i);
		loadimage(&tankPlayer->mProtecCircle.protecImage[i], cir_buf);
	}	
	tankPlayer->mProtecCircle.needShow = true;
	tankPlayer->mProtecCircle.imageIndex = 0;
	tankPlayer->mProtecCircle.timerCount = 0;

	// ��ʼ��̹���ƶ����ӵ��ٶȡ���ը�ٶȼ�����
	clock_init(&tankPlayer->mTankMoveTimer, tankPlayer->mMoveSpeedDev[tankPlayer->mTankLevel]);
	clock_init(&tankPlayer->mBulletTimer, tankPlayer->mBulletSpeedDev[tankPlayer->mTankLevel]);
	clock_init(&tankPlayer->mBombTimer, BOMB_SPEED);

	// �����ڵ�ͼƬ��Դ
	TCHAR bulletBuf[100];
	for (int i = 0; i < 4; i++)
	{
		_stprintf_s(bulletBuf, _T("./res/big/bullet-%d.gif"), i);
		loadimage(&tankPlayer->mBullet.mImage[i], bulletBuf);
	}
	tankPlayer->mBullet.needDraw = false;

	// ���ر�ըЧ��ͼƬ
	for (int i = 0; i < 3; i++)
	{
		_stprintf_s(buf, _T("./res/big/bumb%d.gif"), i+1);
		loadimage(&tankPlayer->mBombStruct.bombImage[i], buf);
	}
	tankPlayer->mBombStruct.bombCounter = 0;
	tankPlayer->mBombStruct.showBomb = false;
}

/**
	���̹���Ľ�����˸��ֻ���ڽ����¹ؿ���һ��ʱ�Ż�����Ľ��ǣ���Ϸ������ֻ����ֱ���Ȧ
*/
void tank_player_show_star(TankPlayer* tankPlayer) {
	if (tankPlayer->mStar.starState == Star_End) { // ���̹���Ѿ���������ʾ�Ľ���
		return;
	}

	if (tankPlayer->mStar.mStarCounter % 2 == 0) { // ��ѭ�������ż���μ����������Ľ���ͼƬ
		if (tankPlayer->mStar.mStarIndex > 3) {
			tankPlayer->mStar.mStarIndexDir = -1; // ��ʼ�ɴ��С
		}
		else if (tankPlayer->mStar.mStarIndex < 0) {
			tankPlayer->mStar.mStarIndexDir = 1; // ��ʼ��С���
		}

		tankPlayer->mStar.mStarIndex += tankPlayer->mStar.mStarIndexDir;
	}
	if (tankPlayer->mStar.mStarCounter++ > 25) {
		tankPlayer->mStar.starState = Star_End; // �Ľ�����˸���
		return;
	}

	// ���ռ�����±�����Ľ���ͼƬ����
	TransparentBlt(center_hdc, 
		tankPlayer->tankPlayerX - BOX_SIZE, tankPlayer->tankPlayerY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE*2,
		GetImageHDC(&tankPlayer->mStar.mStarImage[tankPlayer->mStar.mStarIndex]),
		0, 0, BOX_SIZE * 2, BOX_SIZE * 2,
		0x000000);
}

/**
	�������̹��
*/
void tank_player_draw_tank(TankPlayer* tankPlayer) {
	if (tankPlayer->mStar.starState != Star_End) { // ������ڻ����Ľ����򷵻�
		return;
	}

	// �Ȼ������̹�ˣ��ٻ��Ʊ�����
	if (tankPlayer->mTankMoving) { // ���̹�����ƶ��������ͬһ����ͼƬ�л�ʵ�ֶ�̬�Ĵ�Ч��
		tankPlayer->mTankImageDirIndex = tankPlayer->mTankImageDirIndex == 0 ? 1 : 0;
	}
	IMAGE tankImg = tankPlayer->mTankImage[tankPlayer->mTankLevel][tankPlayer->tankDir][tankPlayer->mTankImageDirIndex];
	TransparentBlt(center_hdc, tankPlayer->tankPlayerX - BOX_SIZE, tankPlayer->tankPlayerY - BOX_SIZE,
		BOX_SIZE * 2, BOX_SIZE * 2,
		GetImageHDC(&tankImg),
		0, 0,
		BOX_SIZE * 2, BOX_SIZE * 2,
		0x000000);

	// �ж��Ƿ���ʾ����������
	if (tankPlayer->mProtecCircle.needShow) {
		tankPlayer->mProtecCircle.timerCount++;
		if (tankPlayer->mProtecCircle.timerCount > 215) { // ����Ȧ��Чʱ�䵽15*215ms
			tankPlayer->mProtecCircle.needShow = false;
			tankPlayer->mProtecCircle.timerCount = 0;
		}
		else {
			tankPlayer->mProtecCircle.imageIndex = tankPlayer->mProtecCircle.timerCount / 4 % 2; // ÿ��һ��ʱ���ٽ���ͼƬ�л����൱��ÿѭ���ĸ����ڽ����л�
			TransparentBlt(center_hdc, tankPlayer->tankPlayerX - BOX_SIZE, tankPlayer->tankPlayerY - BOX_SIZE,
				BOX_SIZE * 2, BOX_SIZE * 2,
				GetImageHDC(&tankPlayer->mProtecCircle.protecImage[tankPlayer->mProtecCircle.imageIndex]),
				0, 0,
				BOX_SIZE * 2, BOX_SIZE * 2,
				0x000000);
		}		
	}

	// ����ڵ��Ƿ������ϰ���
	check_bullet_to_obstacle(tankPlayer);
	// �ڵ������ϰ��������ըЧ��
	if (tankPlayer->mBombStruct.showBomb) { // ��Ҫ��ʾ��ըЧ��
		int index[6] = { 0,1,1,2,2,1 };	// ͼƬ�±��л�˳��
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

	// �ڵ��Ƿ���Ҫ����
	if(tankPlayer->mBullet.needDraw) {
		// �������̹���ڵ�
		int dir = tankPlayer->mBullet.dir; // �ڵ�����
		int bulletX = tankPlayer->mBullet.posX + tankPlayer->mBullet.bullet_bias[dir][0]; // �ڵ���ǰ���Ͻǵ�����
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
	���̹�˻��ڶ�ʱ���ƶ�
*/
void tank_player_move_by_tanktimer(TankPlayer* tankPlayer) {
	int x = tankPlayer->tankPlayerX;
	int y = tankPlayer->tankPlayerY;
	bool result = false;

	if (clock_is_timeout(&tankPlayer->mTankMoveTimer)) { // ̹�˶�ʱ��ʱ�䵽���ſ�ʼ�ƶ�
		switch (tankPlayer->tankDir) {
		case DIR_LEFT:
			x += -1;
			result = check_tank_can_pass(x, y); // �жϽ�Ҫ�˶���λ���Ƿ���Լ���ǰ��
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
	�ж����̹���Ƿ����ͨ��ĳһ������
	tankX��tankY��̹����һ��Ҫ�ƶ������꣬�������ͬ�������겻��
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
				map26x26[i][j] == _STONE) { // �����ǽ��������ʯͷ
				int t_x1 = j * BOX_SIZE;
				int t_x2 = j * BOX_SIZE + BOX_SIZE;
				int t_y1 = i * BOX_SIZE;
				int t_y2 = i * BOX_SIZE + BOX_SIZE;
				// �Ƿ��ͼ�ľ��κ�̹�˵ľ������㲻�ཻ���������������£�:https://zhuanlan.zhihu.com/p/29704064
				bool nonIntersect = (x2 <= t_x1+1) ||
					(x1 >= t_x2-1) ||
					(y2 <= t_y1+1) ||
					(y1 >= t_y2-1);
				if (nonIntersect == false) {
					return false; // ˵���������λ��ཻ
				}				
			}
		}
	}
	
	// �ж��Ƿ񳬹���Ϸ��ͼ�߽�
	if (x1 < 0 || x2 > CENTER_WIDTH || y1 < 0 || y2 > CENTER_HEIGHT) {
		return false;
	}
	return true;
}

/**
	�����ڵ�ײ���ϰ��������ש��ֻ��ը��������ש��ը������
*/
void check_bullet_to_obstacle(TankPlayer* tankPlayer) {
	if (tankPlayer->mBullet.needDraw == false) { // ���û�����ӵ�����Ҫ�ж�
		return;
	}
	int bulletSize[4][2] = { {4, 3}, {3, 4}, {4, 3}, {3, 4} }; // ���ң�4*3   ���£�3*4   

	int dir = tankPlayer->mBullet.dir;
	int x1 = tankPlayer->mBullet.posX;
	int y1 = tankPlayer->mBullet.posY;
	int x2 = x1 + bulletSize[dir][0];
	int y2 = y1 + bulletSize[dir][1];

	bool nonIntersect = false;
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			if (map26x26[i][j] == _WALL ||
				map26x26[i][j] == _STONE) { // �����ǽ��ʯͷ	
				int t_x1 = j * BOX_SIZE;
				int t_x2 = j * BOX_SIZE + BOX_SIZE;
				int t_y1 = i * BOX_SIZE;
				int t_y2 = i * BOX_SIZE + BOX_SIZE;
				// �Ƿ��ͼ�ľ��κ��ڵ��ľ������㲻�ཻ���������������£�:https://zhuanlan.zhihu.com/p/29704064
				nonIntersect = (x2 <= t_x1) ||
					(x1 >= t_x2) ||
					(y2 <= t_y1) ||
					(y1 >= t_y2); // ==true˵�����ཻ  ==false˵���ཻ
				
				if (nonIntersect == false) {
					tankPlayer->mBombStruct.showBomb = true;
					tankPlayer->mBullet.needDraw = false; // �ڵ���ʱ��ֹͣ����
					tankPlayer->mBombStruct.mBombX = (x1 + x2) / 2;
					tankPlayer->mBombStruct.mBombY = (y1 + y2) / 2;
					if (map26x26[i][j] == _WALL) { // ��ը�����������־
						map26x26[i][j] = _EMPTY; // ����Ϊ�յ�
					}
					return;
				}
			}
		}
	}	
}