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

// ���̹�˳���������
typedef struct {
	IMAGE protecImage[2]; // ������ͼƬ
	bool needShow; // �Ƿ���Ҫ��ʾ������
	int imageIndex; // ͼƬ�±�
	int timerCount; // ��ʱ������
}ProtecCircle;

struct TankPlayer {
	bool mDied; // �������ֵ�Ƿ�����
	byte playerId; // ���ID��0/1
	byte mTankLevel; // ̹�˵ȼ�[0-3]
	IMAGE mTankImage[4][4][2]; // ���ּ����ĸ�����ÿ������������ʽʵ��̹���Ĵ�ת����Ч��
	byte mTankImageDirIndex = 0; // ͬһ������Ĳ�ͬͼ���±�
	bool mTankMoving; // ̹���Ƿ����ƶ������ڻ�ȡͬһ������ͬ��̹��ͼƬ������ʱ�����л������ж�̬Ч��
	int tankPlayerX, tankPlayerY; // ̹�����ĵ�����
	int tankWidth, tankHeight; // ̹�˵ĳ��Ϳ�
	TANK_DIR tankDir; // ̹�˷���0-��1-�ϡ�2-�ҡ�3-�£�����ԴͼƬ��Ӧ

	int mPlayerLife; // ���Ĭ������ֵΪ3
	int mPlayerIconLife_x, 						// ͼ������
		mPlayerIconLife_y;

	struct StarFourPoint mStar;			// �Ľ�����˸��
	ProtecCircle mProtecCircle;			// ����Ȧ�࣬�Ľ��ǳ���֮����ر���Ȧ
	BulletStruct mBullet;				// �ڵ���
	BombStruct	mBombStruct;			// ��ը��

	int mMoveSpeedDev[4] = {21, 19, 17, 15};	// �ĸ�����̹���ƶ�ʱ����
	int mBulletSpeedDev[4] = {17, 16, 15, 14 };	// ��ͬ�����ӵ�ʱ�����ٶ�
	TimeClock mTankMoveTimer;		// ̹���ƶ��ٶ�
	TimeClock mBulletTimer;		// �ӵ��ƶ��ٶ�
};

/**
	�����̹����Դ���г�ʼ��
*/
void tank_player_init(TankPlayer* tankPlayer, int playerID,
	int tankX, int tankY,
	int iconLife_x, int iconLife_y);

/**
	���̹���Ľ�����˸��ֻ���ڽ����¹ؿ���һ��ʱ�Ż�����Ľ��ǣ���Ϸ������ֻ����ֱ���Ȧ
*/
void tank_player_show_star(TankPlayer* tankPlayer);

/**
	�������̹��
*/
void tank_player_draw_tank(TankPlayer* tankPlayer);

/**
	���̹�˻��ڶ�ʱ���ƶ�
*/
void tank_player_move_by_tanktimer(TankPlayer* tankPlayer);

/**
	�ж����̹���Ƿ����ͨ��ĳһ������
	tankX��tankY��̹����һ��Ҫ�ƶ������꣬�������ͨ�������겻��
*/
bool check_tank_can_pass(int tankX, int tankY);

/**
	�����ڵ�ײ���ϰ��������ש��ֻ��ը��������ש��ը������
*/
void check_bullet_to_obstacle(TankPlayer* tankPlayer);