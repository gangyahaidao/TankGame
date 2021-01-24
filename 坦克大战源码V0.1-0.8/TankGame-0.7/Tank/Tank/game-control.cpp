#include "common.h"
#include "game-control.h"
#include "game-clock.h"
#include "tank-player.h"
#include "tank-enemy.h"

extern HDC main_hdc, canvas_hdc;
extern IMAGE mGrayBackgroundImage;		// ��Ϸ��ɫ����ͼ
extern IMAGE mBlackNumberImage;		// 0123456789 ��ǰ�ؿ���

extern TimeClock mainTimer; // ���������ڳ���Ľ��涨ʱ����

GameResult result = Victory; // ��Ϸ������������ؿ����¿�ʼѭ������

IMAGE center_img; // ���ڱ���center_hdc���ڵ���Ч��
HDC center_hdc; // �м���Ϸ���򣬷ֿ����Ʒ�����и���

IMAGE mBlackBackgroundImage;	// ��ɫ����ͼ

IMAGE mStoneImage;				// ʯͷ
IMAGE mForestImage;				// ����
IMAGE mIceImage;				// ����
IMAGE mRiverImage[2];			// ����
IMAGE mWallImage;				// ��ǽ
IMAGE mCamp[2];					// ��Ӫ
bool mCampDie;					// ��Ӫ�Ƿ񱻻���
bool showCampBomb = false;      // ��ʾ��Ӫ�����б�ըЧ��
int campBombCounter = 0;

bool showGameOver = false;      // ������ʾGame Over����
int gameOverCounter = 0;
int gameOverX, gameOverY;		// ͼƬ���Ͻ�����

IMAGE mEnemyTankIcoImage;		// �л�̹��ͼ��
IMAGE mFlagImage;				// ����
IMAGE m12PImage;
IMAGE mPlayerTankIcoImage;

int mCutStageCounter;
IMAGE mCurrentStageImage;		// STAGE ����
IMAGE mGameOverImage;			
int mRemainEnemyTankNumber = 20;		// ����δ��ʾ�ĵл�����, ��ʼֵ=20, ����һ�ܵл���ֵ��һ

int mPlayerLife = 3; // ���Ĭ������ֵΪ3
int mCurrentStage = 1; // ��ǰ�ؿ�

TankPlayer tankPlayer0; // �������̹�˽ṹ
TankEnemy tankEnemyArr[MAX_TANK_ENEMY]; // ����л�̹�������б����ڿ������޸�Ϊ����ṹ
int mCurEnemyTankNum = 0; // ��ǰ�����г��ֵ�̹������
int mTotalOutEnemyTank = 0; // �ۼ��Ѿ����ֵĵл�̹��

const char* map[] = { // ��һ�ص�ͼ���ݣ�0�յء�1���֡�2����3ǽ��4������5ʯͷ
		"00000000000000000000000000",
		"00000000000000000000000000",
		"00330033003300330033003300",
		"00330033003300330033003300",
		"00330033003300330033003300",
		"00330033003300330033003300",
		"00330033003355330033003300",
		"00330033003355330033003300",
		"00330033003300330033003300",
		"00330033000000000033003300",
		"00330033000000000033003300",
		"00000000003300330000000000",
		"00000000003300330000000000",
		"33003333000000000033330033",
		"55003333000000000033330055",
		"00000000003300330000000000",
		"00000000003333330000000000",
		"00330033003333330033003300",
		"00330033003300330033003300",
		"00330033003300330033003300",
		"00330033003300330033003300",
		"00330033000000000033003300",
		"00330033000000000033003300",
		"00330033000333300033003300",
		"00000000000300300000000000",
		"00000000000300300000000000" };
char map26x26[26][26] = { 0 }; // ���ڴ洢ÿ����ͼ����״̬

/**
	��Ϸ������Դ��ʼ��
*/
void game_control_init() {
	center_img.Resize(CENTER_WIDTH, CENTER_HEIGHT);
	center_hdc = GetImageHDC(&center_img);

	loadimage(&mBlackBackgroundImage, _T("./res/big/bg_black.gif"));	// ��ɫ����
	loadimage(&mStoneImage, _T("./res/big/stone.gif"));					// 12*12��ʯͷ
	loadimage(&mForestImage, _T("./res/big/forest.gif"));				// ����
	loadimage(&mIceImage, _T("./res/big/ice.gif"));						// ����
	loadimage(&mRiverImage[0], _T("./res/big/river-0.gif"));			// ����
	loadimage(&mRiverImage[1], _T("./res/big/river-1.gif"));
	loadimage(&mWallImage, _T("./res/big/wall.gif"));					// ��ǽ
	loadimage(&mCamp[0], _T("./res/big/camp0.gif"));					// ��Ӫ
	loadimage(&mCamp[1], _T("./res/big/camp1.gif"));	 
	loadimage(&mEnemyTankIcoImage, _T("./res/big/enemytank-ico.gif"));	// �л�ͼ��
	loadimage(&mFlagImage, _T("./res/big/flag.gif"));					// ����
	loadimage(&mCurrentStageImage, _T("./res/big/stage.gif"));
	loadimage(&mGameOverImage, _T("./res/big/gameover.gif"));

	// ���ͼ��
	loadimage(&m12PImage, _T("./res/big/1P.gif"));		// 1P\2Pͼ��
	loadimage(&mPlayerTankIcoImage, _T("./res/big/playertank-ico.gif"));	// ���̹��ͼ��

	mCampDie = false;

	// ��ʼ����ͼ
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			map26x26[i][j] = map[i][j] - '0';
		}
	}
}

/**
	������Ϸ�ؿ���Ԥ�Ȼ���
*/
void game_control_show_stage() {
	// ���ſ�ʼ��Ч
	PlaySounds(S_START); 

	// ���º�£��Ч
	StretchBlt(canvas_hdc, 
		0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, 
		GetImageHDC(&mGrayBackgroundImage), 
		0, 0, 66, 66, SRCCOPY); // ��ɫ������ͼ
	int mCutStageCounter = 0;
	while (mCutStageCounter < 110) {
		Sleep(6);
		mCutStageCounter += 3;
		StretchBlt(canvas_hdc, 0, 0, CANVAS_WIDTH, mCutStageCounter,
			GetImageHDC(&mBlackBackgroundImage), 0, 0, CENTER_WIDTH, CENTER_HEIGHT, SRCCOPY); // �ϰ벿��
		StretchBlt(canvas_hdc, 0, CANVAS_HEIGHT-mCutStageCounter, CANVAS_WIDTH, CANVAS_HEIGHT,
			GetImageHDC(&mBlackBackgroundImage), 0, 0, CENTER_WIDTH, CENTER_HEIGHT, SRCCOPY); // �°벿��
		StretchBlt(main_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, SRCCOPY); // ��canvas����main������
		FlushBatchDraw();
	}

	// ��ʾ�ؿ�ҳ�� Stage 1/2/3/4....
	StretchBlt(canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT,
		GetImageHDC(&mGrayBackgroundImage), 0, 0, 66, 66, SRCCOPY); // ��ɫ������ͼ
	TransparentBlt(canvas_hdc, 97, 103, 39, 7, GetImageHDC(&mCurrentStageImage), 0, 0, 39, 7, 0xffffff); // ����Stage����ͼƬ�����һ������ָ��͸��ɫ

	if (mCurrentStage < 10) { // �ؿ�<10
		TransparentBlt(canvas_hdc, 157, 103, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
			GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * mCurrentStage, 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);
	}
	else {
		TransparentBlt(canvas_hdc, 157, 103, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
			GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * (mCurrentStage / 10), 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);

		TransparentBlt(canvas_hdc, 157, 103, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
			GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * (mCurrentStage % 10), 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);
	}
	StretchBlt(main_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, SRCCOPY); // ��canvas����main������
	FlushBatchDraw();

	Sleep(1200);

	PlaySounds(S_BK);
}

/**
	���ص�ͼ���л���
*/
void game_control_center_panel() {
	BitBlt(center_hdc, 0, 0, CENTER_WIDTH, CENTER_HEIGHT, GetImageHDC(&mBlackBackgroundImage), 0, 0, SRCCOPY);// ���ĺ�ɫ������Ϸ��

	// ��ʼ���Ƶ�ͼ
	int x = 0, y = 0;
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			x = j * BOX_SIZE;
			y = i * BOX_SIZE;
			switch (map26x26[i][j]) {
			case _WALL:
				BitBlt(center_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mWallImage), 0, 0, SRCCOPY);
				break;
			case _ICE:
				BitBlt(center_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mIceImage), 0, 0, SRCCOPY);
				break;
			case _RIVER:
				BitBlt(center_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mRiverImage[0]), 0, 0, SRCCOPY);
				break;
			case _STONE:
				BitBlt(center_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mStoneImage), 0, 0, SRCCOPY);
				break;
			default:
				break;
			}
		}
	}

	// ����Ľ�����˸����
	tank_player_show_star(&tankPlayer0); // �Ľ�����˸���֮��״̬����ΪStar_End
	// �л�̹���Ľ�����˸
	tank_enemy_show_star();

	// �������̹�ˡ��������������ڵ��˶����ڵ�ͼ֮����ƣ��ڵ����ں��Ϸɹ�Ч��
	tank_player_draw_tank(&tankPlayer0);
	// ���Ƶл�̹�˺��ڵ�
	tank_enemy_draw_tank();

	// ����ɭ�֣�ɭ�ֿ��Ը�����̹����
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			if (map26x26[i][j] == _FOREST) {
				TransparentBlt(center_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mForestImage), 0, 0, BOX_SIZE, BOX_SIZE, 0x000000);
			}
		}
	}

	// ���ƴ�Ӫ
	int campIndex = 0;
	if (mCampDie) {
		campIndex = 1;
	}
	TransparentBlt(center_hdc,
		BOX_SIZE * 12, BOX_SIZE * 24,
		BOX_SIZE * 2, BOX_SIZE * 2,
		GetImageHDC(&mCamp[campIndex]),
		0, 0,
		BOX_SIZE * 2, BOX_SIZE * 2,
		0x000000);
	// �����Ӫ�����٣�����ʾ��ըЧ����ͬʱ����"Game Over"����
	if (showCampBomb) {
		int index[26] = { 0,0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,4,4,3,3,2,2,1,1,0,0 };
		TransparentBlt(center_hdc, 10 * BOX_SIZE + BOX_SIZE, 22 * BOX_SIZE + BOX_SIZE, 
			BOX_SIZE * 4, BOX_SIZE * 4,
			GetImageHDC(&tankPlayer0.mBlastStruct.blastImage[campBombCounter++]), // ����һ�����̹�˴�ը��ͼƬ
			0, 0, 
			BOX_SIZE * 4, BOX_SIZE * 4, 
			0x000000);
		if (campBombCounter >= 26) {
			campBombCounter = 0;
			showCampBomb = false;
			showGameOver = true; // ��ʼ��ʾGameOver
		}
	}
	
	if (showGameOver) {		
		gameOverCounter++;
		if (gameOverCounter <= 120) {
			gameOverY -= 1;
		}
		else if (gameOverCounter >= 220) { // ͣ��Լ1��
			gameOverCounter = 0;
			showGameOver = false;
			result = Fail; // ��Ϸ��������ʼ����ѡ��
		}
		StretchBlt(center_hdc,
			gameOverX, gameOverY, GAME_OVER_WIDTH, GAME_OVER_HEIGHT,
			GetImageHDC(&mGameOverImage),
			0, 0, GAME_OVER_WIDTH, GAME_OVER_HEIGHT, SRCCOPY);
	}
	
}

/**
	�����ұߵķ����ؿ�������
*/
void game_control_right_panel() {
	// ���û�ɫ����
	StretchBlt(canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, GetImageHDC(&mGrayBackgroundImage), 0, 0, 66, 66, SRCCOPY);

	// ��ʾʣ��л���ͼ��
	for (int i = 0; i < MAX_TANK_ENEMY- mTotalOutEnemyTank; i++) {
		int x = i % 2 == 0 ? 233 : 241; // ����л�ͼ��x����
		TransparentBlt(canvas_hdc, x, 19 + i / 2 * 8, ENEMY_TANK_ICO_SIZE, ENEMY_TANK_ICO_SIZE,
			GetImageHDC(&mEnemyTankIcoImage), 0, 0, ENEMY_TANK_ICO_SIZE, ENEMY_TANK_ICO_SIZE, 0xffffff);
	}

	// ��ʾ���������Ϣ
	TransparentBlt(canvas_hdc, 233, 129, PLAYER_12_ICO_SIZE_X, PLAYER_12_ICO_SIZE_Y,
		GetImageHDC(&m12PImage), 0, 0, PLAYER_12_ICO_SIZE_X, PLAYER_12_ICO_SIZE_Y, 0xffffff); // ����1P/2Pͼ��
	TransparentBlt(canvas_hdc, 233, 137, PLAYER_TANK_ICO_SIZE_X, PLAYER_TANK_ICO_SIZE_Y,
		GetImageHDC(&mPlayerTankIcoImage), 0, 0, PLAYER_TANK_ICO_SIZE_X, PLAYER_TANK_ICO_SIZE_Y, 0x000000); // ����̹��ͼ��
	TransparentBlt(canvas_hdc, 240, 137, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
		GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * mPlayerLife, 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff); // �����������ֵ

	// ���ƹؿ����Ӻ͹ؿ���
	TransparentBlt(canvas_hdc, 232, 177, FLAG_ICO_SIZE_X, FLAG_ICO_SIZE_Y,
		GetImageHDC(&mFlagImage), 0, 0, FLAG_ICO_SIZE_X, FLAG_ICO_SIZE_Y, 0xffffff);

	if (mCurrentStage < 10) { // �ؿ�<10
		TransparentBlt(canvas_hdc, 238, 193, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
			GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * mCurrentStage, 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);
	}
	else {
		TransparentBlt(canvas_hdc, 233, 193, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
			GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * (mCurrentStage / 10), 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);

		TransparentBlt(canvas_hdc, 241, 193, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
			GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * (mCurrentStage % 10), 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);
	}
}

/**
	��ʼ��Ϸѭ����
*/
void game_control_loop() {	
	result = Victory;
	// ��ʼ�����0�Ľṹ��
	tank_player_init(&tankPlayer0, 0, 4*16+BOX_SIZE, 12*16+BOX_SIZE, 240, 137);

	// ����ʱ����ʼ��
	clock_init(&mainTimer, 15); // ������15msˢ��һ��

	while (result != Fail) {
		game_control_start_game();
		Sleep(1);
	}
}

/**
	���н�����ҡ��ӵ����л�������������Ϣ�ĸ���
*/
void game_control_start_game() {
	if (clock_is_timeout(&mainTimer)) {
		// �����м���Ϸ����
		game_control_center_panel();

		// �����ұ���Ϣ��
		game_control_right_panel();

		// ��ӵл�̹�ˣ�ֻ��ʼ����Դ���������л���
		tank_enemy_add();

		// ���м��ͼ��center_hdc���Ƶ�����canvas_hdc��
		BitBlt(canvas_hdc, CENTER_X, CENTER_Y, CENTER_WIDTH, CENTER_HEIGHT, center_hdc, 0, 0, SRCCOPY);
		// ������canvas_hdc������ʾ��main��������
		StretchBlt(main_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, SRCCOPY);
		FlushBatchDraw();
	}

	// ���²���ֻ����̹�����ݣ������л�ͼ����ͼ��mainTimer��ͳһ����
	// ���ռ��̿��ƣ�ע�⣺������ѭ����ʱ���У�����ʵʱ����
	if (GetAsyncKeyState('A') & 0x8000) { // ����		
		tankPlayer0.tankDir = DIR_LEFT;
		if (tankPlayer0.mTankMoving == false) {
			PlaySounds(S_PLAYER_MOVE);
		}		
		tankPlayer0.mTankMoving = true;
		tank_player_move_by_tanktimer(&tankPlayer0);
	}
	else if (GetAsyncKeyState('W') & 0x8000) {
		tankPlayer0.tankDir = DIR_UP;
		if (tankPlayer0.mTankMoving == false) {
			PlaySounds(S_PLAYER_MOVE);
		}
		tankPlayer0.mTankMoving = true;
		tank_player_move_by_tanktimer(&tankPlayer0);
	}
	else if (GetAsyncKeyState('D') & 0x8000) {
		tankPlayer0.tankDir = DIR_RIGHT;
		if (tankPlayer0.mTankMoving == false) {
			PlaySounds(S_PLAYER_MOVE);
		}
		tankPlayer0.mTankMoving = true;
		tank_player_move_by_tanktimer(&tankPlayer0);
	}
	else if (GetAsyncKeyState('S') & 0x8000) {
		tankPlayer0.tankDir = DIR_DOWN;
		if (tankPlayer0.mTankMoving == false) {
			PlaySounds(S_PLAYER_MOVE);
		}
		tankPlayer0.mTankMoving = true;
		tank_player_move_by_tanktimer(&tankPlayer0);
	}
	else if(tankPlayer0.mTankMoving == true) { // ���û�а������������ͣ�����ƶ�������
		tankPlayer0.mTankMoving = false;
		PlaySounds(S_PLAYER_STOP_MOVE);
	}

	// �ж��Ƿ񰴼�J����ǰһ���ڵ��Ѿ���ը
	if (GetAsyncKeyState('J') & 0x8000 && tankPlayer0.mBullet.needDraw==false) {
		tankPlayer0.mBullet.dir = tankPlayer0.tankDir; // �������ڵ�ʱ̹�˵ķ���һ��
		tankPlayer0.mBullet.posX = tankPlayer0.tankPlayerX;
		tankPlayer0.mBullet.posY = tankPlayer0.tankPlayerY;
		tankPlayer0.mBullet.needDraw = true;
		PlaySounds(S_SHOOT0);
	}

	// ��������ڵ���ʱ�������ڵ��˶����ݣ�Ȼ��������ʱ���н��л���
	if (tankPlayer0.mBullet.needDraw) { // �ж��Ƿ���Ҫ�����ڵ�
		if (clock_is_timeout(&tankPlayer0.mBulletTimer)) { // ���̹�˼���ͬ���ڵ��ٶȲ�һ��
			switch (tankPlayer0.mBullet.dir) {
			case DIR_LEFT: 
				tankPlayer0.mBullet.posX -= tankPlayer0.mBullet.speed[tankPlayer0.mTankLevel];
				break;
			case DIR_UP:
				tankPlayer0.mBullet.posY -= tankPlayer0.mBullet.speed[tankPlayer0.mTankLevel];
				break;
			case DIR_RIGHT:
				tankPlayer0.mBullet.posX += tankPlayer0.mBullet.speed[tankPlayer0.mTankLevel];
				break;
			case DIR_DOWN:
				tankPlayer0.mBullet.posY += tankPlayer0.mBullet.speed[tankPlayer0.mTankLevel];
				break;
			default:
				break;
			}
			if (tankPlayer0.mBullet.posX <= 0 || tankPlayer0.mBullet.posX >= CENTER_WIDTH
				|| tankPlayer0.mBullet.posY <= 0
				|| tankPlayer0.mBullet.posY >= CENTER_HEIGHT) {

				tankPlayer0.mBullet.needDraw = false; // ֹͣ�����ڵ�

				// ����Ǵ��ڱ߽������б�ըЧ����_BIN��Ч
				tankPlayer0.mBombStruct.showBomb = true;
				tankPlayer0.mBombStruct.mBombX = tankPlayer0.mBullet.posX;
				tankPlayer0.mBombStruct.mBombY = tankPlayer0.mBullet.posY;
				PlaySounds(S_BIN);				
			}
		}
	}

	// ���Ƶл�̹��
	// �л�̹���Զ���������ƶ�
	for (int i = 0; i < mTotalOutEnemyTank; i++) {
		TankEnemy* pTankEnemy = &tankEnemyArr[i];
		
		if (pTankEnemy->mDied == false && pTankEnemy->mBorned == true && pTankEnemy->mStar.starState == Star_End) { // ����л�̹�˻�������˸���
			// ��ʱ�ƶ������������ʱʱ�䵽���������ϰ������µ�������
			if (clock_is_timeout(&pTankEnemy->mTankMoveTimer)) { // ����л��������Ѿ������Ҷ�ʱ�ƶ�ʱ�䵽
				// ����ƶ�һ�����������������ϰ����ٱ任�������¿�ʼ����
				if (pTankEnemy->mMoveStep-- < 0) {
					pTankEnemy->mMoveStep = rand() % 250;
					tank_enemy_rejust_direction(pTankEnemy); // ���µ�������
				}

				if (check_tank_enemy_can_pass(pTankEnemy->dir, pTankEnemy->mTankX, pTankEnemy->mTankY) == true) { // ��������ƶ�
					switch (pTankEnemy->dir) {
					case ENE_DIR_LEFT:
						pTankEnemy->mTankX += -1;
						break;
					case ENE_DIR_UP:
						pTankEnemy->mTankY += -1;
						break;
					case ENE_DIR_RIGHT:
						pTankEnemy->mTankX += 1;
						break;
					case ENE_DIR_DOWN:
						pTankEnemy->mTankY += 1;
						break;
					default:
						break;
					}
				}
				else { // ���µ�������
					tank_enemy_rejust_direction(pTankEnemy);
				}
			}

			// �л�̹���Զ����Ƶ���������ǰһ���ڵ��Ѿ���ը
			if (clock_is_timeout(&pTankEnemy->mShootTimer) && pTankEnemy->mBullet.needDraw == false) {
				pTankEnemy->mBullet.dir = pTankEnemy->dir;
				pTankEnemy->mBullet.posX = pTankEnemy->mTankX;
				pTankEnemy->mBullet.posY = pTankEnemy->mTankY;
				pTankEnemy->mBullet.needDraw = true; // ֪ͨ���ƺ�������Ҫ��ʼ�����ڵ����л����ڲ�������Ч
			}

			// ����������Ҫ�����ڵ������꣬������ڱ߽���ֹͣ���ƣ���֪ͨ���Ʊ�ըЧ��
			if (pTankEnemy->mBullet.needDraw) {
				if (clock_is_timeout(&pTankEnemy->mBulletTimer)) { // �л��ڵ����и��¼��ʱ�䶼��30ms
					switch (pTankEnemy->mBullet.dir) {
					case DIR_LEFT:
						pTankEnemy->mBullet.posX -= TANK_ENEMY_POS_SPEED;
						break;
					case DIR_UP:
						pTankEnemy->mBullet.posY -= TANK_ENEMY_POS_SPEED;
						break;
					case DIR_RIGHT:
						pTankEnemy->mBullet.posX += TANK_ENEMY_POS_SPEED;
						break;
					case DIR_DOWN:
						pTankEnemy->mBullet.posY += TANK_ENEMY_POS_SPEED;
						break;
					default:
						break;
					}
					// �ڵ������߽�
					if (pTankEnemy->mBullet.posX <= 0 || pTankEnemy->mBullet.posX >= CENTER_WIDTH
						|| pTankEnemy->mBullet.posY <= 0
						|| pTankEnemy->mBullet.posY >= CENTER_HEIGHT) {

						pTankEnemy->mBullet.needDraw = false; // ֹͣ�����ڵ�

						// ����Ǵ��ڱ߽���ֻ�б�ըЧ��
						pTankEnemy->mBombStruct.showBomb = true;
						pTankEnemy->mBombStruct.mBombX = pTankEnemy->mBullet.posX;
						pTankEnemy->mBombStruct.mBombY = pTankEnemy->mBullet.posY;
					}
				}
			}
		}		
	}
}