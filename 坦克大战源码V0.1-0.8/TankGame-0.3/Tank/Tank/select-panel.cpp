#include "select-panel.h"
#include "common.h"

// ����ȫ�ֱ���
extern HDC main_hdc, canvas_hdc;

// ��������,ѡ����\˫��\�Զ���\��Ϸ.
IMAGE mSelect_player_image;
int mSelect_player_image_y = WINDOW_HEIGHT;		// ͼƬ�ӵײ���������,��ʼֵ=448, ���ڵײ�

// ��ʼѡ���1��2����ͼ
IMAGE mSelectTankImage[2];
POINT mSelectTankPoint[3];	// ����ѡ������
int mSelectIndex;			// ѡ����: 0,1,2
byte mCounter;				// �����л� mSelectTankImage �±�

// ѡ����Һ���ʾ�ؿ���Ϣ
IMAGE mGrayBackgroundImage;	// ��Ϸ��ɫ����ͼ
IMAGE mBlackNumberImage;	// ���� 1234567890 ͼƬ

/**
	���ѡ�������Դ��ʼ��
*/
void selelct_panel_init() {
	// ���ؽ���ͼ����Դ
	loadimage(&mSelect_player_image, _T("./res/big/select_player.gif")); // ����ͼ

	// ѡ��̹���ֱ��α�
	loadimage(&mSelectTankImage[0], _T("./res/big/0Player/m0-2-1.gif"));
	loadimage(&mSelectTankImage[1], _T("./res/big/0Player/m0-2-2.gif"));

	// ��ɫ����
	loadimage(&mGrayBackgroundImage, _T("./res/big/bg_gray.gif"));
	loadimage(&mBlackNumberImage, _T("./res/big/black-number.gif"));

	// ָ������ģʽѡ����α�λ��
	mSelectTankPoint[0].x = 60;
	mSelectTankPoint[1].x = 60;
	mSelectTankPoint[2].x = 60;
	mSelectTankPoint[0].y = 123;
	mSelectTankPoint[1].y = 139;
	mSelectTankPoint[2].y = 156;

	// ѡ������ʼ��
	mSelectIndex = 0;
	mCounter = 0; // �����л�ѡ���α���±�
}

/**
	��ʾ��Ϸ������

*/
EnumSelectResult show_select_panel() {
	cleardevice(); // ���֮ǰ��ʾ�����ݣ��������

	// ��ʾ���ѡ�������������
	while (mSelect_player_image_y > 0) {
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) { // ������������а��˻س�����ֱ����ʾ
			mSelect_player_image_y = 0;
		}

		Sleep(55); // ÿһ��ѭ���������55ms
		mSelect_player_image_y -= 5;
		if (mSelect_player_image_y < 0) {
			mSelect_player_image_y = 0;
		}

		// ����window api�����ѡ��ͼ������ڻ�����
		BitBlt(canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, GetImageHDC(&mSelect_player_image), 0, 0, SRCCOPY);

		// ��canvas_hdc���Ƶ�������main_hdc
		StretchBlt(main_hdc, 0, mSelect_player_image_y, WINDOW_WIDTH, WINDOW_HEIGHT, canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, SRCCOPY);

		// ˢ���������Ļ�ͼ
		FlushBatchDraw();
	}

	int temp = 0;
	// ���ʹ�����¼����س���ѡ����Ϸģʽ
	while (true) {
		Sleep(40);		

		// �ڻ��������»��������Ϸģʽ������֮ǰ͸����ͼ��Ӱ��
		BitBlt(canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, GetImageHDC(&mSelect_player_image), 0, 0, SRCCOPY);

		// ��ѡ��ģʽ���α����͸����ͼ
		mCounter = mCounter == 0 ? 1 : 0; // �л���ʵ���α�̹��ͼƬ��̹�˶�̬Ч��
		TransparentBlt(canvas_hdc, mSelectTankPoint[mSelectIndex].x, mSelectTankPoint[mSelectIndex].y, 16, 16,
			GetImageHDC(&mSelectTankImage[mCounter]), 0, 0, 16, 16, 0x000000);

		// ��canvas_hdc���Ƶ�������main_hdc
		StretchBlt(main_hdc, 0, mSelect_player_image_y, WINDOW_WIDTH, WINDOW_HEIGHT, canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, SRCCOPY);
		FlushBatchDraw();

		temp++;
		if (GetAsyncKeyState(VK_UP) & 0x8000 && temp > 3) {
			temp = 0;
			mSelectIndex -= 1;
			if (mSelectIndex < 0) {
				mSelectIndex = 2;
			}
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && temp > 3) {
			temp = 0;
			mSelectIndex += 1;
			if (mSelectIndex > 2) {
				mSelectIndex = 0;
			}
		}
		else if (GetAsyncKeyState(VK_RETURN) & 0x8000 && temp > 3) {
			temp = 3;
			break;
		}
	}

	if (mSelectIndex == 0) {
		return OnePlayer;
	}
	else if (mSelectIndex == 1) {
		return TwoPlayer;
	}
	else if (mSelectIndex == 2) {
		return Custom;
	}
	else {
		return Error;
	}
}