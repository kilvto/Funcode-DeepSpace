#ifndef MYLIB_H
#define MYLIB_H

#include "CommonAPI.h"

#define MAX_BATTERY 10

extern int mOnMouseinSprite(const float fMouseX, const float fMouseY, const char *szName);

// 电池结构体定义
typedef struct {
	char name[32];     // 精灵名称
	int collected;     // 是否已拾取
	float x, y;        // 坐标
} Battery;

// 声明电池函数
void InitBattery(const char* name, float x, float y);
void ResetBatteries();
void HandleBatteryCollision(const char* spriteA, const char* spriteB);


#define GRID_WIDTH 3
#define GRID_HEIGHT 3
#define MAX_BLOCKS (GRID_WIDTH * GRID_HEIGHT)
//电路拼图起始坐标和大小
#define GRID_ORIGIN_X -39
#define GRID_ORIGIN_Y -25
#define BLOCK_SIZE    10

typedef struct {
	char name[16];    // 精灵名称，如 "block1"
	int gridX, gridY; // 当前所处网格坐标
	int type;         // 电路类型（编号决定连接方向）
} CircuitBlock;

extern CircuitBlock g_Blocks[MAX_BLOCKS];

// 初始化拼图布局
extern void InitCircuitPuzzle();

// 处理鼠标点击拼图交换
extern void HandleCircuitClick(float fX, float fY);

// 检查拼图是否连通成功
extern int CheckCircuitConnected();


#endif