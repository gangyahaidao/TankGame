#pragma once
#include "common.h"
#include "tank-common.h"
#include "game-clock.h"

#define MAX_TANK_ENEMY 20 // ���л�����
#define TANK_ENEMY_BULLET_SPEED 30 // �л�̹���ڵ����м��ʱ��ms������ָ����ʱ����������
#define TANK_ENEMY_POS_SPEED 3 // �л��ڵ�ÿ�θ������������ֵ

enum ENE_TANK_DIR {
	ENE_DIR_LEFT,
	ENE_DIR_UP,
	ENE_DIR_RIGHT,
	ENE_DIR_DOWN
};

typedef struct tank_enemy {
	int bornPosX[3] = { BOX_SIZE, BOX_SIZE * 13, BOX_SIZE * 25 }; // �л�̹��������ֵ�����λ��X����	
	int mTankX;
	int mTankY; // ̹�����ĵ�����
	ENE_TANK_DIR dir = ENE_DIR_DOWN; // ̹�˷���

	byte tankModel; // ̹�����ͣ���Ϊ����̹�˺���̹ͨ������
	bool mDied; // �Ƿ����
	bool mBorned; // �Ƿ��Ѿ����������ڻ��Ʊ���
	bool mTankMoving; // ̹���Ƿ����ƶ�
	int mMoveStep; // ��ǰ����ʱ�ƶ������������һ����������������ϰ�����任���򣬲����¼���
		
	int mMoveSpeed[4] = { 1, 1, 1, 1 }; // ̹��ÿ�ζ�ʱ�ƶ������ص�
	int mMoveSpeedDev[4] = { 30, 15, 27, 25 };	// �ĸ�����̹���ƶ�ʱ����	
	byte mTankLevel; // ̹�˼���[0-3]
	IMAGE mTankImage[4][2]; // ��̹���������һ���ͺţ�ÿ���ͺ��ĸ�����ÿ����������ͼ��
	int mTankImageDirIndex; // ����̹������ͼƬ�±��л�

	IMAGE mScoreImage[4]; // �л���ը֮����ʾ�ķ�����100��200��300��400

	struct StarFourPoint mStar; // �����Ľ���
	BulletStruct mBullet; // �ӵ��ṹ��
	BombStruct mBombStruct; // ��ըЧ��
	BlastStruct mBlastStruct; // �����д�ըЧ��

	TimeClock mBulletTimer; // �����ӵ��ƶ��ٶ�
	TimeClock mTankMoveTimer; // ̹���ƶ��ٶ�
	TimeClock mShootTimer; // ̹�˷����ӵ���Ƶ��

}TankEnemy;

/**
	����һ���л�̹��
*/
void tank_enemy_add();

/**
	��ʾ�л�̹���Ľ���
*/
void tank_enemy_show_star();

/**
	���Ƶл�̹�˺��ڵ�
*/
void tank_enemy_draw_tank();

/**
	�л�̹�����µ�������
*/
void tank_enemy_rejust_direction(TankEnemy* pTankEnemy);

/**
	�жϵл�̹���Ƿ����ͨ��ĳһ������
	tankX��tankY��̹����һ��Ҫ�ƶ������꣬�������ͨ�������겻��
*/
bool check_tank_enemy_can_pass(int dir, int tankX, int tankY);

/**
	�����ڵ�ײ���ϰ��������ש��ֻ��ը��������ש��ը������
*/
void check_enemy_bullet_to_obstacle(TankEnemy* pTankEnemy);