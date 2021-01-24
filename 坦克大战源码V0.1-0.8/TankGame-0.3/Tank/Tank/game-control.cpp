#include "common.h"
#include "game-control.h"
#include "game-clock.h"
#include "tank-player.h"

extern HDC main_hdc, canvas_hdc;
extern IMAGE mGrayBackgroundImage;		// 游戏灰色背景图
extern IMAGE mBlackNumberImage;		// 0123456789 当前关卡数

extern TimeClock mainTimer; // 用于主窗口程序的界面定时绘制

IMAGE center_img; // 用于保持center_hdc存在的有效性
HDC center_hdc; // 中间游戏区域，分开绘制方便进行更新

IMAGE mBlackBackgroundImage;	// 黑色背景图

IMAGE mStoneImage;				// 石头
IMAGE mForestImage;				// 树林
IMAGE mIceImage;				// 冰块
IMAGE mRiverImage[2];			// 河流
IMAGE mWallImage;				// 泥墙
IMAGE mCamp[2];					// 大本营

IMAGE mEnemyTankIcoImage;		// 敌机坦克图标
IMAGE mFlagImage;				// 旗子
IMAGE m12PImage;
IMAGE mPlayerTankIcoImage;

int mCutStageCounter;
IMAGE mCurrentStageImage;		// STAGE 字样
IMAGE mGameOverImage;			
int mRemainEnemyTankNumber = 20;		// 余下未显示的敌机数量, 初始值=20, 出现一架敌机该值减一

int mPlayerLife = 3; // 玩家默认生命值为3
int mCurrentStage = 1; // 当前关卡

TankPlayer tankPlayer0;

const char* map[] = { // 第一关地图数据，0空地、1深林、2冰、3墙、4河流、5石头
		"00000000000000000000000000",
		"00000000000000000000000000",
		"00330033003300330033003300",
		"00330033003300330033003300",
		"00330033003300330033003300",
		"00330033003300330033003300",
		"00330033003355330033003300",
		"00330033003355330033003300",
		"00330033003300330033003300",
		"00330033000000000033003300",
		"00330033000000000033003300",
		"00000000003300330000000000",
		"00000000003300330000000000",
		"33003333000000000033330033",
		"55003333000000000033330055",
		"00000000003300330000000000",
		"00000000003333330000000000",
		"00330033003333330033003300",
		"00330033003300330033003300",
		"00330033003300330033003300",
		"00330033003300330033003300",
		"00330033000000000033003300",
		"00330033000000000033003300",
		"00330033000333300033003300",
		"00000000000300300000000000",
		"00000000000300300000000000" };

/**
	游戏界面资源初始化
*/
void game_control_init() {
	center_img.Resize(CENTER_WIDTH, CENTER_HEIGHT);
	center_hdc = GetImageHDC(&center_img);

	loadimage(&mBlackBackgroundImage, _T("./res/big/bg_black.gif"));	// 黑色背景
	loadimage(&mStoneImage, _T("./res/big/stone.gif"));					// 12*12的石头
	loadimage(&mForestImage, _T("./res/big/forest.gif"));				// 树林
	loadimage(&mIceImage, _T("./res/big/ice.gif"));						// 冰块
	loadimage(&mRiverImage[0], _T("./res/big/river-0.gif"));			// 河流
	loadimage(&mRiverImage[1], _T("./res/big/river-1.gif"));
	loadimage(&mWallImage, _T("./res/big/wall.gif"));					// 泥墙
	loadimage(&mCamp[0], _T("./res/big/camp0.gif"));					// 大本营
	loadimage(&mCamp[1], _T("./res/big/camp1.gif"));	 
	loadimage(&mEnemyTankIcoImage, _T("./res/big/enemytank-ico.gif"));	// 敌机图标
	loadimage(&mFlagImage, _T("./res/big/flag.gif"));					// 旗子
	loadimage(&mCurrentStageImage, _T("./res/big/stage.gif"));
	loadimage(&mGameOverImage, _T("./res/big/gameover.gif"));

	// 玩家图标
	loadimage(&m12PImage, _T("./res/big/1P.gif"));		// 1P\2P图标
	loadimage(&mPlayerTankIcoImage, _T("./res/big/playertank-ico.gif"));	// 玩家坦克图标
}

/**
	进入游戏关卡的预热画面
*/
void game_control_show_stage() {
	// 播放开始音效
	PlaySounds(S_START); 

	// 上下合拢特效
	StretchBlt(canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, 
		GetImageHDC(&mGrayBackgroundImage), 0, 0, 66, 66, SRCCOPY); // 灰色背景贴图
	
	int mCutStageCounter = 0;
	while (mCutStageCounter < 110) {
		Sleep(6);
		mCutStageCounter += 3;
		StretchBlt(canvas_hdc, 0, 0, CANVAS_WIDTH, mCutStageCounter,
			GetImageHDC(&mBlackBackgroundImage), 0, 0, CENTER_WIDTH, CENTER_HEIGHT, SRCCOPY); // 上半部分
		StretchBlt(canvas_hdc, 0, CANVAS_HEIGHT-mCutStageCounter, CANVAS_WIDTH, CANVAS_HEIGHT,
			GetImageHDC(&mBlackBackgroundImage), 0, 0, CENTER_WIDTH, CENTER_HEIGHT, SRCCOPY); // 下半部分
		StretchBlt(main_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, SRCCOPY); // 将canvas贴到main窗口上
		FlushBatchDraw();
	}

	// 显示关卡页面 Stage 1/2/3/4....
	StretchBlt(canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT,
		GetImageHDC(&mGrayBackgroundImage), 0, 0, 66, 66, SRCCOPY); // 灰色背景贴图
	TransparentBlt(canvas_hdc, 97, 103, 39, 7, GetImageHDC(&mCurrentStageImage), 0, 0, 39, 7, 0xffffff); // 加载Stage字样图片，最后一个参数指定透明色

	if (mCurrentStage < 10) { // 关卡<10
		TransparentBlt(canvas_hdc, 157, 103, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
			GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * mCurrentStage, 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);
	}
	else {
		TransparentBlt(canvas_hdc, 157, 103, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
			GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * (mCurrentStage / 10), 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);

		TransparentBlt(canvas_hdc, 157, 103, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
			GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * (mCurrentStage % 10), 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);
	}
	StretchBlt(main_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, SRCCOPY); // 将canvas贴到main窗口上
	FlushBatchDraw();

	Sleep(1200);

	PlaySounds(S_BK);
}

/**
	加载地图进行绘制
*/
void game_control_center_panel() {
	BitBlt(center_hdc, 0, 0, CENTER_WIDTH, CENTER_HEIGHT, GetImageHDC(&mBlackBackgroundImage), 0, 0, SRCCOPY);// 中心黑色背景游戏区

	// 玩家四角星闪烁控制
	tank_player_show_star(&tankPlayer0); // 四角星闪烁完成之后状态设置为Star_End

	// 开始绘制地图
	int i = 0, j = 0;
	int x = 0, y = 0;
	for (i = 0; i < 26; i++) {
		for (j = 0; j < 26; j++) {
			x = j * BOX_SIZE;
			y = i * BOX_SIZE;
			switch (map[i][j]-'0') {
			case _WALL:
				BitBlt(center_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mWallImage), 0, 0, SRCCOPY);
				break;
			case _ICE:
				BitBlt(center_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mIceImage), 0, 0, SRCCOPY);
				break;
			case _RIVER:
				BitBlt(center_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mRiverImage[0]), 0, 0, SRCCOPY);
				break;
			case _STONE:
				BitBlt(center_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mStoneImage), 0, 0, SRCCOPY);
				break;
			default:
				break;
			}

			// 绘制森林
			if ((map[i][j] - '0') == _FOREST) {
				TransparentBlt(center_hdc, x, y, BOX_SIZE, BOX_SIZE, GetImageHDC(&mForestImage), 0, 0, BOX_SIZE, BOX_SIZE, 0x000000);
			}
		}
	}
}

/**
	更新右边的分数关卡等数据
*/
void game_control_right_panel() {
	// 设置灰色背景
	StretchBlt(canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, GetImageHDC(&mGrayBackgroundImage), 0, 0, 66, 66, SRCCOPY);

	// 显示剩余敌机的图标
	for (int i = 0; i < mRemainEnemyTankNumber; i++) {
		int x = i % 2 == 0 ? 233 : 241; // 计算敌机图标x坐标
		TransparentBlt(canvas_hdc, x, 19 + i / 2 * 8, ENEMY_TANK_ICO_SIZE, ENEMY_TANK_ICO_SIZE,
			GetImageHDC(&mEnemyTankIcoImage), 0, 0, ENEMY_TANK_ICO_SIZE, ENEMY_TANK_ICO_SIZE, 0xffffff);
	}

	// 显示玩家生命信息
	TransparentBlt(canvas_hdc, 233, 129, PLAYER_12_ICO_SIZE_X, PLAYER_12_ICO_SIZE_Y,
		GetImageHDC(&m12PImage), 0, 0, PLAYER_12_ICO_SIZE_X, PLAYER_12_ICO_SIZE_Y, 0xffffff); // 绘制1P/2P图标
	TransparentBlt(canvas_hdc, 233, 137, PLAYER_TANK_ICO_SIZE_X, PLAYER_TANK_ICO_SIZE_Y,
		GetImageHDC(&mPlayerTankIcoImage), 0, 0, PLAYER_TANK_ICO_SIZE_X, PLAYER_TANK_ICO_SIZE_Y, 0x000000); // 绘制坦克图标
	TransparentBlt(canvas_hdc, 240, 137, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
		GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * mPlayerLife, 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff); // 绘制玩家生命值

	// 限制关卡旗子和关卡数
	TransparentBlt(canvas_hdc, 232, 177, FLAG_ICO_SIZE_X, FLAG_ICO_SIZE_Y,
		GetImageHDC(&mFlagImage), 0, 0, FLAG_ICO_SIZE_X, FLAG_ICO_SIZE_Y, 0xffffff);

	if (mCurrentStage < 10) { // 关卡<10
		TransparentBlt(canvas_hdc, 238, 193, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
			GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * mCurrentStage, 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);
	}
	else {
		TransparentBlt(canvas_hdc, 233, 193, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
			GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * (mCurrentStage / 10), 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);

		TransparentBlt(canvas_hdc, 241, 193, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE,
			GetImageHDC(&mBlackNumberImage), BLACK_NUMBER_SIZE * (mCurrentStage % 10), 0, BLACK_NUMBER_SIZE, BLACK_NUMBER_SIZE, 0xffffff);
	}
}

/**
	开始游戏循环体
*/
void game_control_loop() {
	GameResult result = Victory;

	// 初始化玩家0的结构体
	tank_player_init(&tankPlayer0, 0, 4*16+BOX_SIZE, 12*16+BOX_SIZE, 240, 137);

	// 定时器初始化
	clock_init(&mainTimer, 15); // 主窗口15ms刷新一次

	while (result != Fail) {
		result = game_control_start_game();
		Sleep(1);
	}
}

/**
	进行界面玩家、子弹、敌机、其他所有信息的更新
*/
GameResult game_control_start_game() {
	if (clock_is_timeout(&mainTimer)) {

		// 胜利或者失败，显示分数面板

		// 如果失败，上升GAME OVER字样

		// 添加敌机坦克

		// 绘制中间游戏主体区域
		game_control_center_panel();

		// 绘制右边信息栏
		game_control_right_panel();

		// 将中间绘图区center_hdc绘制到画布canvas_hdc上
		BitBlt(canvas_hdc, CENTER_X, CENTER_Y, CENTER_WIDTH, CENTER_HEIGHT, center_hdc, 0, 0, SRCCOPY);
		// 将整个canvas_hdc缩放显示到main主窗口上
		StretchBlt(main_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			canvas_hdc, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, SRCCOPY);
		FlushBatchDraw();
	}

	return Victory;
}