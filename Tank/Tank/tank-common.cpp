#include "tank-common.h"

/**
	四角星图片资源初始化
*/
void star_load_resource(StarFourPoint* starFourPoint) {
	TCHAR buf[100];
	// 出生四角星闪烁
	for (int i = 0; i < 4; i++)
	{
		_stprintf_s(buf, _T("./res/big/star%d.gif"), i);
		loadimage(&starFourPoint->mStarImage[i], buf);
	}
}

/**
	结构体控制属性初始化
*/
void star_init(StarFourPoint* starFourPoint) {
	starFourPoint->mStarIndex = 3;
	starFourPoint->mStarCounter = 0;
	starFourPoint->starState = Star_Out; // 四角星刚出现
}