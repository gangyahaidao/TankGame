#pragma once
// ��Ҫ���������һЩ���̹����з�̹�˹�ͬʹ�õ�һЩ���ܣ��磺̹��λ�ú��ӱ�ǡ��ӵ��ṹ���ӵ���ը��̹�˱�ը�����/�л��Ľ�����˸���������

#include "common.h"

// �Ľ�����˸���л�������ҵ��Ľ�����˸��������
enum Star_State {
	Star_Begin,			// �Ľ��Ǹոճ���
	Star_End			// ̹���Ѿ�����
};
typedef struct {
	IMAGE mStarImage[4]; // �����Ľ���ͼƬ
	int mStarIndex; // �����±�
	int mStarIndexDir; // �����±�仯����1/-1��1����С���-1���ɴ��С
	int mStarCounter; // �������೤��ѭ�����һ��ͼ��
	Star_State starState;				// �Ľ�����ʧ��ʱ��̹�˳���, ֹͣ�����Ľ�����˸ͼ
}StarFourPoint;

typedef struct bullet_struct{
	int posX, posY; // �ڵ���ǰ����
	int dir; // �ڵ�����
	int speed[4] = {2, 3, 3, 4}; // ̹�˲�ͬ������ò�ͬ���ӵ��ٶȣ�����ÿ���ƶ������4�����ص㣬��Ȼ���л���BUG
	int mKillID; // ��¼���е����/�л�ID
	IMAGE mImage[4]; // �ڵ�ͼƬ��Դ
	int bulletSize[4][2]; // ���ң�4*3   ���£�3*4   
	bool needDraw; // �Ƿ���Ҫ��ָ����������ڵ�
}BulletStruct;