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
int			g_iGameState		=	0;		// ��Ϸ״̬��0 -- ��Ϸ�����ȴ���ʼ״̬��1 -- ���¿ո����ʼ����ʼ����Ϸ��2 -- ��Ϸ������
extern int g_iMapStage;
void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();
extern int IsLoadMap2;
extern int IsLoadMap3;
extern char jig[100];

extern int lengNumber;//��ȴ��

float g_fCountdownTime = 40.0f;  // ��ʼ
bool g_bTimerRunning = false;//��ʱ����
void StartTimer(float seconds) {  //��ʱ��ʼ����������ʱ����ʱ�伴�ɿ�ʼ��ʱ
    g_fCountdownTime = seconds;
    g_bTimerRunning = true;
}


//==============================================================================
//
// ����ĳ�������Ϊ��GameMainLoop����Ϊ��ѭ��������������ÿ֡ˢ����Ļͼ��֮�󣬶��ᱻ����һ�Ρ�


//==============================================================================
//
// ��Ϸ��ѭ�����˺���������ͣ�ĵ��ã�����ÿˢ��һ����Ļ���˺�����������һ��
// ���Դ�����Ϸ�Ŀ�ʼ�������С������ȸ���״̬. 
// ��������fDeltaTime : �ϴε��ñ��������˴ε��ñ�������ʱ��������λ����
void GameMainLoop( float	fDeltaTime )
{
	switch( g_iGameState )
	{
		// ��ʼ����Ϸ�������һ���������
	case 1:
		{
			GameInit();
			g_iGameState	=	2; // ��ʼ��֮�󣬽���Ϸ״̬����Ϊ������
		}
		break;

		// ��Ϸ�����У����������Ϸ�߼�
	case 2:
		{
			// TODO �޸Ĵ˴���Ϸѭ�������������ȷ��Ϸ�߼�
			if( true )
			{
				GameRun( fDeltaTime );
			}
			else
			{
				// ��Ϸ������������Ϸ���㺯����������Ϸ״̬�޸�Ϊ����״̬
				g_iGameState	=	0;
				GameEnd();
			}
		}
		break;

		// ��Ϸ����/�ȴ����ո����ʼ
	case 0:
	default:
		break;
	};
}

//==============================================================================
//
// ÿ�ֿ�ʼǰ���г�ʼ���������һ���������
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
    StartTimer(40.0f);  // ����ʱ120��
}
}
//==============================================================================
//
// ÿ����Ϸ������
void GameRun( float fDeltaTime )
{	// ÿ�����������״̬����
	static float fLaserVisibleTime[4] = {0};
	static float gBornJiguangTime[4] = {3.0f, 2.5f, 2.0f, 1.5f}; // ��ͬ��ȴʱ��
	if(g_iMap == 1||g_iMap==2) { 
		/*================== ���Ƽ����������� ========================*/
		for(int i = 0; i < 4; i++) {
			// ��ȫ���ɼ�������
			char szName[32];
			sprintf(szName, "jig%d", i);  // ��������Ϊjig0��jig3

			// ÿ�����������״̬����
			static float fLaserVisibleTime[4] = {0};
			static float gBornJiguangTime[4] = {3.0f, 2.5f, 2.0f, 1.5f}; // ��ͬ��ȴʱ��

			// ��ȴ�׶�
			if(gBornJiguangTime[i] > 0) {
				gBornJiguangTime[i] -= fDeltaTime;

				// ��ȴ�����Ҽ���δ��ʾʱ
				if(gBornJiguangTime[i] <= 0 && !dIsSpriteVisible(szName)) {
					PlaySound("game/data/audio/jg.wav",0,1);
					fLaserVisibleTime[i] = 1.0f; // ��ʾ1��
					dSetSpriteVisible(szName, 1);
					dSetSpriteCollisionReceive(szName, 1);
				}
			} 
			// ��ʾ�׶�
			else if(fLaserVisibleTime[i] > 0) {
				fLaserVisibleTime[i] -= fDeltaTime;

				// ��ʾʱ�䵽
				if(fLaserVisibleTime[i] <= 0) {
					dSetSpriteVisible(szName, 0);
					dSetSpriteCollisionReceive(szName, 0);
					gBornJiguangTime[i] = 3.0f + i * 0.5f; // ���컯��ȴ
				}
			}
		}
	}
	if (IsLoadMap2 == 1) {
		dLoadMap("intro2.t2d");   // �ȼ��ؾ����ͼ
		g_iMap = 2;
		g_iMapStage = 1;          // ��ʾ���ھ����ͼ��
		g_iGameState = 0;         // ��ͣ��ʽ����
		IsLoadMap2 = 0;
	}
	if (IsLoadMap3 == 1) {
		dLoadMap("intro3.t2d");  // ���ؾ����ͼ
		g_iMap = 3;
		g_iMapStage = 1;         // ����׶�
		g_iGameState = 0;        // ��ͣ��ʽ����
		IsLoadMap3 = 0;
	}
	/*==================================================*/
	g_bOnGround = false;

	if(g_iMap==3){
		// ����ʱ����
		if (g_bTimerRunning) {
			g_fCountdownTime -= fDeltaTime;
			// ʱ�䵽
			if (g_fCountdownTime <= 0) {
				g_bTimerRunning = false;
				PlaySound("alarm.wav", 0, 1); // ��ը����
			}
			dSetTextValue("timer_text", (int)(g_fCountdownTime + 1));
		}

	}
	if(g_iMap==3)
		if(CheckCircuitConnected()&&lengNumber==0){
			dLoadMap("intro4.t2d");  // ���ؾ����ͼ
			g_iMap = 4;
			g_iMapStage = 1;         // ����׶�
			g_iGameState = 0;
		}

	

}
//==============================================================================
//
// ������Ϸ����
void GameEnd()
{
}
//==========================================================================
//
// ����ƶ�
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseMove( const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// �����
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// ��굯��
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// ���̰���
// ���� iKey�������µļ���ֵ�� enum KeyCodes �궨��
// ���� iAltPress, iShiftPress��iCtrlPress�������ϵĹ��ܼ�Alt��Ctrl��Shift��ǰ�Ƿ�Ҳ���ڰ���״̬(0δ���£�1����)
void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{	

}
//==========================================================================
//
// ���̵���
// ���� iKey������ļ���ֵ�� enum KeyCodes �궨��
void OnKeyUp( const int iKey )
{

}
//===========================================================================
//
// �����뾫����ײ
// ���� szSrcName��������ײ�ľ�������
// ���� szTarName������ײ�ľ�������
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{

}
//===========================================================================
//
// ����������߽���ײ
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{

}