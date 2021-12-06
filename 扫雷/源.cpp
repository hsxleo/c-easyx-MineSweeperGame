#define _CET_SECURE_NO_WARNINGS
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_HEIGHT 20  //含边界 
#define MAP_WIDTH 18   //含边界 
#define MAP_MINE_NUM 40  //地雷的个数
#define SIZE 25   //方块大小

#define GRAPH_HEIGHT 500  //画布的高
#define GRAPH_WIDRH  500  //画布的宽

int over_flag;  //游戏的结果

struct Map {
	int x;
	int y;
	int num;  //周围雷的数量
	int mine; // 1为雷  0不是雷
	int state; //0为未点击  1为点击完显示出来的 
}map[MAP_HEIGHT][MAP_WIDTH];   //外面一圈数组不用  作为边界

int dir[8][2] = { {-1,-1},{ 0,-1}, {1,-1}, {-1,0}, {1,0,},{-1,1},{ 0,1}, {1,1} };   //方块的周围8个方向

void buildMap(); //生成地图
void drawMap();  //画地图
void zoreBlock();  //为0的方块  周围的显示出来
void mouseClick(); //鼠标点击
void gameOver(); //游戏结束


int main() {
	srand((int)time(0));  //随机种子

	buildMap(); //生成地图

	drawMap();  //画地图

	mouseClick(); //鼠标点击

	return 0;
}

//生成地图
void buildMap() {
	//初始化方块
	for (int i = 0; i < MAP_HEIGHT ; i++) { 
		for (int j = 0; j < MAP_WIDTH ; j++) {
			map[i][j].mine = 0;  //默认无雷
			map[i][j].state = 0; //默认没有点击
			map[i][j].num = 0;   //默认周围没有雷
			map[i][j].x = i;    
			map[i][j].y = j;
		}
	}

	//随机生成MAP_MINE_NUM个雷
	int n = MAP_MINE_NUM;
	while (n--) {   
		int tx = rand() % (MAP_HEIGHT - 2) + 1;
		int ty = rand() % (MAP_WIDTH - 2) + 1;
		if (map[tx][ty].mine != 1) {  //判断这个随机的地方是不是雷   不是再赋值为雷
			map[tx][ty].mine = 1;
		}
		else {  //如果是雷重新循环
			n++;
		}
	}

	//计算出非雷方块周围雷的个数 
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {   //从1开始 外面一圈方块不用  作为边界
		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			for (int k = 0; k < 8; k++) {   //方块的周围8个方向
				int tx = map[i][j].x + dir[k][0];
				int ty = map[i][j].y + dir[k][1];
				map[i][j].num = map[i][j].num + map[tx][ty].mine;  //通过有雷为1 这个属性计算周围雷的个数
			}
		}
	}
}

//画地图
void drawMap() {
	//初始化画布
	initgraph(GRAPH_WIDRH, GRAPH_HEIGHT);

	//画出地图
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {  //从1开始 外面一圈方块不用  作为边界
		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			setcolor(WHITE); //方块框颜色
			setfillcolor(RGB(128, 128, 128));  //方块填充颜色
			fillrectangle(i * SIZE, j * SIZE, i * SIZE + SIZE, j * SIZE + SIZE);
		}
	}
}

//为0的方块  周围的显示出来
void zoreBlock() {
	int t_flag;
	//循环把点击过周围
	while (1) {
		t_flag = 0;
		for (int i = 1; i < MAP_HEIGHT - 1; i++) {   //从1开始 外面一圈方块不用  作为边界
			for (int j = 1; j < MAP_WIDTH - 1; j++) {
				if (map[i][j].state == 1 && map[i][j].num == 0) {  //点击过的 且 周围雷数为0的
					map[i][j].num = -1; //周围为0个雷的 num值变为-1
					t_flag = 1;
					for (int k = 0; k < 8; k++) {   //方块的周围8个方向
						int tx = map[i][j].x + dir[k][0];
						int ty = map[i][j].y + dir[k][1];
						if (tx >= 1 && tx <= MAP_HEIGHT - 2 && ty >= 1 && ty <= MAP_WIDTH - 2) {
							map[tx][ty].state = 1; //状态改为点击过
							//显示数字
							WCHAR s[5];
							wsprintf(s, L"%d", map[tx][ty].num);
							setbkmode(TRANSPARENT);
							outtextxy(tx * SIZE + 5, ty * SIZE + 5, s);
						}
						
					}
				}
			}
		}
		//把num为-1的方块的数字覆盖掉
		for (int i = 1; i < MAP_HEIGHT - 1; i++) {   //从1开始 外面一圈方块不用  作为边界
			for (int j = 1; j < MAP_WIDTH - 1; j++) {
				if (map[i][j].num == -1 && map[i][j].state == 1) {
					setfillcolor(RGB(193,210,240));  //用不到的方块填充颜色
					fillrectangle(i * SIZE, j * SIZE, i * SIZE + SIZE, j * SIZE + SIZE);
				}
			}
		}
		if (t_flag == 0) {  //一个（点击过且周围雷数为0）的方块都没有
			return;
		}
	}



}

//鼠标点击
void mouseClick() {
	MOUSEMSG m;
	while (true) {
		m = GetMouseMsg(); //获取鼠标坐标
 		int tx = m.x / SIZE;
		int ty = m.y / SIZE;
		if (tx > 0 && tx <= MAP_WIDTH  && ty > 0 && ty <= MAP_HEIGHT ) {
			if (m.uMsg == WM_LBUTTONDOWN) {
				//状态变为已点击
				map[tx][ty].state = 1;  
				//显示数字
				WCHAR s[5];
				wsprintf(s, L"%d", map[tx][ty].num);
				setbkmode(TRANSPARENT);
				outtextxy(tx * SIZE + 5, ty * SIZE + 5, s);			
				
				//判断是否点到雷
				if (map[tx][ty].mine == 1) {
					setfillcolor(RED);  //方块填充颜色
					fillrectangle(tx * SIZE, ty * SIZE, tx * SIZE + SIZE, ty * SIZE + SIZE);
					over_flag = 0;
					gameOver();
				}
				//如果点击个这个方块周围没有雷
				if (map[tx][ty].num == 0) {
					zoreBlock();
				}

				//判断游戏是否胜利  未被点击数为雷数则胜利
				int cout = 0;
				for (int i = 1; i < MAP_HEIGHT - 1; i++) {   //从1开始 外面一圈方块不用  作为边界
					for (int j = 1; j < MAP_WIDTH - 1; j++) {
						if (map[i][j].state == 0) {
							cout++;
						}
					}
				}
				if (cout == MAP_MINE_NUM) {
					over_flag = 1;
					gameOver();
				}
				
			}
		}
	}
}

//游戏结束
void gameOver() {
	if (over_flag == 0) {
		settextcolor(RED);
		setbkmode(TRANSPARENT);
		settextstyle(50, 0, L"华文楷体");
		outtextxy(160,140,L"游戏失败！");
		settextstyle(40, 0, L"华文楷体");
		outtextxy(120, 200, L"按任意键重新开始");
	}
	else {
		settextcolor(RED);
		setbkmode(TRANSPARENT);
		settextstyle(50, 0, L"华文楷体");
		outtextxy(160, 140, L"游戏胜利！");
		settextstyle(40, 0, L"华文楷体");
		outtextxy(120, 200, L"按任意键重新开始");
	}
	_getch();
	main(); //重新开始
}


