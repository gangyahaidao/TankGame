#pragma once
// ��Ҫ���������һЩ���̹����з�̹�˹�ͬʹ�õ�һЩ���ܣ��磺̹��λ�ú��ӱ�ǡ��ӵ��ṹ���ӵ���ը��̹�˱�ը�����/�л��Ľ�����˸���������

#include "common.h"

// �Ľ�����˸���л�������ҵ��Ľ�����˸��������
enum Star_State {
	Star_Begin,			// �Ľ��Ǹոճ���
	Star_Showing,		// ������˸�Ľ���
	Star_End			// ̹���Ѿ�����
};


typedef struct {
	IMAGE mStarImage[4]; // �����Ľ���ͼƬ
	int mStarIndex; // �����±�
	int mStarIndexDir; // �����±�仯����1/-1��1����С���-1���ɴ��С
	int mStarCounter; // �������೤��ѭ�����һ��ͼ��
	Star_State starState;				// �Ľ�����ʧ��ʱ��̹�˳���, ֹͣ�����Ľ�����˸ͼ
}StarFourPoint;