#include "common.h"
#include "mci-sound.h"
#include "select-panel.h"

HDC main_hdc, canvas_hdc;

int main()
{
	srand((unsigned)time(NULL)); // 定义随机数种子

	// 初始化音效资源
	InitSounds();
	
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, EW_SHOWCONSOLE); // 显示控制台窗口
	BeginBatchDraw(); // 开启批量绘图，直到执行 FlushBatchDraw 或 EndBatchDraw 才将之前的绘图输出

	// 定义IMAGE画布
	IMAGE canvas_img(CANVAS_WIDTH, CANVAS_HEIGHT);

	// 获取graphics绘图设备句柄，可以用在window GDI函数中
	main_hdc = GetImageHDC(); // 获取默认绘图窗口的HDC句柄
	canvas_hdc = GetImageHDC(&canvas_img); // 获取img对象的HDC句柄

	// 显示玩家模式选择面板
	selelct_panel_init(); // 玩家选择面板资源初始化	

	while (_kbhit() != 27) {
		EnumSelectResult select_result = show_select_panel(); // 游戏结束之后继续进行模式选择

		switch (select_result)
		{
		case OnePlayer:
			// 添加玩家

			// 加载地图

			// 进入地图

			break;
		case TwoPlayer:
			break;
		case Custom:
			break;
		case Error:
			break;
		default:
			break;
		}
	}

}