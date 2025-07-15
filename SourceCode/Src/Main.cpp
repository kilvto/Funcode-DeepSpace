//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include "CommonAPI.h"
#include "LessonX.h"
#include "mylib.h"
#include "math.h"
#include "stdio.h"
int g_iMap = 0; //��ͼ״̬������0->��ʼ�����ͼ
extern int g_iGameState;
int g_iMapStage = 0;// 0: ��ʼ���棬1: �����ͼ�У�2: ��ʽ��ͼ��

void UpdateJumpInput();
void UpdateMoveInput(int iKey);
void UpdatePlayerRotationByGravity();


//yhy���߲���
bool g_bOnGround = 0; // �Ƿ�վ��ƽ̨��
static float s_fPlayerSpeedX = 0;        // ˮƽ�ƶ��ٶ�
static bool g_bIsJumping = 0;       // ��Ծ״̬��־
static const float Gravity = 55.0f;      // �������ٶ�
static float Force;					//֧����
static float s_fPlayerSpeedY = 0;   // ��ֱ�����ٶ�
static const float g_fJumpForce = 20.0f;  // ��Ծ���ٶ�
int s_iPlayerDirection = 1;				//��ҳ����״̬��1->���ң�0��>����

int lengNumber=4;
int BatteryNumber = 0;
int IsLoadMap2;//�Ƿ������һ�ص�״̬����
int IsLoadMap3;
int GravityDirection = 0; //0->DOWN,1->UP,2->RIGHT,3->LEFT��������״̬����


float		g_fScreenLeft		=	0.f;    // ��Ļ��߽�ֵ
float		g_fScreenRight		=	0.f;    // ��
float		g_fScreenTop		=	0.f;    // ��
float		g_fScreenBottom	=	0.f;


char jig[100];//������������

//���������л�����
void mSwitchGravity(int x, float Gravity, float Force, float *fX, float *fY, float fTimeDelta) {
	switch (x) {
	case 0 :
		s_fPlayerSpeedY += (Gravity + Force) * fTimeDelta;//��
		(*fY) += s_fPlayerSpeedY * fTimeDelta;
		break;
	case 1 :
		s_fPlayerSpeedY += -(Gravity + Force) * fTimeDelta;//��
		(*fY) += s_fPlayerSpeedY * fTimeDelta;
		break;
	case 2 :
		s_fPlayerSpeedX += (Gravity + Force) * fTimeDelta; //��
		(*fX) += s_fPlayerSpeedX * fTimeDelta;
		break;
	case 3 :
		s_fPlayerSpeedX += -(Gravity + Force) * fTimeDelta; //��
		(*fX) += s_fPlayerSpeedX * fTimeDelta;
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
//
// ���������
//
//////////////////////////////////////////////////////////////////////////////////////////
int PASCAL WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow) {
		// ��ʼ����Ϸ����
		if ( !dInitGameEngine( hInstance, lpCmdLine ) )
			return 0;

		// To do : �ڴ�ʹ��API���Ĵ��ڱ���
		dSetWindowTitle("Lesson");
		//����߽�
		g_fScreenLeft	 	= 	dGetScreenLeft();
		g_fScreenRight  	= 	dGetScreenRight();
		g_fScreenTop 	 	= 	dGetScreenTop();
		g_fScreenBottom 	= 	dGetScreenBottom();

		dSetSpriteWorldLimit("yhy", WORLD_LIMIT_NULL, g_fScreenLeft - 23,
			g_fScreenTop, g_fScreenRight + 24, g_fScreenBottom);
		dSetSpriteWorldLimit("yhyrun", WORLD_LIMIT_NULL, g_fScreenLeft,
			g_fScreenTop, g_fScreenRight, g_fScreenBottom);

		for (int i = 0; i < 4; i++) {
			sprintf(&jig[i], "jig%d", i); //����������������
		}

		// ������ѭ����������Ļͼ��ˢ�µȹ���
		while ( dEngineMainLoop() ) {
			// ��ȡ���ε���֮���ʱ�����ݸ���Ϸ�߼�����
			float fTimeDelta =	dGetTimeDelta();
			// ��ȡ��ǰ����λ��
			float fX, fY;
			fX = dGetSpritePositionX("yhy");
			fY = dGetSpritePositionY("yhy");

			if (g_iMap == 1) {
				// ��ֱ�˶���ֻ�в��ڵ���/ƽ̨��ʱ��������Ӱ�죩
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
				// ��ֱ�˶���ֻ�в��ڵ���/ƽ̨��ʱ��������Ӱ�죩
				if (g_bOnGround == 1) Force = -Gravity;
				else Force = 0;
				s_fPlayerSpeedY += (Gravity + Force) * fTimeDelta;
				// ��������ٶȣ�������
				if (s_fPlayerSpeedY > 25) s_fPlayerSpeedY = 25;
				fY += s_fPlayerSpeedY * fTimeDelta;
			}

			// ���¾���λ��
			dSetSpritePosition("yhy", fX, fY);
			// ִ����Ϸ��ѭ��
			GameMainLoop( fTimeDelta );
		};

		// �ر���Ϸ����
		dShutdownGameEngine();
		return 0;
}

//==========================================================================
//
// ���沶׽����ƶ���Ϣ�󣬽����õ�������
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//
void dOnMouseMove( const float fMouseX, const float fMouseY ) {
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseMove(fMouseX, fMouseY );
}
//==========================================================================
//
// ���沶׽�������Ϣ�󣬽����õ�������
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//
void dOnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY ) {
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseClick(iMouseType, fMouseX, fMouseY);
	if (g_iMap == 0){
		if (g_iMapStage == 0 && iMouseType == 0 && mOnMouseinSprite(fMouseX, fMouseY, "start")) {
			dLoadMap("intro1.t2d");  // ���ؾ����ͼ
			g_iMapStage = 1;         // ���ھ����ͼ��
		}
		if (g_iMapStage == 1 && iMouseType == 1) {
			dLoadMap("guan1.t2d");
			g_iMap = 1;
			g_iGameState = 1;
			g_iMapStage = 2;
		}
	}
	// �����ǰ�ǵڶ��صľ����ͼ�׶Σ�����������ʽ��ͼ
	if (g_iMap == 2 && g_iMapStage == 1 && iMouseType == 0) {
		dLoadMap("guan2.t2d");
		g_iMapStage = 2;       // ������ʽ��ͼ
		g_iGameState = 1;      // ��ʼ��ʼ��
		return;
	}
	if (g_iMap == 3 && g_iMapStage == 1 && iMouseType == 0) {
		dLoadMap("guan3.t2d");    // ������ʽ��ͼ
		g_iMapStage = 2;
		g_iGameState = 1;         // ���� GameInit()
		return;
	}
	if (g_iMap == 3&& iMouseType == 0) {
		HandleCircuitClick(fMouseX, fMouseY);
	}
}
//==========================================================================
//
// ���沶׽��굯����Ϣ�󣬽����õ�������
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//
void dOnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY ) {
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseUp(iMouseType, fMouseX, fMouseY);

}
//==========================================================================
//
// ���沶׽���̰�����Ϣ�󣬽����õ�������
// ���� iKey�������µļ���ֵ�� enum KeyCodes �궨��
// ���� iAltPress, iShiftPress��iCtrlPress�������ϵĹ��ܼ�Alt��Ctrl��Shift��ǰ�Ƿ�Ҳ���ڰ���״̬(0δ���£�1����)
//
void dOnKeyDown( const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress ) {
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnKeyDown(iKey, iAltPress, iShiftPress, iCtrlPress);


	//�л�Ϊ�ܶ�
	if (KEY_A == iKey || KEY_D == iKey) {
		dSetSpriteVisible("yhy", 0);
		dSetSpriteVisible("yhyrun", 1);
	}

	if (g_iMap == 1) {
		/*===============guan1==============*/
		switch (iKey) {
		case KEY_SPACE:
			if (g_bIsJumping == 0 ) {
				s_fPlayerSpeedY = -g_fJumpForce;  // ��ֵ��ʾ����
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
				s_fPlayerSpeedY = -g_fJumpForce;  // ��ֵ��ʾ����
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
	// ����ˮƽ�ٶȣ���ֱ�ٶȱ��ֲ��䣩
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
// ���沶׽���̵�����Ϣ�󣬽����õ�������
// ���� iKey������ļ���ֵ�� enum KeyCodes �궨��
//
void dOnKeyUp( const int iKey ) {
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnKeyUp(iKey);
	//�л�Ϊ��ֹ
	if(g_iMap==1){
		if (KEY_A == iKey || KEY_D == iKey) {
			dSetSpriteVisible("yhy", 1);
			dSetSpriteVisible("yhyrun", 0);
			// �ͷ�A����D��ʱֹͣˮƽ�ƶ�
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

			// ֹͣ��Ӧ�����ٶ�
			if (GravityDirection == 0 || GravityDirection == 1) {
				s_fPlayerSpeedX = 0;
			} else {
				s_fPlayerSpeedY = 0;
			}

			// �������ٶ�
			dSetSpriteLinearVelocity("yhy", s_fPlayerSpeedX, s_fPlayerSpeedY);

			// ���³���
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
// ���沶׽�������뾫����ײ֮�󣬵��ô˺���
// ����֮��Ҫ������ײ�������ڱ༭�����ߴ��������þ��鷢�ͼ�������ײ
// ���� szSrcName��������ײ�ľ�������
// ���� szTarName������ײ�ľ�������
//
void dOnSpriteColSprite( const char *szSrcName, const char *szTarName ) {
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	dSetSpriteCollisionMaxIterations("yhy", 10);
	if (g_iMap == 1) {
		/*=================================*/
		HandleBatteryCollision(szSrcName, szTarName);
		/*================================*/
		if ((strcmp(szSrcName, "yhy") == 0 && strstr(szTarName, "platform") != NULL) ||
			(strcmp(szTarName, "yhy") == 0 && strstr(szSrcName, "platform") != NULL)) {
				const char* szPlatform = strstr(szTarName, "platform") ? szTarName : szSrcName;
				// �����ҵ�ǰ�ٶ������µģ��� s_fPlayerSpeedY > 0����˵���Ǵ������µ���ƽ̨
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
					BatteryNumber = 0; // ���õ�ؼ���
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
				// ��ȡ��ײ�İ�ť����
				const char* szButton = (strcmp(szSrcName, "yhy") == 0) ? szTarName : szSrcName;
				//������Ч
				PlaySound("game/data/audio/btn.wav",0,1);
				//��������������������
				if (strstr(szButton, "0"))
					GravityDirection = 0;
				if (strstr(szButton, "1"))
					GravityDirection = 1;
				if (strstr(szButton, "2"))
					GravityDirection = 2;
				if (strstr(szButton, "3"))
					GravityDirection = 3;
				g_bOnGround = false; // �뿪����
				UpdatePlayerRotationByGravity();
		}

		//����������£��ٶ�Y > 0 ��ʾ���
		//����������ϣ��ٶ�Y < 0 ��ʾ���
		//����������ң��ٶ�X > 0 ��ʾ���
		//������������ٶ�X < 0 ��ʾ���

		if ((strcmp(szSrcName, "yhy") == 0 && strstr(szTarName, "platform") != NULL) ||
			(strcmp(szTarName, "yhy") == 0 && strstr(szSrcName, "platform") != NULL)) {

				// �������������ж��Ƿ���������ء�
				switch (GravityDirection) {
				case 0: // ��������
					if (s_fPlayerSpeedY > 0) {
						s_fPlayerSpeedY = 0;
						g_bOnGround = 1;
						g_bIsJumping = 0;
					}
					break;
				case 1: // ��������
					if (s_fPlayerSpeedY < 0) {
						s_fPlayerSpeedY = 0;
						g_bOnGround = 1;
						g_bIsJumping = 0;
					}
					break;
				case 2: // ��������
					if (s_fPlayerSpeedX > 0) {
						s_fPlayerSpeedX = 0;
						g_bOnGround = 1;
						g_bIsJumping = 0;
					}
					break;
				case 3: // ��������
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

				// ��ȡ��ǰ core ����
				const char* szHeart = strstr(szSrcName, "core") ? szSrcName : szTarName;

				//  ֹͣ���˶�
				dSetSpriteLinearVelocity(szHeart, 0, 0);
				dSetSpriteAutoMassInertia(szHeart, 0);

				// �����ƶ��� yhy ��������ֹ�ҽ�˲�Ƴ���
				float x = dGetSpritePositionX("yhy");
				float y = dGetSpritePositionY("yhy");
				dSetSpritePosition(szHeart, x, y);

				// ������ײ����ֹ֮��ɳ���
				dSetSpriteCollisionActive(szHeart,0,0);

				// ִ�йҽӣ�Link Point 1��
				dSpriteMountToSpriteLinkPoint(szHeart, "yhy", 1);

				//ʹ��core����yhy�ƶ�
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
				// �����ҵ�ǰ�ٶ������µģ��� s_fPlayerSpeedY > 0����˵���Ǵ������µ���ƽ̨
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
// ���沶׽������������߽���ײ֮�󣬵��ô˺���.
// ����֮��Ҫ������ײ�������ڱ༭�����ߴ��������þ��������߽�����
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
//
void dOnSpriteColWorldLimit( const char *szName, const int iColSide ) {
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnSpriteColWorldLimit(szName, iColSide);
	if (strcmp(szName, "yhy") == 0 )
		dSetSpriteLinearVelocity("yhy", 0, 0);
	if (strcmp(szName, "yhyrun") == 0 )
		dSetSpriteLinearVelocity("yhyrun", 0, 0);
}


//��ͬ�����µ���Ծ������
void UpdateJumpInput() {
	if (g_bIsJumping == 0) {
		switch (GravityDirection) {
		case 0: // ������������Ծ����
			s_fPlayerSpeedY = -g_fJumpForce;
			break;
		case 1: // ������������Ծ����
			s_fPlayerSpeedY = g_fJumpForce;
			break;
		case 2: // ������������Ծ����
			s_fPlayerSpeedX = -g_fJumpForce;
			break;
		case 3: // ������������Ծ����
			s_fPlayerSpeedX = g_fJumpForce;
			break;
		}
		g_bIsJumping = 1;
		g_bOnGround = 0;
	}
}

//����A\D���Ƶ��ƶ�����
void UpdateMoveInput(int iKey) {
	switch (GravityDirection) {
	case 0:// �������£����������ƶ�
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
	case 2: // �������ң��ƶ�Ϊ���·���A/D ����Y�ᣩ
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
//������ת������
/*==================================================
| ��������  | �Ӿ�����				| ��Ӧ����			|
| --------------------------------------------------|
| ���£�0�� | yhy �ų��£�Ĭ�ϣ�	| ����ת				|
| ���ϣ�1�� | yhy ͷ����			| ��ת 180��			|
| ���ң�2�� | yhy ���Ҳ�ǽ			| ��ת -90��			|
| ����3�� | yhy �����ǽ			| ��ת 90��	|
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
		dSetSpriteRotation("yhy", 90); // �� 270
		dSetSpriteRotation("yhyrun", 90);
		break;
	}
}