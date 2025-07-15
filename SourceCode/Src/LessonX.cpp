/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
#include "mylib.h"
////////////////////////////////////////////////////////////////////////////////
//
//
extern bool g_bOnGround;
float		gBornJiguangTime;
extern int  g_iMap;
int			g_iGameState		=	0;		// 游戏状态，0 -- 游戏结束等待开始状态；1 -- 按下空格键开始，初始化游戏；2 -- 游戏进行中
extern int g_iMapStage;
void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();
extern int IsLoadMap2;
extern int IsLoadMap3;
extern char jig[100];

extern int lengNumber;//冷却剂

float g_fCountdownTime = 40.0f;  // 初始
bool g_bTimerRunning = false;//计时开关
void StartTimer(float seconds) {  //计时开始函数，调用时输入时间即可开始计时
    g_fCountdownTime = seconds;
    g_bTimerRunning = true;
}


//==============================================================================
//
// 大体的程序流程为：GameMainLoop函数为主循环函数，在引擎每帧刷新屏幕图像之后，都会被调用一次。


//==============================================================================
//
// 游戏主循环，此函数将被不停的调用，引擎每刷新一次屏幕，此函数即被调用一次
// 用以处理游戏的开始、进行中、结束等各种状态. 
// 函数参数fDeltaTime : 上次调用本函数到此次调用本函数的时间间隔，单位：秒
void GameMainLoop( float	fDeltaTime )
{
	switch( g_iGameState )
	{
		// 初始化游戏，清空上一局相关数据
	case 1:
		{
			GameInit();
			g_iGameState	=	2; // 初始化之后，将游戏状态设置为进行中
		}
		break;

		// 游戏进行中，处理各种游戏逻辑
	case 2:
		{
			// TODO 修改此处游戏循环条件，完成正确游戏逻辑
			if( true )
			{
				GameRun( fDeltaTime );
			}
			else
			{
				// 游戏结束。调用游戏结算函数，并把游戏状态修改为结束状态
				g_iGameState	=	0;
				GameEnd();
			}
		}
		break;

		// 游戏结束/等待按空格键开始
	case 0:
	default:
		break;
	};
}

//==============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
void GameInit()
{
	gBornJiguangTime = 3.0f;
	if(g_iMap==1){
		InitBattery("dianchi1", -22.600, 27.500);
		InitBattery("dianchi2", 7.500, 16.100);
		InitBattery("dianchi3", -10.00, -25.100);
	}
	if(g_iMap==3)
		InitCircuitPuzzle();
	if (g_iMap == 3 && g_iMapStage == 2) {
    StartTimer(40.0f);  // 倒计时120秒
}
}
//==============================================================================
//
// 每局游戏进行中
void GameRun( float fDeltaTime )
{	// 每个激光独立的状态变量
	static float fLaserVisibleTime[4] = {0};
	static float gBornJiguangTime[4] = {3.0f, 2.5f, 2.0f, 1.5f}; // 不同冷却时间
	if(g_iMap == 1||g_iMap==2) { 
		/*================== 控制激光周期生成 ========================*/
		for(int i = 0; i < 4; i++) {
			// 安全生成激光名称
			char szName[32];
			sprintf(szName, "jig%d", i);  // 激光命名为jig0到jig3

			// 每个激光独立的状态变量
			static float fLaserVisibleTime[4] = {0};
			static float gBornJiguangTime[4] = {3.0f, 2.5f, 2.0f, 1.5f}; // 不同冷却时间

			// 冷却阶段
			if(gBornJiguangTime[i] > 0) {
				gBornJiguangTime[i] -= fDeltaTime;

				// 冷却结束且激光未显示时
				if(gBornJiguangTime[i] <= 0 && !dIsSpriteVisible(szName)) {
					PlaySound("game/data/audio/jg.wav",0,1);
					fLaserVisibleTime[i] = 1.0f; // 显示1秒
					dSetSpriteVisible(szName, 1);
					dSetSpriteCollisionReceive(szName, 1);
				}
			} 
			// 显示阶段
			else if(fLaserVisibleTime[i] > 0) {
				fLaserVisibleTime[i] -= fDeltaTime;

				// 显示时间到
				if(fLaserVisibleTime[i] <= 0) {
					dSetSpriteVisible(szName, 0);
					dSetSpriteCollisionReceive(szName, 0);
					gBornJiguangTime[i] = 3.0f + i * 0.5f; // 差异化冷却
				}
			}
		}
	}
	if (IsLoadMap2 == 1) {
		dLoadMap("intro2.t2d");   // 先加载剧情地图
		g_iMap = 2;
		g_iMapStage = 1;          // 表示正在剧情地图中
		g_iGameState = 0;         // 暂停正式流程
		IsLoadMap2 = 0;
	}
	if (IsLoadMap3 == 1) {
		dLoadMap("intro3.t2d");  // 加载剧情地图
		g_iMap = 3;
		g_iMapStage = 1;         // 剧情阶段
		g_iGameState = 0;        // 暂停正式流程
		IsLoadMap3 = 0;
	}
	/*==================================================*/
	g_bOnGround = false;

	if(g_iMap==3){
		// 倒计时更新
		if (g_bTimerRunning) {
			g_fCountdownTime -= fDeltaTime;
			// 时间到
			if (g_fCountdownTime <= 0) {
				g_bTimerRunning = false;
				PlaySound("alarm.wav", 0, 1); // 爆炸警报
			}
			dSetTextValue("timer_text", (int)(g_fCountdownTime + 1));
		}

	}
	if(g_iMap==3)
		if(CheckCircuitConnected()&&lengNumber==0){
			dLoadMap("intro4.t2d");  // 加载剧情地图
			g_iMap = 4;
			g_iMapStage = 1;         // 剧情阶段
			g_iGameState = 0;
		}

	

}
//==============================================================================
//
// 本局游戏结束
void GameEnd()
{
}
//==========================================================================
//
// 鼠标移动
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseMove( const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// 鼠标点击
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// 鼠标弹起
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// 键盘按下
// 参数 iKey：被按下的键，值见 enum KeyCodes 宏定义
// 参数 iAltPress, iShiftPress，iCtrlPress：键盘上的功能键Alt，Ctrl，Shift当前是否也处于按下状态(0未按下，1按下)
void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{	

}
//==========================================================================
//
// 键盘弹起
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
void OnKeyUp( const int iKey )
{

}
//===========================================================================
//
// 精灵与精灵碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{

}
//===========================================================================
//
// 精灵与世界边界碰撞
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{

}