#include "common.h"
#include "game-control.h"
#include "game-clock.h"
#include "tank-player.h"

extern HDC main_hdc, canvas_hdc;
extern IMAGE mGrayBackgroundImage;		// ��Ϸ��ɫ����ͼ
extern IMAGE mBlackNumberImage;		// 0123456789 ��ǰ�ؿ���

extern TimeClock mainTimer; // ���������ڳ���Ľ��涨ʱ����

IMAGE center_img; // ���ڱ���center_hdc���ڵ���Ч��
HDC center_hdc; // �м���Ϸ���򣬷ֿ����Ʒ�����и���

IMAGE mBlackBackgroundImage;	// ��ɫ����ͼ

IMAGE mStoneImage;				// ʯͷ
IMAGE mForestImage;				// ����
IMAGE mIceImage;				// ����
IMAGE mRiverImage[2];			// ����
IMAGE mWallImage;				// ��ǽ
IMAGE mCamp[2];					// ��Ӫ

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

TankPlayer tankPlayer0;

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
}

/**
	������Ϸ�ؿ���Ԥ�Ȼ���
*/
void game_control_show_stage() {
	// ���ſ�ʼ��Ч
	PlaySounds(S_START); 

	// ���º�£��Ч
	StretchBlt(canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, 
		GetImageHDC(&mGrayBackgroundImage), 0, 0, 66, 66, SRCCOPY); // ��ɫ������ͼ
	
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

	// ����Ľ�����˸����
	tank_player_show_star(&tankPlayer0); // �Ľ�����˸���֮��״̬����ΪStar_End

	// ��ʼ���Ƶ�ͼ
	int i = 0, j = 0;
	int x = 0, y = 0;
	for (i = 0; i < 26; i++) {
		for (j = 0; j < 26; j++) {
			x = j * BOX_SIZE;
			y = i * BOX_SIZE;
			switch (map[i][j]-'0') {
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

			// ����ɭ��
			if ((map[i][j] - '0') == _FOREST) {
				TransparentBlt(center_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mForestImage), 0, 0, BOX_SIZE, BOX_SIZE, 0x000000);
			}
		}
	}
}

/**
	�����ұߵķ����ؿ�������
*/
void game_control_right_panel() {
	// ���û�ɫ����
	StretchBlt(canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, GetImageHDC(&mGrayBackgroundImage), 0, 0, 66, 66, SRCCOPY);

	// ��ʾʣ��л���ͼ��
	for (int i = 0; i < mRemainEnemyTankNumber; i++) {
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
	GameResult result = Victory;

	// ��ʼ�����0�Ľṹ��
	tank_player_init(&tankPlayer0, 0, 4*16+BOX_SIZE, 12*16+BOX_SIZE, 240, 137);

	// ��ʱ����ʼ��
	clock_init(&mainTimer, 15); // ������15msˢ��һ��

	while (result != Fail) {
		result = game_control_start_game();
		Sleep(1);
	}
}

/**
	���н�����ҡ��ӵ����л�������������Ϣ�ĸ���
*/
GameResult game_control_start_game() {
	if (clock_is_timeout(&mainTimer)) {

		// ʤ������ʧ�ܣ���ʾ�������

		// ���ʧ�ܣ�����GAME OVER����

		// ��ӵл�̹��

		// �����м���Ϸ��������
		game_control_center_panel();

		// �����ұ���Ϣ��
		game_control_right_panel();

		// ���м��ͼ��center_hdc���Ƶ�����canvas_hdc��
		BitBlt(canvas_hdc, CENTER_X, CENTER_Y, CENTER_WIDTH, CENTER_HEIGHT, center_hdc, 0, 0, SRCCOPY);
		// ������canvas_hdc������ʾ��main��������
		StretchBlt(main_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, SRCCOPY);
		FlushBatchDraw();
	}

	return Victory;
}