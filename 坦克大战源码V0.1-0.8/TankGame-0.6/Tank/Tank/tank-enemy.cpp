#include "common.h"
#include "tank-enemy.h"

extern TankEnemy tankEnemyArr[MAX_TANK_ENEMY];
extern int mCurEnemyTankNum; // ��ǰ�����г��ֵ�̹������
extern int mTotalOutEnemyTank; // �ۼ��Ѿ����ֵĵл�̹��
extern HDC center_hdc; // �м���Ϸ���򣬷ֿ����Ʒ�����и���
extern char map26x26[26][26]; // ��ͼ����

int add_enemy_counter = 0;

/**
	����һ���л�̹��
*/
void tank_enemy_add() {
	if (add_enemy_counter++ % 100 != 0) { // ÿ��100*15ms����һ��̹��
		return;
	}
	add_enemy_counter = 0;
	
	if (mCurEnemyTankNum >= 6 || mTotalOutEnemyTank >= 20 || (mCurEnemyTankNum + mTotalOutEnemyTank) >= 20) { // ��ǰ����Ҫ����̹��
		return;
	}
	printf("before mTotalOutEnemyTank = %d, curOut = %d\n", mTotalOutEnemyTank, mCurEnemyTankNum);

	TankEnemy* pTankEnemy = &tankEnemyArr[mTotalOutEnemyTank]; // ��ȡ��ǰҪ������������̹�˶���
	
	pTankEnemy->mDied = false; // δ��
	pTankEnemy->mBorned = true; // �Ѿ�����
	pTankEnemy->mTankMoving = false;
	pTankEnemy->mTankImageDirIndex = 0;
	pTankEnemy->mTankX = pTankEnemy->bornPosX[rand() % 3];
	pTankEnemy->mTankY = BOX_SIZE;
	pTankEnemy->mMoveStep = rand() % 200; // ����ƶ��Ķ�ʱ������ֵ

	// ���õл�̹�˼���
	if (mTotalOutEnemyTank < 17) {
		pTankEnemy->mTankLevel = rand() % 3; // �������0��1��2����̹��
	}	
	else {
		pTankEnemy->mTankLevel = 3;
	}

	// ����̹����Դ������ֻ������ͨ�Ļ�ɫ̹�ˣ������ص���̹�˺�������ɫ������̹��
	TCHAR c[100];
	for (int dir = 0; dir < 4; dir++) { // i��ʾ�������̹�˵��ĸ�����
		_stprintf_s(c, L"./res/big/gray-tank/%d-%d-1.gif", pTankEnemy->mTankLevel + 1, dir + 1);
		loadimage(&pTankEnemy->mTankImage[dir][0], c);
		_stprintf_s(c, L"./res/big/gray-tank/%d-%d-2.gif", pTankEnemy->mTankLevel + 1, dir + 1);
		loadimage(&pTankEnemy->mTankImage[dir][1], c);
	}

	// ��ʼ���Ľ�����Դ��״̬
	TCHAR buf[100] = { 0 };
	// �����Ľ�����˸
	for (int i = 0; i < 4; i++)
	{
		_stprintf_s(buf, _T("./res/big/star%d.gif"), i);
		loadimage(&pTankEnemy->mStar.mStarImage[i], buf);
	}
	pTankEnemy->mStar.starState = Star_Begin; // �Ľ��Ǹճ���
	pTankEnemy->mStar.mStarCounter = 0;
	pTankEnemy->mStar.mStarIndex = 0;
	pTankEnemy->mStar.mStarIndexDir = 1; // ��ʼ�±�����С���

	// ��ʼ��̹���ƶ����ӵ��ٶȡ���ը�ٶȼ�����
	clock_init(&pTankEnemy->mTankMoveTimer, pTankEnemy->mMoveSpeedDev[pTankEnemy->mTankLevel]);
	clock_init(&pTankEnemy->mBulletTimer, TANK_ENEMY_BULLET_SPEED);
	clock_init(&pTankEnemy->mShootTimer, rand() % 1000 + 600); // ���õл�̹�˷����ڵ�������ʱ��ms��ֻ����һ�Σ�����̶�����

	// �����ڵ�ͼƬ��Դ
	TCHAR bulletBuf[100];
	for (int i = 0; i < 4; i++)
	{
		_stprintf_s(bulletBuf, _T("./res/big/bullet-%d.gif"), i);
		loadimage(&pTankEnemy->mBullet.mImage[i], bulletBuf);
	}
	pTankEnemy->mBullet.needDraw = false;

	// ���ر�ըЧ��ͼƬ
	for (int i = 0; i < 3; i++)
	{
		_stprintf_s(buf, _T("./res/big/bumb%d.gif"), i + 1);
		loadimage(&pTankEnemy->mBombStruct.bombImage[i], buf);
	}
	pTankEnemy->mBombStruct.bombCounter = 0;
	pTankEnemy->mBombStruct.showBomb = false;

	// ���صл������еĴ�ըЧ��
	pTankEnemy->mBlastStruct.showBlast = false;
	for (int i = 0; i < 5; i++)
	{
		_stprintf_s(buf, _T("./res/big/blast/%d.gif"), i);
		loadimage(&pTankEnemy->mBlastStruct.blastImage[i], buf);
	}

	// ���صл���ը֮����ʾ�ķ�����Դ
	loadimage(&pTankEnemy->mScoreImage[0], _T("./res/big/100.gif"));
	loadimage(&pTankEnemy->mScoreImage[1], _T("./res/big/200.gif"));
	loadimage(&pTankEnemy->mScoreImage[2], _T("./res/big/300.gif"));
	loadimage(&pTankEnemy->mScoreImage[3], _T("./res/big/400.gif"));

	mCurEnemyTankNum++;
	mTotalOutEnemyTank++; // ��󽫳��ֵ�̹������+1
	printf("mTotalOutEnemyTank = %d, curOut = %d\n", mTotalOutEnemyTank, mCurEnemyTankNum);
}

/**
	��ʾ�л�̹���Ľ���
*/
void tank_enemy_show_star() {
	for (int i = 0; i < mTotalOutEnemyTank; i++) {
		TankEnemy* pTankEnemy = &tankEnemyArr[i];
		if (pTankEnemy->mDied == false && pTankEnemy->mBorned == true) { // ֻ�ж���Щ�ճ�����δ�ҵ���̹��
			if (pTankEnemy->mStar.starState == Star_End) { // ���̹���Ѿ���������ʾ�Ľ���
				continue;
			}

			if (pTankEnemy->mStar.mStarCounter % 2 == 0) { // ��ѭ�������ż���μ����������Ľ���ͼƬ
				if (pTankEnemy->mStar.mStarIndex > 3) {
					pTankEnemy->mStar.mStarIndexDir = -1; // ��ʼ�ɴ��С
				}
				else if (pTankEnemy->mStar.mStarIndex < 0) {
					pTankEnemy->mStar.mStarIndexDir = 1; // ��ʼ��С���
				}

				pTankEnemy->mStar.mStarIndex += pTankEnemy->mStar.mStarIndexDir;
			}
			if (pTankEnemy->mStar.mStarCounter++ > 25) {
				pTankEnemy->mStar.starState = Star_End; // �Ľ�����˸���
				continue;
			}

			// ���ռ�����±�����Ľ���ͼƬ����
			TransparentBlt(center_hdc,
				pTankEnemy->mTankX - BOX_SIZE, pTankEnemy->mTankY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
				GetImageHDC(&pTankEnemy->mStar.mStarImage[pTankEnemy->mStar.mStarIndex]),
				0, 0, BOX_SIZE * 2, BOX_SIZE * 2,
				0x000000);
		}
	}
}

/**
	���Ƶл�̹�˺��ڵ�
*/
void tank_enemy_draw_tank() {
	for (int i = 0; i < mTotalOutEnemyTank; i++) {
		TankEnemy* pTankEnemy = &tankEnemyArr[i];
		if (pTankEnemy->mDied == false && 
			pTankEnemy->mBorned == true &&
			pTankEnemy->mStar.starState == Star_End) { // ���ڻ���״̬���Ѿ����ֹ��Ľ��ǵ�̹��
			
			pTankEnemy->mTankImageDirIndex = pTankEnemy->mTankImageDirIndex == 0 ? 1 : 0; // �л�̹���ƶ�����ͼƬ
			IMAGE tankImg = pTankEnemy->mTankImage[pTankEnemy->dir][pTankEnemy->mTankImageDirIndex];
			TransparentBlt(center_hdc, pTankEnemy->mTankX - BOX_SIZE, pTankEnemy->mTankY - BOX_SIZE,
				BOX_SIZE * 2, BOX_SIZE * 2,
				GetImageHDC(&tankImg),
				0, 0,
				BOX_SIZE * 2, BOX_SIZE * 2,
				0x000000);

			// ����ڵ��Ƿ������ϰ�������ϰ����ֹͣ�����ڵ���֪ͨ������ըЧ��
			check_enemy_bullet_to_obstacle(pTankEnemy);
			
			// �Ƿ���Ҫ��ʾ�ڵ��ı�ըЧ��
			if (pTankEnemy->mBombStruct.showBomb) { // ��Ҫ��ʾ��ըЧ��
				int index[6] = { 0,1,1,2,2,1 };	// ͼƬ�±��л�˳��
				if (pTankEnemy->mBombStruct.bombCounter >= 6) {
					pTankEnemy->mBombStruct.bombCounter = 0;
					pTankEnemy->mBombStruct.showBomb = false;
					pTankEnemy->mBullet.needDraw = false; // ǰһ���ڵ���ը֮���ٿ�ʼ��Ӧ����µķ����ڵ�����
				}
				else {
					TransparentBlt(center_hdc,
						pTankEnemy->mBombStruct.mBombX - BOX_SIZE, pTankEnemy->mBombStruct.mBombY - BOX_SIZE,
						BOX_SIZE * 2, BOX_SIZE * 2,
						GetImageHDC(&pTankEnemy->mBombStruct.bombImage[index[pTankEnemy->mBombStruct.bombCounter++]]),
						0, 0,
						BOX_SIZE * 2, BOX_SIZE * 2,
						0x000000);
				}
			}

			// �Ƿ���Ҫ���Ƶл�̹���ڵ�
			if (pTankEnemy->mBullet.needDraw) {
				// �������̹���ڵ�
				int dir = pTankEnemy->mBullet.dir; // �ڵ�����
				int bulletX = pTankEnemy->mBullet.posX + pTankEnemy->mBullet.bullet_bias[dir][0]; // ���ڵ����Ͻǵ�λ����̹�˵������ƶ���̹���ڹܵ�λ��
				int bulletY = pTankEnemy->mBullet.posY + pTankEnemy->mBullet.bullet_bias[dir][1];
				TransparentBlt(center_hdc,
					bulletX, bulletY,
					pTankEnemy->mBullet.bulletSize[dir][0], pTankEnemy->mBullet.bulletSize[dir][1],
					GetImageHDC(&pTankEnemy->mBullet.mImage[pTankEnemy->mBullet.dir]),
					0, 0,
					pTankEnemy->mBullet.bulletSize[dir][0], pTankEnemy->mBullet.bulletSize[dir][1],
					0x000000);
			}
		}

		// �л�̹�����������ƴ�ըЧ��
		if (pTankEnemy->mDied == true && pTankEnemy->mBlastStruct.showBlast == true) {
			int index[13] = { 0,1,1,2,2,3,3,4,4,3,2,1,0 };
			if (pTankEnemy->mBlastStruct.blastCounter >= 12) {
				if (pTankEnemy->mBlastStruct.blastCounter++ <= 18) {
					// ���Ƶл�̹�˷���ͼƬ
					TransparentBlt(center_hdc, 
						pTankEnemy->mBlastStruct.blastX - 7, pTankEnemy->mBlastStruct.blastY - 3,
						14, 7,
						GetImageHDC(&pTankEnemy->mScoreImage[pTankEnemy->mTankLevel]), 
						0, 0, 
						14, 7, 
						0x000000);
				}
				else {
					pTankEnemy->mBlastStruct.blastCounter = 0;
					pTankEnemy->mBlastStruct.showBlast = false;
				}							
			}
			else {
				TransparentBlt(center_hdc,
					pTankEnemy->mBlastStruct.blastX - BOX_SIZE*2, pTankEnemy->mBlastStruct.blastY - BOX_SIZE*2,
					BOX_SIZE * 4, BOX_SIZE * 4,
					GetImageHDC(&pTankEnemy->mBlastStruct.blastImage[index[pTankEnemy->mBlastStruct.blastCounter++]]),
					0, 0,
					BOX_SIZE * 4, BOX_SIZE * 4,
					0x000000);
			}

		}
	}
}

/**
	�л�̹�����µ�������
*/
void tank_enemy_rejust_direction(TankEnemy* pTankEnemy) {
	if (pTankEnemy->dir == ENE_DIR_LEFT || pTankEnemy->dir == ENE_DIR_RIGHT) {
		bool val = rand() % 100 < 70;
		if (val) {
			pTankEnemy->dir = ENE_DIR_DOWN; // ���õл�̹�˴���������ƶ�
		}
		else {
			pTankEnemy->dir = ENE_TANK_DIR(rand() % 4); // ǿ�ƽ�intת����enumö������
		}
	}
	else {
		pTankEnemy->dir = ENE_TANK_DIR(rand() % 4);
	}
}

/**
	�жϵл�̹���Ƿ����ͨ��ĳһ������
	tankX��tankY��̹����һ��Ҫ�ƶ������꣬�������ͨ�������겻��
*/
bool check_tank_enemy_can_pass(int dir, int tankX, int tankY) {	
	switch (dir) {
	case ENE_DIR_LEFT:
		tankX += -1;		
		break;
	case ENE_DIR_UP:
		tankY += -1;		
		break;
	case ENE_DIR_RIGHT:
		tankX += 1;
		break;
	case ENE_DIR_DOWN:
		tankY += 1;
		break;
	default:
		break;
	}

	int x1 = tankX - BOX_SIZE; // ���㽫Ҫ�ƶ���λ�õ�̹�˾������ϽǺ����½�����
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
				bool nonIntersect = (x2 <= t_x1 + 1) ||
					(x1 >= t_x2 - 1) ||
					(y2 <= t_y1 + 1) ||
					(y1 >= t_y2 - 1);
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
void check_enemy_bullet_to_obstacle(TankEnemy* pTankEnemy) {
	if (pTankEnemy->mBullet.needDraw == false) { // ���û�����ӵ�����Ҫ�����ж�
		return;
	}
	int bulletSize[4][2] = { {4, 3}, {3, 4}, {4, 3}, {3, 4} }; // ���ң�4*3   ���£�3*4   

	int dir = pTankEnemy->mBullet.dir;
	int x1 = pTankEnemy->mBullet.posX; // �ڵ����Ͻǵ�����(x1, y1)
	int y1 = pTankEnemy->mBullet.posY;
	int x2 = x1 + bulletSize[dir][0];
	int y2 = y1 + bulletSize[dir][1];

	switch (dir) { // �����ڵ���ͬ����ĺ���棬��������ͬʱ��������ש��
	case ENE_DIR_LEFT:
		y1 = y1 - bulletSize[dir][1];
		break;
	case ENE_DIR_UP:
		x1 = x1 - bulletSize[dir][0];
		break;
	case ENE_DIR_RIGHT:
		y1 = y1 - bulletSize[dir][1];
		break;
	case ENE_DIR_DOWN:
		x1 = x1 - bulletSize[dir][0];
		break;
	default:
		break;
	}

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
					pTankEnemy->mBombStruct.showBomb = true;	
					pTankEnemy->mBullet.needDraw = false; // �����ڵ������˶�������α�ը��BUG
					pTankEnemy->mBombStruct.mBombX = (x1 + x2) / 2;
					pTankEnemy->mBombStruct.mBombY = (y1 + y2) / 2;
					if (map26x26[i][j] == _WALL) { // ��ը�����������־
						map26x26[i][j] = _EMPTY; // ����Ϊ�յ�
					}
				}
			}
		}
	}
}