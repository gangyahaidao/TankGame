#pragma once

#include "mci-sound.h"

enum GameResult { Victory, Fail };		// ÿһ��ʤ��\ʧ��

/**
	��Ϸ������Դ��ʼ��
*/
void game_control_init();

/**
	������Ϸ�ؿ���Ԥ�Ȼ���
*/
void game_control_show_stage();

/**
	��ʼ��Ϸѭ���壺���н�����ҡ��ӵ����л�������������Ϣ�ĸ���
*/
GameResult game_control_start_game();

/**
	�����ұߵķ����ؿ�������
*/
void game_control_right_panel();

/**
	���ص�ͼ���л���
*/
void game_control_center_panel();

/**
	��ʼ��Ϸѭ����
*/
void game_control_loop();