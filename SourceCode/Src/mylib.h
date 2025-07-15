#ifndef MYLIB_H
#define MYLIB_H

#include "CommonAPI.h"

#define MAX_BATTERY 10

extern int mOnMouseinSprite(const float fMouseX, const float fMouseY, const char *szName);

// ��ؽṹ�嶨��
typedef struct {
	char name[32];     // ��������
	int collected;     // �Ƿ���ʰȡ
	float x, y;        // ����
} Battery;

// ������غ���
void InitBattery(const char* name, float x, float y);
void ResetBatteries();
void HandleBatteryCollision(const char* spriteA, const char* spriteB);


#define GRID_WIDTH 3
#define GRID_HEIGHT 3
#define MAX_BLOCKS (GRID_WIDTH * GRID_HEIGHT)
//��·ƴͼ��ʼ����ʹ�С
#define GRID_ORIGIN_X -39
#define GRID_ORIGIN_Y -25
#define BLOCK_SIZE    10

typedef struct {
	char name[16];    // �������ƣ��� "block1"
	int gridX, gridY; // ��ǰ������������
	int type;         // ��·���ͣ���ž������ӷ���
} CircuitBlock;

extern CircuitBlock g_Blocks[MAX_BLOCKS];

// ��ʼ��ƴͼ����
extern void InitCircuitPuzzle();

// ���������ƴͼ����
extern void HandleCircuitClick(float fX, float fY);

// ���ƴͼ�Ƿ���ͨ�ɹ�
extern int CheckCircuitConnected();


#endif