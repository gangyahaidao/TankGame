#pragma once

// ָʾ���ѡ��
enum EnumSelectResult
{
	OnePlayer = 1,	// 1->1�����
	TwoPlayer,		// 2->2�����
	Custom,			// 3->�Զ����ͼ
	Error			// 4
};

/**
	���ѡ�������Դ��ʼ��
*/
void selelct_panel_init();

/**
	��ʾ��Ϸ������

*/
EnumSelectResult show_select_panel();