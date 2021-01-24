#include "select-panel.h"
#include "common.h"

// 声明全局变量
extern HDC main_hdc, canvas_hdc;

// 上升动画,选择单人\双人\自定义\游戏.
IMAGE mSelect_player_image;
int mSelect_player_image_y = WINDOW_HEIGHT;		// 图片从底部上升控制,初始值=448, 窗口底部

// 开始选玩家1，2，地图
IMAGE mSelectTankImage[2];
POINT mSelectTankPoint[3];	// 三个选项坐标
int mSelectIndex;			// 选择结果: 0,1,2
byte mCounter;				// 用来切换 mSelectTankImage 下标

// 选择玩家后显示关卡信息
IMAGE mGrayBackgroundImage;	// 游戏灰色背景图
IMAGE mBlackNumberImage;	// 数字 1234567890 图片

/**
	玩家选择面板资源初始化
*/
void selelct_panel_init() {
	// 加载界面图像资源
	loadimage(&mSelect_player_image, _T("./res/big/select_player.gif")); // 背景图

	// 选择坦克手柄游标
	loadimage(&mSelectTankImage[0], _T("./res/big/0Player/m0-2-1.gif"));
	loadimage(&mSelectTankImage[1], _T("./res/big/0Player/m0-2-2.gif"));

	// 灰色背景
	loadimage(&mGrayBackgroundImage, _T("./res/big/bg_gray.gif"));
	loadimage(&mBlackNumberImage, _T("./res/big/black-number.gif"));

	// 指定三种模式选择的游标位置
	mSelectTankPoint[0].x = 60;
	mSelectTankPoint[1].x = 60;
	mSelectTankPoint[2].x = 60;
	mSelectTankPoint[0].y = 123;
	mSelectTankPoint[1].y = 139;
	mSelectTankPoint[2].y = 156;

	// 选择结果初始化
	mSelectIndex = 0;
	mCounter = 0; // 用来切换选择游标的下标
}

/**
	显示游戏玩家面板

*/
EnumSelectResult show_select_panel() {
	cleardevice(); // 清空之前显示的内容，避免干扰

	// 显示玩家选择面板上升动画
	while (mSelect_player_image_y > 0) {
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) { // 如果上升过程中按了回车键，直接显示
			mSelect_player_image_y = 0;
		}

		Sleep(55); // 每一次循环间隔休眠55ms
		mSelect_player_image_y -= 5;
		if (mSelect_player_image_y < 0) {
			mSelect_player_image_y = 0;
		}

		// 调用window api将玩家选择图像绘制在画布上
		BitBlt(canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, GetImageHDC(&mSelect_player_image), 0, 0, SRCCOPY);

		// 将canvas_hdc绘制到主窗口main_hdc
		StretchBlt(main_hdc, 0, mSelect_player_image_y, WINDOW_WIDTH, WINDOW_HEIGHT, canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, SRCCOPY);

		// 刷新输出缓存的绘图
		FlushBatchDraw();
	}

	int temp = 0;
	// 玩家使用上下键、回车键选择游戏模式
	while (true) {
		Sleep(40);		

		// 在画布上重新绘制玩家游戏模式，避免之前透明贴图的影响
		BitBlt(canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, GetImageHDC(&mSelect_player_image), 0, 0, SRCCOPY);

		// 将选择模式的游标进行透明贴图
		mCounter = mCounter == 0 ? 1 : 0; // 切换现实的游标坦克图片，坦克动态效果
		TransparentBlt(canvas_hdc, mSelectTankPoint[mSelectIndex].x, mSelectTankPoint[mSelectIndex].y, 16, 16,
			GetImageHDC(&mSelectTankImage[mCounter]), 0, 0, 16, 16, 0x000000);

		// 将canvas_hdc绘制到主窗口main_hdc
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