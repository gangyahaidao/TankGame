#pragma once

#include <stdbool.h>

#define S_START		0
#define S_SHOOT0	100
#define S_SHOOT1	101
#define S_SHOOT2	102
#define S_SHOOT3	103
#define S_BIN		2
#define S_BK		3
#define S_GETPROP	4 // ��ȡ����
#define S_PROPOUT	5 // ʧȥ����
#define	S_ENEMY_BOMB 6 // �л���ը
#define S_CAMP_BOMB	7 // ���ر�ը
#define S_SCOREPANEL_DI	8 // �������ε���
#define S_BOUNS1000		9 // �����ﵽ1000������
#define S_ADDLIFE	10 // �Ե�������������
#define S_WIN		11 // ʤ��
#define S_FAIL		12 // ʧ��
#define S_PLAYER_BOMB	13 // ���ѱ�ը
#define S_PLAYER_MOVE	14 // ���̹���ƶ�
#define S_PLAYER_STOP_MOVE 15 // ���̹��ֹͣ�ƶ�

/**
	��ʼ����Ч��Դ
*/
void InitSounds(); 

/**
	�ͷ���Ч��Դ
*/
void CloseSounds();

/**
	����ָ����ŵ���Ч
*/
void PlaySounds(int kind);