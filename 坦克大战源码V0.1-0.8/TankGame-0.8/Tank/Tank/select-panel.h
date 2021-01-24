#pragma once

// 指示玩家选择
enum EnumSelectResult
{
	OnePlayer = 1,	// 1->1个玩家
	TwoPlayer,		// 2->2个玩家
	Custom,			// 3->自定义地图
	Error			// 4
};

/**
	玩家选择面板资源初始化
*/
void selelct_panel_init();

/**
	显示游戏玩家面板

*/
EnumSelectResult show_select_panel();