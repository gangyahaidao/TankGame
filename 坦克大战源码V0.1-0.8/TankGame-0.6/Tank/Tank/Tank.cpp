#include "common.h"
#include "mci-sound.h"
#include "select-panel.h"
#include "game-control.h"

HDC main_hdc, canvas_hdc;

int main()
{
	srand((unsigned)time(NULL)); // 定义随机数种子，用来生成游戏中的一些随机值

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
		game_control_init(); 
		game_control_show_stage(); // 显示关卡信息

		switch (select_result)
		{
		case OnePlayer:
			// 添加玩家，将一个玩家对象添加到链表中

			// 加载地图，读取地图文件数据，现在简化直接创建地图数组变量map
			game_control_loop();
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