#pragma once
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <tchar.h>

#include <easyx.h>			// ����ͼ�ο�ͷ�ļ�

#include <windows.h>
#include <Mmsystem.h>
#pragma comment ( lib, "Winmm.lib" )

// GDI
#include <wingdi.h>
#pragma comment (lib, "msimg32.lib")

#define WINDOW_WIDTH 512 // ���崰�ڴ�С
#define WINDOW_HEIGHT 448
#define CANVAS_WIDTH	256		// ���� image ��С������ı䣬���һ�ͼ���� image �ϲ�����Ȼ��һ���Ի��Ƶ�������
#define CANVAS_HEIGHT	224
#define CENTER_WIDTH	208		// �м��ɫ��Ϸ����
#define CENTER_HEIGHT	208
#define BLACK_NUMBER_SIZE 7		// �ؿ���ɫ���ִ�С
#define ENEMY_TANK_ICO_SIZE 7	// ����ұߵл�ͼ���С
#define FLAG_ICO_SIZE_X 16		// ����ͼ��Ŀ�
#define FLAG_ICO_SIZE_Y 15		// ���ӵĸ�
#define CENTER_X		16		// ��ɫ��Ϸ����������Ͻǵ�����
#define CENTER_Y		9
#define PLAYER_12_ICO_SIZE_X		14		// 1P/2P�Ҳ����ͼ���С
#define PLAYER_12_ICO_SIZE_Y		7
#define PLAYER_TANK_ICO_SIZE_X		7		// ���ͼ���С
#define PLAYER_TANK_ICO_SIZE_Y		8

#define TANK_WIDTH	16 // ���̹�˿��
#define TANK_HEIGHT 16 // ���̹�˸߶�

#define BOX_SIZE		8		// �м仭�������ֳ�26*26�ĸ��ӣ�ÿ��������8����
#define _CLEAR		-1		// ����ϰ��ﱻ�����ĸ���, ����ɨ��ʱ�� 0 ���ֿ�, Ȼ����ƺ�ɫͼƬ, ����� 0 Ҳ���ж������
#define _EMPTY		0		// �յ�
#define _FOREST		1		// ɭ��
#define _ICE		2		// ��
// ------ ���� < 3 ̹�˿��Դ���
#define _WALL		3		// ǽ
#define _RIVER		4		// ����
#define _STONE		5		// ʯͷ