#include "mylib.h"
#include "CommonAPI.h"
#include "LessonX.h"
#include "stdio.h"
#include "math.h"
//========================================================================================
int mOnMouseinSprite(const float fMouseX, const float fMouseY,const char *szName)
{
	float x, y, w, h;
	x=dGetSpritePositionX(szName);
	y=dGetSpritePositionY(szName);
	w=dGetSpriteWidth(szName);
	h=dGetSpriteHeight(szName);
	if(fMouseX >= x-w && fMouseY <=x+w && fMouseY>=y-h && fMouseY<= y+h)
		return 1;
	else return 0;
}
//===============================================================================
Battery g_Batteries[MAX_BATTERY];
int g_BatteryCount = 0;

void InitBattery(const char* name, float x, float y) {
    if (g_BatteryCount >= MAX_BATTERY) return;

    strcpy(g_Batteries[g_BatteryCount].name, name);
    g_Batteries[g_BatteryCount].x = x;
    g_Batteries[g_BatteryCount].y = y;
    g_Batteries[g_BatteryCount].collected = 0;

    // 假设你有 battery_template 精灵做模板
    dCloneSprite("battery_template", name);
    dSetSpritePosition(name, x, y);
    dSetSpriteCollisionActive(name, 1, 1);  // 开启发送和接收碰撞

    g_BatteryCount++;
}

void ResetBatteries() {
    g_BatteryCount = 0;
}

// 检查是否是电池精灵，并处理碰撞逻辑
void HandleBatteryCollision(const char* spriteA, const char* spriteB) {
    const char* szBattery = strstr(spriteA, "dianchi") ? spriteA :
                            (strstr(spriteB, "dianchi") ? spriteB : NULL);
    if (szBattery == NULL) return;

    for (int i = 0; i < g_BatteryCount; i++) {
        if (strcmp(g_Batteries[i].name, szBattery) == 0 && g_Batteries[i].collected == 0) {
            g_Batteries[i].collected = 1;
            dDeleteSprite(g_Batteries[i].name);
            // 你原先的变量
            extern int BatteryNumber;
            BatteryNumber++;
            break;
        }
    }
}


CircuitBlock g_Blocks[MAX_BLOCKS];
int g_SelectedIndex = -1;

// 方向bitmask定义：1=左 2=上 4=右 8=下
int g_BlockDirections[][4] = {
    {1, 0, 1, 0}, // 0 ━  左右
    {0, 1, 0, 1}, // 1 ┃  上下
    {0, 0, 1, 1}, // 2 ┏  右下
    {1, 0, 0, 1}, // 3 ┓  左下
    {0, 1, 1, 0}, // 4 ┗  右上
    {1, 1, 0, 0}  // 5 ┛  左上
};

// 判断一个块在某方向上是否有连接口
int HasConnection(int type, int dir) {
    return g_BlockDirections[type][dir];
}

// 初始化拼图（随机类型和位置）
void InitCircuitPuzzle() {
    int types[MAX_BLOCKS] = {0,1,2,3,4,5,0,1,2}; // 你自己设定的顺序

    for (int i = 0; i < MAX_BLOCKS; i++) {
        sprintf(g_Blocks[i].name, "block%d", i);
        g_Blocks[i].gridX = i % GRID_WIDTH;
        g_Blocks[i].gridY = i / GRID_WIDTH;
        g_Blocks[i].type = types[i];

        // 根据类型选择模板名
        char templateName[32];
        sprintf(templateName, "block%d_template", g_Blocks[i].type);

        // 克隆模板精灵
        dCloneSprite(templateName, g_Blocks[i].name);
        dSetSpritePosition(g_Blocks[i].name,
                           10 + g_Blocks[i].gridX * 10,
                           20 + g_Blocks[i].gridY * 10);
        dSetSpriteCollisionActive(g_Blocks[i].name, 0, 0);
    }

    g_SelectedIndex = -1;
}

// 处理点击交换逻辑
void HandleCircuitClick(float x, float y) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        float sx = 10 + g_Blocks[i].gridX * 10;
        float sy = 20 + g_Blocks[i].gridY * 10;
        if (fabsf(x - sx) < 5 && fabsf(y - sy) < 5) {
            if (g_SelectedIndex == -1) {
                g_SelectedIndex = i;
            } else {
                // 如果与上次点击的是邻近格子，则交换
                int dx = abs(g_Blocks[i].gridX - g_Blocks[g_SelectedIndex].gridX);
                int dy = abs(g_Blocks[i].gridY - g_Blocks[g_SelectedIndex].gridY);
                if ((dx + dy) == 1) {
                    int tempX = g_Blocks[i].gridX;
                    int tempY = g_Blocks[i].gridY;

                    g_Blocks[i].gridX = g_Blocks[g_SelectedIndex].gridX;
                    g_Blocks[i].gridY = g_Blocks[g_SelectedIndex].gridY;

                    g_Blocks[g_SelectedIndex].gridX = tempX;
                    g_Blocks[g_SelectedIndex].gridY = tempY;

                    // 更新位置
                    dSetSpritePosition(g_Blocks[i].name, 10 + g_Blocks[i].gridX * 10, 20 + g_Blocks[i].gridY * 10);
                    dSetSpritePosition(g_Blocks[g_SelectedIndex].name, 10 + g_Blocks[g_SelectedIndex].gridX * 10, 20 + g_Blocks[g_SelectedIndex].gridY * 10);
                }
                g_SelectedIndex = -1;
            }
            break;
        }
    }
}

// 递归检查是否连通（从(0,0)向右下走）
int VisitGrid(int visited[GRID_HEIGHT][GRID_WIDTH], int x, int y, int fromDir) {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return 0;
    if (visited[y][x]) return 0;
    int i;
    for (i = 0; i < MAX_BLOCKS; i++) {
        if (g_Blocks[i].gridX == x && g_Blocks[i].gridY == y) break;
    }
    if (i == MAX_BLOCKS) return 0;

    visited[y][x] = 1;

    int type = g_Blocks[i].type;

    // 检查四个方向
    static int dx[4] = {-1, 0, 1, 0};
    static int dy[4] = {0, -1, 0, 1};
    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        int opp = (d + 2) % 4;

        if (HasConnection(type, d)) {
            // 查找目标块
            for (int j = 0; j < MAX_BLOCKS; j++) {
                if (g_Blocks[j].gridX == nx && g_Blocks[j].gridY == ny) {
                    if (HasConnection(g_Blocks[j].type, opp)) {
                        VisitGrid(visited, nx, ny, d);
                    }
                    break;
                }
            }
        }
    }

    return 1;
}

// 检查是否全部连通
int CheckCircuitConnected() {
    int visited[GRID_HEIGHT][GRID_WIDTH] = {0};
    VisitGrid(visited, 0, 0, -1); // 从(0,0)出发

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            int found = 0;
            for (int i = 0; i < MAX_BLOCKS; i++) {
                if (g_Blocks[i].gridX == x && g_Blocks[i].gridY == y) {
                    found = 1;
                    if (!visited[y][x]) return 0;
                    break;
                }
            }
            if (!found) return 0;
        }
    }
    return 1; // 所有格子都访问过，说明连通
}