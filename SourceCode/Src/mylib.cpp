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

	// �������� battery_template ������ģ��
	dCloneSprite("battery_template", name);
	dSetSpritePosition(name, x, y);
	dSetSpriteCollisionActive(name, 1, 1);  // �������ͺͽ�����ײ

	g_BatteryCount++;
}

void ResetBatteries() {
	g_BatteryCount = 0;
}

// ����Ƿ��ǵ�ؾ��飬��������ײ�߼�
void HandleBatteryCollision(const char* spriteA, const char* spriteB) {
	const char* szBattery = strstr(spriteA, "dianchi") ? spriteA :
		(strstr(spriteB, "dianchi") ? spriteB : NULL);
	if (szBattery == NULL) return;

	PlaySound("game/data/audio/dianchi.wav",0,1);
	for (int i = 0; i < g_BatteryCount; i++) {
		if (strcmp(g_Batteries[i].name, szBattery) == 0 && g_Batteries[i].collected == 0) {
			g_Batteries[i].collected = 1;
			dDeleteSprite(g_Batteries[i].name);
			// ��ԭ�ȵı���
			extern int BatteryNumber;
			BatteryNumber++;
			break;
		}
	}
}


CircuitBlock g_Blocks[MAX_BLOCKS];
int g_SelectedIndex = -1;

// ����bitmask���壺1=�� 2=�� 4=�� 8=��
int g_BlockDirections[][4] = {
	{1, 0, 1, 0}, // 0 ��  ����
	{0, 1, 0, 1}, // 1 ��  ����
	{0, 0, 1, 1}, // 2 ��  ����
	{1, 0, 0, 1}, // 3 ��  ����
	{0, 1, 1, 0}, // 4 ��  ����
	{1, 1, 0, 0}  // 5 ��  ����
};

// �ж�һ������ĳ�������Ƿ������ӿ�
int HasConnection(int type, int dir) {
	return g_BlockDirections[type][dir];
}

// ��ʼ��ƴͼ��������ͺ�λ�ã�
void InitCircuitPuzzle() {
	int types[MAX_BLOCKS] = {0,1,2,3,4,5,0,3,1}; // �趨��˳��

	for (int i = 0; i < MAX_BLOCKS; i++) {
		sprintf(g_Blocks[i].name, "block%d", i);
		g_Blocks[i].gridX = i % GRID_WIDTH;
		g_Blocks[i].gridY = i / GRID_WIDTH;
		g_Blocks[i].type = types[i];

		// ��������ѡ��ģ����
		char templateName[32];
		sprintf(templateName, "block%d_template", g_Blocks[i].type);

		// ��¡ģ�徫��
		dCloneSprite(templateName, g_Blocks[i].name);
		dSetSpritePosition(g_Blocks[i].name,
			GRID_ORIGIN_X + g_Blocks[i].gridX * BLOCK_SIZE,
			GRID_ORIGIN_Y + g_Blocks[i].gridY * BLOCK_SIZE);
		dSetSpriteCollisionActive(g_Blocks[i].name, 0, 0);
	}

	g_SelectedIndex = -1;
}

// �����������߼�
void HandleCircuitClick(float x, float y) {
	for (int i = 0; i < MAX_BLOCKS; i++) {
		if (dIsPointInSprite(g_Blocks[i].name, x, y)) {
			// �ɹ��㵽һ��ƴͼ�� 
			PlaySound("game/data/audio/select.wav",0,1);
			if (g_SelectedIndex == -1) {
				// ��¼��һ�ε��
				g_SelectedIndex = i;
			} else {
				// �Ѿ�ѡ��һ�����ٵ����һ�������Խ���
				int dx = abs(g_Blocks[i].gridX - g_Blocks[g_SelectedIndex].gridX);
				int dy = abs(g_Blocks[i].gridY - g_Blocks[g_SelectedIndex].gridY);

				if ((dx + dy) == 1) { // ���ڸ��Ӳ��ܽ���
					PlaySound("game/data/audio/swap.wav",0,1);
					// ���� grid ����
					int tempX = g_Blocks[i].gridX;
					int tempY = g_Blocks[i].gridY;

					g_Blocks[i].gridX = g_Blocks[g_SelectedIndex].gridX;
					g_Blocks[i].gridY = g_Blocks[g_SelectedIndex].gridY;

					g_Blocks[g_SelectedIndex].gridX = tempX;
					g_Blocks[g_SelectedIndex].gridY = tempY;

					// ���¾���ʵ��λ��
					dSetSpritePosition(g_Blocks[i].name,
						GRID_ORIGIN_X + g_Blocks[i].gridX * BLOCK_SIZE,
						GRID_ORIGIN_Y + g_Blocks[i].gridY * BLOCK_SIZE);

					dSetSpritePosition(g_Blocks[g_SelectedIndex].name,
						GRID_ORIGIN_X + g_Blocks[g_SelectedIndex].gridX * BLOCK_SIZE,
						GRID_ORIGIN_Y + g_Blocks[g_SelectedIndex].gridY * BLOCK_SIZE);
				}
				g_SelectedIndex = -1;
			}

			break; // ����ɹ�����˳�ѭ��
		}
	}
}

// �ݹ����Ƿ���ͨ����(0,0)�������ߣ�
/*int VisitGrid(int visited[H][W], int x, int y, int fromDir) {
if �����߽���ѷ��ʹ� �� return;
�ҵ���ǰλ�õĵ�·�� index;
��� visited[y][x] = 1;

for �ĸ�����
�����ǰ���ڸ÷��������ӣ�
�����ھ�λ���Ƿ��п��Ҹ÷������
������� �� �ݹ�����ھ�
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

	// ����ĸ�����
	static int dx[4] = {-1, 0, 1, 0};
	static int dy[4] = {0, -1, 0, 1};
	for (int d = 0; d < 4; d++) {
		int nx = x + dx[d];
		int ny = y + dy[d];
		int opp = (d + 2) % 4;

		if (HasConnection(type, d)) {
			// ����Ŀ���
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

// ����Ƿ�ȫ����ͨ
/*��ʼ�� visited[H][W] ȫΪ 0;
�����Ͻ� (0, 0) ��ʼ���� VisitGrid(...);

Ȼ��������� block��
���ĳ����û���� visited ���г��� �� ���� 0��δ��ͨ��

���� 1����ͨ�ɹ���*/
int CheckCircuitConnected() {
	int visited[GRID_HEIGHT][GRID_WIDTH] = {0};
	VisitGrid(visited, 0, 0, -1); // ��(0,0)����

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
	return 1; // ���и��Ӷ����ʹ���˵����ͨ
}