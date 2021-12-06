#define _CET_SECURE_NO_WARNINGS
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_HEIGHT 20  //���߽� 
#define MAP_WIDTH 18   //���߽� 
#define MAP_MINE_NUM 40  //���׵ĸ���
#define SIZE 25   //�����С

#define GRAPH_HEIGHT 500  //�����ĸ�
#define GRAPH_WIDRH  500  //�����Ŀ�

int over_flag;  //��Ϸ�Ľ��

struct Map {
	int x;
	int y;
	int num;  //��Χ�׵�����
	int mine; // 1Ϊ��  0������
	int state; //0Ϊδ���  1Ϊ�������ʾ������ 
}map[MAP_HEIGHT][MAP_WIDTH];   //����һȦ���鲻��  ��Ϊ�߽�

int dir[8][2] = { {-1,-1},{ 0,-1}, {1,-1}, {-1,0}, {1,0,},{-1,1},{ 0,1}, {1,1} };   //�������Χ8������

void buildMap(); //���ɵ�ͼ
void drawMap();  //����ͼ
void zoreBlock();  //Ϊ0�ķ���  ��Χ����ʾ����
void mouseClick(); //�����
void gameOver(); //��Ϸ����


int main() {
	srand((int)time(0));  //�������

	buildMap(); //���ɵ�ͼ

	drawMap();  //����ͼ

	mouseClick(); //�����

	return 0;
}

//���ɵ�ͼ
void buildMap() {
	//��ʼ������
	for (int i = 0; i < MAP_HEIGHT ; i++) { 
		for (int j = 0; j < MAP_WIDTH ; j++) {
			map[i][j].mine = 0;  //Ĭ������
			map[i][j].state = 0; //Ĭ��û�е��
			map[i][j].num = 0;   //Ĭ����Χû����
			map[i][j].x = i;    
			map[i][j].y = j;
		}
	}

	//�������MAP_MINE_NUM����
	int n = MAP_MINE_NUM;
	while (n--) {   
		int tx = rand() % (MAP_HEIGHT - 2) + 1;
		int ty = rand() % (MAP_WIDTH - 2) + 1;
		if (map[tx][ty].mine != 1) {  //�ж��������ĵط��ǲ�����   �����ٸ�ֵΪ��
			map[tx][ty].mine = 1;
		}
		else {  //�����������ѭ��
			n++;
		}
	}

	//��������׷�����Χ�׵ĸ��� 
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {   //��1��ʼ ����һȦ���鲻��  ��Ϊ�߽�
		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			for (int k = 0; k < 8; k++) {   //�������Χ8������
				int tx = map[i][j].x + dir[k][0];
				int ty = map[i][j].y + dir[k][1];
				map[i][j].num = map[i][j].num + map[tx][ty].mine;  //ͨ������Ϊ1 ������Լ�����Χ�׵ĸ���
			}
		}
	}
}

//����ͼ
void drawMap() {
	//��ʼ������
	initgraph(GRAPH_WIDRH, GRAPH_HEIGHT);

	//������ͼ
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {  //��1��ʼ ����һȦ���鲻��  ��Ϊ�߽�
		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			setcolor(WHITE); //�������ɫ
			setfillcolor(RGB(128, 128, 128));  //���������ɫ
			fillrectangle(i * SIZE, j * SIZE, i * SIZE + SIZE, j * SIZE + SIZE);
		}
	}
}

//Ϊ0�ķ���  ��Χ����ʾ����
void zoreBlock() {
	int t_flag;
	//ѭ���ѵ������Χ
	while (1) {
		t_flag = 0;
		for (int i = 1; i < MAP_HEIGHT - 1; i++) {   //��1��ʼ ����һȦ���鲻��  ��Ϊ�߽�
			for (int j = 1; j < MAP_WIDTH - 1; j++) {
				if (map[i][j].state == 1 && map[i][j].num == 0) {  //������� �� ��Χ����Ϊ0��
					map[i][j].num = -1; //��ΧΪ0���׵� numֵ��Ϊ-1
					t_flag = 1;
					for (int k = 0; k < 8; k++) {   //�������Χ8������
						int tx = map[i][j].x + dir[k][0];
						int ty = map[i][j].y + dir[k][1];
						if (tx >= 1 && tx <= MAP_HEIGHT - 2 && ty >= 1 && ty <= MAP_WIDTH - 2) {
							map[tx][ty].state = 1; //״̬��Ϊ�����
							//��ʾ����
							WCHAR s[5];
							wsprintf(s, L"%d", map[tx][ty].num);
							setbkmode(TRANSPARENT);
							outtextxy(tx * SIZE + 5, ty * SIZE + 5, s);
						}
						
					}
				}
			}
		}
		//��numΪ-1�ķ�������ָ��ǵ�
		for (int i = 1; i < MAP_HEIGHT - 1; i++) {   //��1��ʼ ����һȦ���鲻��  ��Ϊ�߽�
			for (int j = 1; j < MAP_WIDTH - 1; j++) {
				if (map[i][j].num == -1 && map[i][j].state == 1) {
					setfillcolor(RGB(193,210,240));  //�ò����ķ��������ɫ
					fillrectangle(i * SIZE, j * SIZE, i * SIZE + SIZE, j * SIZE + SIZE);
				}
			}
		}
		if (t_flag == 0) {  //һ�������������Χ����Ϊ0���ķ��鶼û��
			return;
		}
	}



}

//�����
void mouseClick() {
	MOUSEMSG m;
	while (true) {
		m = GetMouseMsg(); //��ȡ�������
 		int tx = m.x / SIZE;
		int ty = m.y / SIZE;
		if (tx > 0 && tx <= MAP_WIDTH  && ty > 0 && ty <= MAP_HEIGHT ) {
			if (m.uMsg == WM_LBUTTONDOWN) {
				//״̬��Ϊ�ѵ��
				map[tx][ty].state = 1;  
				//��ʾ����
				WCHAR s[5];
				wsprintf(s, L"%d", map[tx][ty].num);
				setbkmode(TRANSPARENT);
				outtextxy(tx * SIZE + 5, ty * SIZE + 5, s);			
				
				//�ж��Ƿ�㵽��
				if (map[tx][ty].mine == 1) {
					setfillcolor(RED);  //���������ɫ
					fillrectangle(tx * SIZE, ty * SIZE, tx * SIZE + SIZE, ty * SIZE + SIZE);
					over_flag = 0;
					gameOver();
				}
				//�����������������Χû����
				if (map[tx][ty].num == 0) {
					zoreBlock();
				}

				//�ж���Ϸ�Ƿ�ʤ��  δ�������Ϊ������ʤ��
				int cout = 0;
				for (int i = 1; i < MAP_HEIGHT - 1; i++) {   //��1��ʼ ����һȦ���鲻��  ��Ϊ�߽�
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

//��Ϸ����
void gameOver() {
	if (over_flag == 0) {
		settextcolor(RED);
		setbkmode(TRANSPARENT);
		settextstyle(50, 0, L"���Ŀ���");
		outtextxy(160,140,L"��Ϸʧ�ܣ�");
		settextstyle(40, 0, L"���Ŀ���");
		outtextxy(120, 200, L"����������¿�ʼ");
	}
	else {
		settextcolor(RED);
		setbkmode(TRANSPARENT);
		settextstyle(50, 0, L"���Ŀ���");
		outtextxy(160, 140, L"��Ϸʤ����");
		settextstyle(40, 0, L"���Ŀ���");
		outtextxy(120, 200, L"����������¿�ʼ");
	}
	_getch();
	main(); //���¿�ʼ
}


