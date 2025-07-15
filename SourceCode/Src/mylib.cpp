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

	PlaySound("game/data/audio/dianchi.wav",0,1);
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
	int types[MAX_BLOCKS] = {0,1,2,3,4,5,0,3,1}; // 设定的顺序

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
			GRID_ORIGIN_X + g_Blocks[i].gridX * BLOCK_SIZE,
			GRID_ORIGIN_Y + g_Blocks[i].gridY * BLOCK_SIZE);
		dSetSpriteCollisionActive(g_Blocks[i].name, 0, 0);
	}

	g_SelectedIndex = -1;
}

// 处理点击交换逻辑
void HandleCircuitClick(float x, float y) {
	for (int i = 0; i < MAX_BLOCKS; i++) {
		if (dIsPointInSprite(g_Blocks[i].name, x, y)) {
			// 成功点到一个拼图块 
			PlaySound("game/data/audio/select.wav",0,1);
			if (g_SelectedIndex == -1) {
				// 记录第一次点击
				g_SelectedIndex = i;
			} else {
				// 已经选中一个，再点击另一个：尝试交换
				int dx = abs(g_Blocks[i].gridX - g_Blocks[g_SelectedIndex].gridX);
				int dy = abs(g_Blocks[i].gridY - g_Blocks[g_SelectedIndex].gridY);

				if ((dx + dy) == 1) { // 相邻格子才能交换
					PlaySound("game/data/audio/swap.wav",0,1);
					// 交换 grid 坐标
					int tempX = g_Blocks[i].gridX;
					int tempY = g_Blocks[i].gridY;

					g_Blocks[i].gridX = g_Blocks[g_SelectedIndex].gridX;
					g_Blocks[i].gridY = g_Blocks[g_SelectedIndex].gridY;

					g_Blocks[g_SelectedIndex].gridX = tempX;
					g_Blocks[g_SelectedIndex].gridY = tempY;

					// 更新精灵实际位置
					dSetSpritePosition(g_Blocks[i].name,
						GRID_ORIGIN_X + g_Blocks[i].gridX * BLOCK_SIZE,
						GRID_ORIGIN_Y + g_Blocks[i].gridY * BLOCK_SIZE);

					dSetSpritePosition(g_Blocks[g_SelectedIndex].name,
						GRID_ORIGIN_X + g_Blocks[g_SelectedIndex].gridX * BLOCK_SIZE,
						GRID_ORIGIN_Y + g_Blocks[g_SelectedIndex].gridY * BLOCK_SIZE);
				}
				g_SelectedIndex = -1;
			}

			break; // 点击成功后就退出循环
		}
	}
}

// 递归检查是否连通（从(0,0)向右下走）
/*int VisitGrid(int visited[H][W], int x, int y, int fromDir) {
if 超出边界或已访问过 → return;
找到当前位置的电路块 index;
标记 visited[y][x] = 1;

for 四个方向：
如果当前块在该方向有连接：
查找邻居位置是否有块且该方向对上
如果对上 → 递归访问邻居
}*/
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
/*初始化 visited[H][W] 全为 0;
从左上角 (0, 0) 开始调用 VisitGrid(...);

然后遍历所有 block：
如果某个块没有在 visited 表中出现 → 返回 0（未连通）

返回 1（连通成功）*/
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
	PlaySound("game/data/audio/connect.wav",0,1);
	return 1; // 所有格子都访问过，说明连通
}