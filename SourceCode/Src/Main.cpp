//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include "CommonAPI.h"
#include "LessonX.h"
#include "mylib.h"
#include "math.h"
#include "stdio.h"
int g_iMap = 0; //地图状态函数，0->开始界面地图
extern int g_iGameState;
int g_iMapStage = 0;// 0: 初始界面，1: 剧情地图中，2: 正式地图中

void UpdateJumpInput();
void UpdateMoveInput(int iKey);
void UpdatePlayerRotationByGravity();


//yhy行走参数
bool g_bOnGround = 0; // 是否站在平台上
static float s_fPlayerSpeedX = 0;        // 水平移动速度
static bool g_bIsJumping = 0;       // 跳跃状态标志
static const float Gravity = 55.0f;      // 重力加速度
static float Force;					//支持力
static float s_fPlayerSpeedY = 0;   // 垂直方向速度
static const float g_fJumpForce = 20.0f;  // 跳跃初速度
int s_iPlayerDirection = 1;				//玩家朝向的状态，1->朝右，0—>朝左

int lengNumber=4;
int BatteryNumber = 0;
int IsLoadMap2;//是否进入下一关的状态函数
int IsLoadMap3;
int GravityDirection = 0; //0->DOWN,1->UP,2->RIGHT,3->LEFT重力方向状态变量


float		g_fScreenLeft		=	0.f;    // 屏幕左边界值
float		g_fScreenRight		=	0.f;    // 右
float		g_fScreenTop		=	0.f;    // 上
float		g_fScreenBottom	=	0.f;


char jig[100];//精灵名称数组

//重力方向切换函数
void mSwitchGravity(int x, float Gravity, float Force, float *fX, float *fY, float fTimeDelta) {
	switch (x) {
	case 0 :
		s_fPlayerSpeedY += (Gravity + Force) * fTimeDelta;//下
		(*fY) += s_fPlayerSpeedY * fTimeDelta;
		break;
	case 1 :
		s_fPlayerSpeedY += -(Gravity + Force) * fTimeDelta;//上
		(*fY) += s_fPlayerSpeedY * fTimeDelta;
		break;
	case 2 :
		s_fPlayerSpeedX += (Gravity + Force) * fTimeDelta; //右
		(*fX) += s_fPlayerSpeedX * fTimeDelta;
		break;
	case 3 :
		s_fPlayerSpeedX += -(Gravity + Force) * fTimeDelta; //左
		(*fX) += s_fPlayerSpeedX * fTimeDelta;
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
//
// 主函数入口
//
//////////////////////////////////////////////////////////////////////////////////////////
int PASCAL WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow) {
		// 初始化游戏引擎
		if ( !dInitGameEngine( hInstance, lpCmdLine ) )
			return 0;

		// To do : 在此使用API更改窗口标题
		dSetWindowTitle("Lesson");
		//世界边界
		g_fScreenLeft	 	= 	dGetScreenLeft();
		g_fScreenRight  	= 	dGetScreenRight();
		g_fScreenTop 	 	= 	dGetScreenTop();
		g_fScreenBottom 	= 	dGetScreenBottom();

		dSetSpriteWorldLimit("yhy", WORLD_LIMIT_NULL, g_fScreenLeft - 23,
			g_fScreenTop, g_fScreenRight + 24, g_fScreenBottom);
		dSetSpriteWorldLimit("yhyrun", WORLD_LIMIT_NULL, g_fScreenLeft,
			g_fScreenTop, g_fScreenRight, g_fScreenBottom);

		for (int i = 0; i < 4; i++) {
			sprintf(&jig[i], "jig%d", i); //创建精灵名称数组
		}

		// 引擎主循环，处理屏幕图像刷新等工作
		while ( dEngineMainLoop() ) {
			// 获取两次调用之间的时间差，传递给游戏逻辑处理
			float fTimeDelta =	dGetTimeDelta();
			// 获取当前精灵位置
			float fX, fY;
			fX = dGetSpritePositionX("yhy");
			fY = dGetSpritePositionY("yhy");

			if (g_iMap == 1) {
				// 垂直运动（只有不在地面/平台上时才受重力影响）
				if (g_bOnGround == 1) Force = -Gravity;
				else Force = 0;
				s_fPlayerSpeedY += (Gravity + Force) * fTimeDelta;
				fY += s_fPlayerSpeedY * fTimeDelta;
			}

			if (g_iMap == 2) {
				if (g_bOnGround == 1) Force = -Gravity;
				else Force = 0;
				mSwitchGravity (GravityDirection, Gravity, Force, &fX, &fY, fTimeDelta);
			}
			if (g_iMap == 3) {
				// 垂直运动（只有不在地面/平台上时才受重力影响）
				if (g_bOnGround == 1) Force = -Gravity;
				else Force = 0;
				s_fPlayerSpeedY += (Gravity + Force) * fTimeDelta;
				// 限制最大速度，避免震荡
				if (s_fPlayerSpeedY > 25) s_fPlayerSpeedY = 25;
				fY += s_fPlayerSpeedY * fTimeDelta;
			}

			// 更新精灵位置
			dSetSpritePosition("yhy", fX, fY);
			// 执行游戏主循环
			GameMainLoop( fTimeDelta );
		};

		// 关闭游戏引擎
		dShutdownGameEngine();
		return 0;
}

//==========================================================================
//
// 引擎捕捉鼠标移动消息后，将调用到本函数
// 参数 fMouseX, fMouseY：为鼠标当前坐标
//
void dOnMouseMove( const float fMouseX, const float fMouseY ) {
	// 可以在此添加游戏需要的响应函数
	OnMouseMove(fMouseX, fMouseY );
}
//==========================================================================
//
// 引擎捕捉鼠标点击消息后，将调用到本函数
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
//
void dOnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY ) {
	// 可以在此添加游戏需要的响应函数
	OnMouseClick(iMouseType, fMouseX, fMouseY);
	if (g_iMap == 0){
		if (g_iMapStage == 0 && iMouseType == 0 && mOnMouseinSprite(fMouseX, fMouseY, "start")) {
			dLoadMap("intro1.t2d");  // 加载剧情地图
			g_iMapStage = 1;         // 正在剧情地图中
		}
		if (g_iMapStage == 1 && iMouseType == 1) {
			dLoadMap("guan1.t2d");
			g_iMap = 1;
			g_iGameState = 1;
			g_iMapStage = 2;
		}
	}
	// 如果当前是第二关的剧情地图阶段，点击后进入正式地图
	if (g_iMap == 2 && g_iMapStage == 1 && iMouseType == 0) {
		dLoadMap("guan2.t2d");
		g_iMapStage = 2;       // 进入正式地图
		g_iGameState = 1;      // 开始初始化
		return;
	}
	if (g_iMap == 3 && g_iMapStage == 1 && iMouseType == 0) {
		dLoadMap("guan3.t2d");    // 加载正式地图
		g_iMapStage = 2;
		g_iGameState = 1;         // 启动 GameInit()
		return;
	}
	if (g_iMap == 3&& iMouseType == 0) {
		HandleCircuitClick(fMouseX, fMouseY);
	}
}
//==========================================================================
//
// 引擎捕捉鼠标弹起消息后，将调用到本函数
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
//
void dOnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY ) {
	// 可以在此添加游戏需要的响应函数
	OnMouseUp(iMouseType, fMouseX, fMouseY);

}
//==========================================================================
//
// 引擎捕捉键盘按下消息后，将调用到本函数
// 参数 iKey：被按下的键，值见 enum KeyCodes 宏定义
// 参数 iAltPress, iShiftPress，iCtrlPress：键盘上的功能键Alt，Ctrl，Shift当前是否也处于按下状态(0未按下，1按下)
//
void dOnKeyDown( const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress ) {
	// 可以在此添加游戏需要的响应函数
	OnKeyDown(iKey, iAltPress, iShiftPress, iCtrlPress);


	//切换为跑动
	if (KEY_A == iKey || KEY_D == iKey) {
		dSetSpriteVisible("yhy", 0);
		dSetSpriteVisible("yhyrun", 1);
	}

	if (g_iMap == 1) {
		/*===============guan1==============*/
		switch (iKey) {
		case KEY_SPACE:
			if (g_bIsJumping == 0 ) {
				s_fPlayerSpeedY = -g_fJumpForce;  // 负值表示向上
				g_bIsJumping = 1;
				g_bOnGround = 0;
			}
			break;
		case KEY_A:
			s_fPlayerSpeedX = -20.0f;
			if (s_iPlayerDirection == 1) {
				dSetSpriteFlipX("yhy", 1);
				dSetSpriteFlipX("yhyrun", 1);
			}
			break;
		case KEY_D:
			s_fPlayerSpeedX = 20.0f;
			if (s_iPlayerDirection == 0) {
				dSetSpriteFlipX("yhy", 0);
				dSetSpriteFlipX("yhyrun", 0);
			}
			break;
		}
	}
	if (g_iMap == 2) {
		switch (iKey) {
		case KEY_SPACE:
			if (g_bIsJumping == 0 ) {
				UpdateJumpInput();
			}
			break;
		case KEY_A:
		case KEY_D:
			dSetSpriteVisible("yhy", 0);
			dSetSpriteVisible("yhyrun", 1);
			UpdateMoveInput(iKey);
			break;
		}
	}
	if (g_iMap == 3) {
		switch (iKey) {
		case KEY_SPACE:
			if (g_bIsJumping == 0 ) {
				s_fPlayerSpeedY = -g_fJumpForce;  // 负值表示向上
				g_bIsJumping = 1;
				g_bOnGround = 0;
			}
			break;
		case KEY_A:
			s_fPlayerSpeedX = -20.0f;
			if (s_iPlayerDirection == 1) {
				dSetSpriteFlipX("yhy", 1);
				dSetSpriteFlipX("yhyrun", 1);
			}
			break;
		case KEY_D:
			s_fPlayerSpeedX = 20.0f;
			if (s_iPlayerDirection == 0) {
				dSetSpriteFlipX("yhy", 0);
				dSetSpriteFlipX("yhyrun", 0);
			}
			break;
		}
	}
	// 更新水平速度（垂直速度保持不变）
	dSetSpriteLinearVelocity("yhy", s_fPlayerSpeedX, s_fPlayerSpeedY);
	/*===============================*/
	if(g_iMapStage == 1 && g_iMap==4){
		if(KEY_SPACE==iKey){
			g_iMapStage = 0;
			dLoadMap("start.t2d");
		}
	}
	
}
//==========================================================================
//
// 引擎捕捉键盘弹起消息后，将调用到本函数
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
//
void dOnKeyUp( const int iKey ) {
	// 可以在此添加游戏需要的响应函数
	OnKeyUp(iKey);
	//切换为静止
	if(g_iMap==1){
		if (KEY_A == iKey || KEY_D == iKey) {
			dSetSpriteVisible("yhy", 1);
			dSetSpriteVisible("yhyrun", 0);
			// 释放A键或D键时停止水平移动
			s_fPlayerSpeedX = 0.0f;
			dSetSpriteLinearVelocity("yhy", s_fPlayerSpeedX, s_fPlayerSpeedY);
			if (iKey == KEY_A) s_iPlayerDirection = 0;
			if (iKey == KEY_D) s_iPlayerDirection = 1;
		}
	}
	if(g_iMap==2){
		if (iKey == KEY_A || iKey == KEY_D) {
			dSetSpriteVisible("yhy", 1);
			dSetSpriteVisible("yhyrun", 0);

			// 停止对应轴向速度
			if (GravityDirection == 0 || GravityDirection == 1) {
				s_fPlayerSpeedX = 0;
			} else {
				s_fPlayerSpeedY = 0;
			}

			// 更新线速度
			dSetSpriteLinearVelocity("yhy", s_fPlayerSpeedX, s_fPlayerSpeedY);

			// 更新朝向
			if (iKey == KEY_A) s_iPlayerDirection = 0;
			if (iKey == KEY_D) s_iPlayerDirection = 1;
		}
	}
	if (g_iMap == 3) {
		if (iKey == KEY_A || iKey == KEY_D) {
			dSetSpriteVisible("yhy", 1);
			dSetSpriteVisible("yhyrun", 0);

			if (GravityDirection == 0 || GravityDirection == 1)
				s_fPlayerSpeedX = 0;
			else
				s_fPlayerSpeedY = 0;

			dSetSpriteLinearVelocity("yhy", s_fPlayerSpeedX, s_fPlayerSpeedY);

			if (iKey == KEY_A) s_iPlayerDirection = 0;
			if (iKey == KEY_D) s_iPlayerDirection = 1;
		}
	}
}

//===========================================================================
//
// 引擎捕捉到精灵与精灵碰撞之后，调用此函数
// 精灵之间要产生碰撞，必须在编辑器或者代码里设置精灵发送及接受碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
//
void dOnSpriteColSprite( const char *szSrcName, const char *szTarName ) {
	// 可以在此添加游戏需要的响应函数
	dSetSpriteCollisionMaxIterations("yhy", 10);
	if (g_iMap == 1) {
		/*=================================*/
		HandleBatteryCollision(szSrcName, szTarName);
		/*================================*/
		if ((strcmp(szSrcName, "yhy") == 0 && strstr(szTarName, "platform") != NULL) ||
			(strcmp(szTarName, "yhy") == 0 && strstr(szSrcName, "platform") != NULL)) {
				const char* szPlatform = strstr(szTarName, "platform") ? szTarName : szSrcName;
				// 如果玩家当前速度是向下的（即 s_fPlayerSpeedY > 0），说明是从上往下掉落平台
				if (s_fPlayerSpeedY > 0) {
					s_fPlayerSpeedY = 0;
					g_bOnGround = 1;
					g_bIsJumping = 0;
				}
		}

		if ((strcmp(szSrcName, "yhy") == 0 && strstr(szTarName, "men") != NULL) ||
			(strcmp(szTarName, "yhy") == 0 && strstr(szSrcName, "men") != NULL)) {
				if (BatteryNumber >= 3) {
					IsLoadMap2 = 1;
					BatteryNumber = 0; // 重置电池计数
				}
		}

		if ( (strcmp(szSrcName, "yhy") == 0 && strstr(szTarName, "jig") != NULL) ||
			(strcmp(szTarName, "yhy") == 0 && strstr(szSrcName, "jig") != NULL) ) {
				dSetSpritePosition("yhy", -46.705, 16.076);
		}
	}
	/*================================*/
	if (g_iMap == 2) {
		if ((strcmp(szSrcName, "yhy") == 0 && strstr(szTarName, "btn_") != NULL) ||
			(strcmp(szTarName, "yhy") == 0 && strstr(szSrcName, "btn_") != NULL)) {
				// 获取碰撞的按钮名称
				const char* szButton = (strcmp(szSrcName, "yhy") == 0) ? szTarName : szSrcName;
				//播放音效
				PlaySound("game/data/audio/btn.wav",0,1);
				//根据名称设置重力方向
				if (strstr(szButton, "0"))
					GravityDirection = 0;
				if (strstr(szButton, "1"))
					GravityDirection = 1;
				if (strstr(szButton, "2"))
					GravityDirection = 2;
				if (strstr(szButton, "3"))
					GravityDirection = 3;
				g_bOnGround = false; // 离开地面
				UpdatePlayerRotationByGravity();
		}

		//如果重力向下：速度Y > 0 表示落地
		//如果重力向上：速度Y < 0 表示落地
		//如果重力向右：速度X > 0 表示落地
		//如果重力向左：速度X < 0 表示落地

		if ((strcmp(szSrcName, "yhy") == 0 && strstr(szTarName, "platform") != NULL) ||
			(strcmp(szTarName, "yhy") == 0 && strstr(szSrcName, "platform") != NULL)) {

				// 根据重力方向判断是否真正“落地”
				switch (GravityDirection) {
				case 0: // 重力向下
					if (s_fPlayerSpeedY > 0) {
						s_fPlayerSpeedY = 0;
						g_bOnGround = 1;
						g_bIsJumping = 0;
					}
					break;
				case 1: // 重力向上
					if (s_fPlayerSpeedY < 0) {
						s_fPlayerSpeedY = 0;
						g_bOnGround = 1;
						g_bIsJumping = 0;
					}
					break;
				case 2: // 重力向右
					if (s_fPlayerSpeedX > 0) {
						s_fPlayerSpeedX = 0;
						g_bOnGround = 1;
						g_bIsJumping = 0;
					}
					break;
				case 3: // 重力向左
					if (s_fPlayerSpeedX < 0) {
						s_fPlayerSpeedX = 0;
						g_bOnGround = 1;
						g_bIsJumping = 0;
					}
					break;
				}
		}
		if ((strcmp(szSrcName, "yhy") == 0 && strstr(szTarName, "core") != NULL) ||
			(strcmp(szTarName, "yhy") == 0 && strstr(szSrcName, "core") != NULL)) {

				// 获取当前 core 名称
				const char* szHeart = strstr(szSrcName, "core") ? szSrcName : szTarName;

				//  停止其运动
				dSetSpriteLinearVelocity(szHeart, 0, 0);
				dSetSpriteAutoMassInertia(szHeart, 0);

				// 把它移动到 yhy 附近，防止挂接瞬移出错
				float x = dGetSpritePositionX("yhy");
				float y = dGetSpritePositionY("yhy");
				dSetSpritePosition(szHeart, x, y);

				// 禁用碰撞（防止之后飞出）
				dSetSpriteCollisionActive(szHeart,0,0);

				// 执行挂接（Link Point 1）
				dSpriteMountToSpriteLinkPoint(szHeart, "yhy", 1);

				//使得core跟随yhy移动
				dSetSpriteAutoMassInertia(szHeart, 0);
				dSetSpriteLinearVelocity(szHeart, 0, 0);
		}

		if ((strcmp(szSrcName, "yhy") == 0 && strcmp(szTarName, "engine")==0) ||
			(strcmp(szTarName, "yhy") == 0 && strcmp(szSrcName, "engine")==0)	){
				if(dGetSpriteCollisionReceive("core")==0)
					IsLoadMap3=1;
		}
		if ( (strcmp(szSrcName, "yhy") == 0 && strstr(szTarName, "jig") != NULL) ||
			(strcmp(szTarName, "yhy") == 0 && strstr(szSrcName, "jig") != NULL) ) {
				dSetSpritePosition("yhy", -47.000, 30);
				GravityDirection = 0;
				dSetSpriteLinearVelocity("yhy", 0, 0);
				dSetSpriteRotation("yhy", 0);
				dSetSpriteRotation("yhyrun", 0);
		}
	}
	if(g_iMap==3){
		if( (strcmp(szSrcName, "yhy") == 0 && strstr(szTarName, "lengqueji") != NULL) ||
			(strcmp(szTarName, "yhy") == 0 && strstr(szSrcName, "lengqueji") != NULL) )
		{
			const char *szLengquejiToDelete = (strcmp(szSrcName, "yhy") == 0) ? szTarName : szSrcName;
			dDeleteSprite(szLengquejiToDelete);
			lengNumber--;
		}
		if ((strcmp(szSrcName, "yhy") == 0 && strstr(szTarName, "platform") != NULL) ||
			(strcmp(szTarName, "yhy") == 0 && strstr(szSrcName, "platform") != NULL)) {
				const char* szPlatform = strstr(szTarName, "platform") ? szTarName : szSrcName;
				// 如果玩家当前速度是向下的（即 s_fPlayerSpeedY > 0），说明是从上往下掉落平台
				if (s_fPlayerSpeedY > 1.0f) {
					s_fPlayerSpeedY = 0;
					g_bOnGround = 1;
					g_bIsJumping = 0;
				}
		}
	}

	/*================================*/


	OnSpriteColSprite(szSrcName, szTarName);
}

//===========================================================================
//
// 引擎捕捉到精灵与世界边界碰撞之后，调用此函数.
// 精灵之间要产生碰撞，必须在编辑器或者代码里设置精灵的世界边界限制
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
//
void dOnSpriteColWorldLimit( const char *szName, const int iColSide ) {
	// 可以在此添加游戏需要的响应函数
	OnSpriteColWorldLimit(szName, iColSide);
	if (strcmp(szName, "yhy") == 0 )
		dSetSpriteLinearVelocity("yhy", 0, 0);
	if (strcmp(szName, "yhyrun") == 0 )
		dSetSpriteLinearVelocity("yhyrun", 0, 0);
}


//不同重力下的跳跃方向函数
void UpdateJumpInput() {
	if (g_bIsJumping == 0) {
		switch (GravityDirection) {
		case 0: // 向下重力，跳跃向上
			s_fPlayerSpeedY = -g_fJumpForce;
			break;
		case 1: // 向上重力，跳跃向下
			s_fPlayerSpeedY = g_fJumpForce;
			break;
		case 2: // 向右重力，跳跃向左
			s_fPlayerSpeedX = -g_fJumpForce;
			break;
		case 3: // 向左重力，跳跃向右
			s_fPlayerSpeedX = g_fJumpForce;
			break;
		}
		g_bIsJumping = 1;
		g_bOnGround = 0;
	}
}

//调整A\D控制的移动方向
void UpdateMoveInput(int iKey) {
	switch (GravityDirection) {
	case 0:// 重力向下，正常横向移动
		if (iKey == KEY_A) {
			s_fPlayerSpeedX = -20.0f;
			dSetSpriteFlipX("yhy", 1);
			dSetSpriteFlipX("yhyrun", 1);
			s_iPlayerDirection = 0;
		}
		if (iKey == KEY_D) {
			s_fPlayerSpeedX = 20.0f;
			dSetSpriteFlipX("yhy", 0);
			dSetSpriteFlipX("yhyrun", 0);
			s_iPlayerDirection = 1;
		}
		break;
	case 1:
		if (iKey == KEY_A) {
			s_fPlayerSpeedX = -20.0f;
			dSetSpriteFlipX("yhy", 0);
			dSetSpriteFlipX("yhyrun", 0);
			s_iPlayerDirection = 1;
		}
		if (iKey == KEY_D) {
			s_fPlayerSpeedX = 20.0f;
			dSetSpriteFlipX("yhy", 1);
			dSetSpriteFlipX("yhyrun", 1);
			s_iPlayerDirection = 0;
		}
		break;
	case 2: // 重力向右，移动为上下方向（A/D 控制Y轴）
	case 3:
		if (iKey == KEY_A) {
			s_fPlayerSpeedY = -20.0f;
			s_iPlayerDirection = 0;
		}
		if (iKey == KEY_D) {
			s_fPlayerSpeedY = 20.0f;
			s_iPlayerDirection = 1;
		}
		break;
	}
}
//================================
//重力旋转处理函数
/*==================================================
| 重力方向  | 视觉期望				| 对应操作			|
| --------------------------------------------------|
| 向下（0） | yhy 脚朝下（默认）	| 不旋转				|
| 向上（1） | yhy 头朝下			| 旋转 180°			|
| 向右（2） | yhy 向右踩墙			| 旋转 -90°			|
| 向左（3） | yhy 向左踩墙			| 旋转 90°	|
===================================================*/
void UpdatePlayerRotationByGravity() {
	switch (GravityDirection) {
	case 0: // DOWN
		dSetSpriteRotation("yhy", 0);
		dSetSpriteRotation("yhyrun", 0);
		break;
	case 1: // UP
		dSetSpriteRotation("yhy", 180);
		dSetSpriteRotation("yhyrun", 180);
		break;
	case 2: // RIGHT
		dSetSpriteRotation("yhy", -90);
		dSetSpriteRotation("yhyrun", -90);
		break;
	case 3: // LEFT
		dSetSpriteRotation("yhy", 90); // 或 270
		dSetSpriteRotation("yhyrun", 90);
		break;
	}
}