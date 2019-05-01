#include<iostream>
#include<cstdio>
#include<windows.h>
#include<string>
#include<stdlib.h>
#include <time.h>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#define random(x) (rand()%x)

#pragma GCC optimize(3)

using namespace std;

string refresh_left(int left);
string refresh_top(int top);
int get_width(int d_type, int d_dir);
int get_height(int d_type, int d_dir);
int get_left(int d_type, int d_dir);
int one_player();
void paint(int d_type, int d_dir, int d_x, int d_y);
void clearScreen();
void quickCls();

int selects = 0;
string left_str, top_str;
int half_height, half_width_1, half_width_2;
int play_width = 10, play_height = 20;
const int TICK = 5, DELAY = 60, SPEED = 500; //��λms����ֵԽСʱ��Խ�̣��ٶ�Խ�죩
const bool COUNT_DOWN = true; //��ʼǰ����ʱ Ĭ��true��������
//������Ϣ���
const int BLOCK_DATA[7][4][4][2] = {
	{ // 1 ����
		{{0,1},{1,1},{2,1},{3,1}},
		{{2,0},{2,1},{2,2},{2,3}},
		{{0,2},{1,2},{2,2},{3,2}},
		{{1,0},{1,1},{1,2},{1,3}}
	},
	{ // 2 ������
		{{0,0},{1,0},{0,1},{1,1}},
		{{0,0},{1,0},{0,1},{1,1}},
		{{0,0},{1,0},{0,1},{1,1}},
		{{0,0},{1,0},{0,1},{1,1}}
	},
	{ // 3 ��L��
		{{0,0},{0,1},{1,1},{2,1}},
		{{1,0},{1,1},{1,2},{2,0}},
		{{0,1},{1,1},{2,1},{2,2}},
		{{1,0},{1,1},{1,2},{0,0}}
	},
	{ // 4 L��
		{{2,0},{0,1},{1,1},{2,1}},
		{{1,0},{1,1},{1,2},{2,2}},
		{{0,1},{1,1},{2,1},{0,2}},
		{{0,0},{1,0},{1,1},{1,2}}
	},
	{ // 5 T��
		{{1,0},{0,1},{1,1},{2,1}},
		{{1,0},{1,1},{1,2},{2,1}},
		{{0,1},{1,1},{2,1},{1,2}},
		{{0,1},{1,0},{1,1},{1,2}}
	},
	{ // 6 Z��
		{{0,0},{1,0},{1,1},{2,1}},
		{{1,1},{1,2},{2,0},{2,1}},
		{{0,1},{1,1},{1,2},{2,2}},
		{{0,1},{0,2},{1,0},{1,1}}
	},
	{ // 7 ��Z��
		{{1,0},{2,0},{0,1},{1,1}},
		{{1,0},{1,1},{2,1},{2,2}},
		{{1,1},{2,1},{0,2},{1,2}},
		{{0,0},{0,1},{1,1},{1,2}}
	},
};

int main() {
	int left = 10, top = 3;
	system("mode con cols=100 lines=32");
	//lines=32
	system("color F");
	system("chcp 936");
	//UTF-8: 65001
	//GBK: 936[d_x + BLOCK_DATA[d_type - 1][d_dir][0]]

	//���ÿ��ٱ༭ģʽ
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE; //�Ƴ����ٱ༭ģʽ
	mode &= ~ENABLE_INSERT_MODE; //�Ƴ�����ģʽ
	mode &= ~ENABLE_MOUSE_INPUT;
	mode &= ~ENABLE_WINDOW_INPUT;
	mode &= ~ENABLE_PROCESSED_INPUT;
	//mode &= ~ENABLE_LINE_INPUT;
	SetConsoleMode(hStdin, mode);

	left_str = refresh_left(left);
	top_str = refresh_top(top);
	while (true) {
		clearScreen();
		cout << top_str;
		if (selects == 0) {
			cout << left_str << "������������\n"
			<< left_str << "������ģʽ��\n"
			<< left_str << "������������\n";
		}else {
			cout << "\n" << left_str << "  ����ģʽ\n\n";
		}
		cout << "\n";
		if (selects == 1) {
			cout << left_str << "������������\n"
			<< left_str << "��˫�˶�ս��  �ù�����δʵ��\n"
			<< left_str << "������������\n";
		}else {
			cout << "\n" << left_str << "  ˫�˶�ս\n\n";
		}
		cout << "\n";
		if (selects == 2) {
			cout << left_str << "������������\n"
			<< left_str << "��  ����  ��  �ù�����δʵ��\n"
			<< left_str << "������������\n";
		}else {
			cout << "\n" << left_str << "    ����\n\n";
		}
		cout << "\n";
		if (selects == 3) {
			cout << left_str << "������������\n"
			<< left_str << "��  �˳�  ��\n"
			<< left_str << "������������\n";
		}else {
			cout << "\n" << left_str << "    �˳�\n\n";
		}
		cout << "\n";
		Sleep(100);
		while (true) {
			Sleep(20);
			//Up
			if (KEY_DOWN(VK_UP) || KEY_DOWN('W')) {
				if (selects == 0) selects = 3; else selects--;
				break;
			}
			//Down
			if (KEY_DOWN(VK_DOWN) || KEY_DOWN('S')) {
				if (selects == 3) selects = 0; else selects++;
				break;
			}
			//Enter
			if (KEY_DOWN(VK_RETURN)) {
				switch (selects) {
					case 0:
						one_player();
					break;
					case 1:
						//TODO
					break;
					case 2:
						//TODO
					left_str = refresh_left(left);
					top_str = refresh_top(top);
					break;
					case 3:
					return 0;
				}
			}
		}
	}
}

string refresh_left(int left) {
	string str = "";
	for (int i = 0; i < left; i++) str += " ";
		return str;
}

string refresh_top(int top) {
	string str = "";
	for (int i = 0; i < top; i++) str += "\n";
		return str;
}

int one_player() {
	srand((unsigned int)(time(NULL)));
	bool refresh = false, down = false, hard_down = false, apply = false, paint_back = false;
	int tmp_dir, tmp_x, tmp_y;
	int tick_count = 0;
	int d_type = 0, d_dir = 0, d_x, d_y;
	bool scr[play_width][play_height];
	bool block[play_width][play_height];
	int last_type = 0;
	bool pause = false;
	int score = 0;
	bool force_refresh = false;
	
	for(int i = 0; i < play_width; i++) {
		for(int j = 0; j < play_height; j++) {
			scr[i][j] = false;
			block[i][j] = false;
		}
	}
	
	half_height = (play_height - 3) / 2;
	half_width_1 = (play_width - 7) / 2;
	half_width_2 = (play_width - 1) / 2;
	
	//Count down
	clearScreen();
	if (COUNT_DOWN) {
		for (int count = 3; count > 0; count--) {
		quickCls();
		printf("%s%sScore: %d\n%s��", top_str.c_str(), left_str.c_str(), score, left_str.c_str());
		for (int i = 0; i < play_width; i++) printf("��");
		printf("��\n");
		for (int i = 0; i < half_height; i++) {
			printf("%s��", left_str.c_str());
			for (int j = 0; j < play_width; j++) printf("  ");
			printf("��\n");
		}
		printf("%s��", left_str.c_str());
		for (int i = 0; i < half_width_1; i++) printf("  ");
		printf(" Game start in");
		for (int i = 0; i < play_width - half_width_1 - 7; i++) printf("  ");
		printf("��\n%s��", left_str.c_str());
		for (int i = 0; i < play_width; i++) printf("  ");
		printf("��\n%s��", left_str.c_str());
		for (int i = 0; i < half_width_2; i++) printf("  ");
		printf(" %d", count);
		for (int i = 0; i < play_width - half_width_2 - 1; i++) printf("  ");
		printf("��\n");
		for (int i = 0; i < play_height - half_height - 3; i++) {
			printf("%s��", left_str.c_str());
			for (int j = 0; j < play_width; j++) printf("  ");
			printf("��\n");
		}
		printf("%s��", left_str.c_str());
		for (int i = 0; i < play_width; i++) printf("��");
		printf("��\n");
		Sleep(1000);
	}
	}
	
	//Game Start Refresh
	while (true) {
		
		//new block
		if (d_type == 0) {
			for (int i = 0; i < play_width; i++) {
				for (int j = 0; j < play_height; j++) {
					block[i][j] = scr[i][j];
				}
			}
			if (last_type != 0) {
				//���м�� Game over check
				bool game_over = false;
				for (int x = 0; x < play_width; x++) {
					if (block[x][0] || block[x][1]) {
						game_over = true;
						break;
					}
				}
				if (game_over) {
					goto GameOver;
				}

				//�����м�� Clear check
				//�ⲿ�ִ�������Ż�����Ϊֻɨ��䶯���У�������ʱ�䲻����ɨ��ȫ�����
				int clear[4]; //����Clear��������
				//for (int i = 0; i < play_height; i++)
				//	clear[i] = 0;
				int clear_num = 0;
				for (/*int y = d_y; y <= check_max_y; y++*/int y = 0; y < play_height; y++) {
					int i;
					for (i = 1; i < play_width; i++) {
						if (!block[i][y]) break;
					}
					if (i == play_width) { //clear
						clear[clear_num++] = y;
						//printf("\nclear y=%d\n", y);
						//printf("i=%d y=%d clear_num=%d\n", i, y, clear_num);		
					}
				}
				if (clear_num != 0) {
					for (int i = 0; i < clear_num; i++) { //ÿ��ƽ��ѭ��
						for (int y = clear[i]; y > 0; y--) { //ÿ��ƽ��ɨ��
							for (int x = 0; x < play_width; x++) { //ÿ��ƽ��ɨ��
								block[x][y] = block[x][y - 1];
							}
						}
					}

					//������� Score
					for (int i = 1; i <= clear_num; i++) score += i;

					//Disable
					for (int i = 0; i < play_width; i++) {
						for (int j = 0; j < play_height; j++) {
							scr[i][j] = block[i][j];
						}
					}
					quickCls();
					printf("%s%sScore: %d\n%s��", top_str.c_str(), left_str.c_str(), score, left_str.c_str());
					for (int i = 0; i < play_width; i++) printf("��");
					printf("��\n");
					for (int i = 0; i < play_height; i++) {
						printf("%s��", left_str.c_str());
						for (int j = 0; j < play_width; j++) printf(scr[j][i] ? "��" : "  ");
						printf("��\n");
					}
					printf("%s��", left_str.c_str());
					for (int i = 0; i < play_width; i++) printf("��");
					printf("��\n");
				}

			}

			d_type = random(7) + 1;
			d_dir = 0;
			d_x = half_width_2;
			d_y = 0;
			tmp_x = d_x;
			tmp_y = d_y;
			tmp_dir = d_dir;
		}else {
			for (int i = 0; i < play_width; i++) {
				for (int j = 0; j < play_height; j++) {
					scr[i][j] = block[i][j];
				}
			}
		}

		//Paint
		Paint:
		for (int i = 0; i < 4; i++)
			scr[d_x + BLOCK_DATA[d_type - 1][d_dir][i][0]][d_y + BLOCK_DATA[d_type - 1][d_dir][i][1]] = true;
		if (paint_back) goto PaintBack;

		//Disable
		quickCls();
		printf("%s%sScore: %d\n%s��", top_str.c_str(), left_str.c_str(), score, left_str.c_str());
		for (int i = 0; i < play_width; i++) printf("��");
			printf("��\n");
		for (int i = 0; i < play_height; i++) {
			printf("%s��", left_str.c_str());
			for (int j = 0; j < play_width; j++) printf(scr[j][i] ? "��" : "  ");
				printf("��\n");
		}
		printf("%s��", left_str.c_str());
		for (int i = 0; i < play_width; i++) printf("��");
			printf("��\n");

		//debug
		//printf("\nBlock width: %d\nBlock height: %d\nBlock left: %d\nBlock type: %d\nBlock direction: %d", get_width(d_type, d_dir), get_height(d_type, d_dir), get_left(d_type, d_dir), d_type, d_dir);

		while (true) {
			Sleep(TICK);
			if (pause) {
				pause = false;
				tick_count += DELAY;
				Sleep(DELAY);
			}
			tick_count += TICK;

			tmp_dir = d_dir;
			tmp_x = d_x;
			tmp_y = d_y;

			//��ת
			if (KEY_DOWN(VK_RIGHT)) {
				if (tmp_dir == 3) tmp_dir = 0; else tmp_dir += 1;
				if ((tmp_x + get_width(d_type, d_dir) == play_width) && (get_width(d_type, tmp_dir) > get_width(d_type, d_dir))) tmp_x -= get_width(d_type, tmp_dir) - get_width(d_type, d_dir);
				if ((tmp_x + get_left(d_type, d_dir) == 0) && (get_left(d_type, tmp_dir) < get_left(d_type, d_dir))) tmp_x += get_left(d_type, d_dir) - get_left(d_type, tmp_dir);
				refresh = true;
				pause = true;
			}
			//��ת
			if (KEY_DOWN(VK_LEFT)) {
				if (tmp_dir == 0) tmp_dir = 3; else tmp_dir -= 1;
				if ((tmp_x + get_width(d_type, d_dir) == play_width) && (get_width(d_type, tmp_dir) > get_width(d_type, d_dir))) tmp_x -= get_width(d_type, tmp_dir) - get_width(d_type, d_dir);
				if ((tmp_x + get_left(d_type, d_dir) == 0) && (get_left(d_type, tmp_dir) < get_left(d_type, d_dir))) tmp_x += get_left(d_type, d_dir) - get_left(d_type, tmp_dir);
				refresh = true;
				pause = true;
			}
			//����
			if (KEY_DOWN('A')) {
				if (tmp_x + get_left(d_type, tmp_dir) > 0) {
					tmp_x -= 1;
					refresh = true;
				}
				pause = true;
			}
			//����
			if (KEY_DOWN('D')) {
				if (tmp_x + get_width(d_type, tmp_dir) < play_width) {
					tmp_x += 1;
					refresh = true;
				}
				pause = true;
			}
			if (KEY_DOWN(VK_DOWN)) {
				tmp_y += 1;
				down = true;
				hard_down = true;
				refresh = true;
				pause = true;
			}
			if (KEY_DOWN('S')) {
				tmp_y += 1;
				down = true;
				refresh = true;
				pause = true;
			}

			if (tick_count >= SPEED) {
				tick_count = 0;
				tmp_y += 1;
				down = true;
				refresh = true;
			}

			//ǿ��ˢ�� Force refresh
			if (KEY_DOWN(VK_F8)) {
				force_refresh = true;
			}

			//���任�Ƿ�Ϸ� 
			//Refresh check
			if (refresh) {
				Refresh_check:
				apply = true;
				if (block[tmp_x + BLOCK_DATA[d_type - 1][tmp_dir][0][0]][tmp_y + BLOCK_DATA[d_type - 1][tmp_dir][0][1]]
					|| block[tmp_x + BLOCK_DATA[d_type - 1][tmp_dir][1][0]][tmp_y + BLOCK_DATA[d_type - 1][tmp_dir][1][1]]
					|| block[tmp_x + BLOCK_DATA[d_type - 1][tmp_dir][2][0]][tmp_y + BLOCK_DATA[d_type - 1][tmp_dir][2][1]]
					|| block[tmp_x + BLOCK_DATA[d_type - 1][tmp_dir][3][0]][tmp_y + BLOCK_DATA[d_type - 1][tmp_dir][3][1]])
					apply = false;
				if (d_y + get_height(d_type, d_dir) >= play_height) apply = false;
				if (force_refresh) { //ǿ��ˢ��
					apply = false;
					force_refresh = false;
				}
				if (apply) {
					d_dir = tmp_dir;
					d_x = tmp_x;
					d_y = tmp_y;
					//Ӳ�� 
					if (hard_down) {
						tmp_y++;
						goto Refresh_check;
					}
				}else {
					if (down) {
						//Paint again
						for (int i = 0; i < play_width; i++) {
							for (int j = 0; j < play_height; j++) {
								scr[i][j] = block[i][j];
							}
						}
						paint_back = true;
						goto Paint;
						PaintBack:
						paint_back = false;
						if (hard_down) {
							for (int i = 0; i < play_width; i++) {
								for (int j = 0; j < play_height; j++) {
									block[i][j] = scr[i][j];
								}
							}
						}
						if ((d_y + 1 == tmp_y) && (d_x == tmp_x)) {
							last_type = d_type;
							d_type = 0;
						}
						down = false;
						hard_down = false;
					}
				}
				refresh = false;
				break;
			}
		}
	}

	GameOver:
	for (int count = 3; count > 0; count--) {
		quickCls();
		printf("%s%sScore: %d\n%s��", top_str.c_str(), left_str.c_str(), score, left_str.c_str());
		for (int i = 0; i < play_width; i++) printf("��");
		printf("��\n");
		for (int i = 0; i < half_height; i++) {
			printf("%s��", left_str.c_str());
			for (int j = 0; j < play_width; j++) printf("  ");
			printf("��\n");
		}
		printf("%s��", left_str.c_str());
		for (int i = 0; i < half_width_1; i++) printf("  ");
		printf("   Game over  ");
		for (int i = 0; i < play_width - half_width_1 - 7; i++) printf("  ");
		printf("��\n%s��", left_str.c_str());
		for (int i = 0; i < play_width; i++) printf("  ");
		printf("��\n%s��", left_str.c_str());
		for (int i = 0; i < half_width_2; i++) printf("  ");
		printf(" %d", count);
		for (int i = 0; i < play_width - half_width_2 - 1; i++) printf("  ");
		printf("��\n");
		for (int i = 0; i < play_height - half_height - 3; i++) {
			printf("%s��", left_str.c_str());
			for (int j = 0; j < play_width; j++) printf("  ");
			printf("��\n");
		}
		printf("%s��", left_str.c_str());
		for (int i = 0; i < play_width; i++) printf("��");
		printf("��\n");
		Sleep(1000);
	}
	return 1;
}

int get_width(int d_type, int d_dir) {
	switch (d_type) {
		case 1:
		switch (d_dir) {
			case 0:
			case 2:
			return 4;
			case 1:
			return 3;
			case 3:
			return 2;
		}
		case 2:
		return 2;
		//case 3 to 7:
		default:
		if (d_dir == 3) return 2; else return 3;
	}
}

int get_height(int d_type, int d_dir) {
	switch (d_type) {
		case 1:
		switch (d_dir) {
			case 0:
			return 2;
			case 2:
			return 3;
			//case 1 or 3:
			default:
			return 4;
		}
		case 2:
		return 2;
		//case 3 to 7:
		default:
		if (d_dir == 0) return 2; else return 3;
	}
}

int get_left(int d_type, int d_dir) {
	switch (d_type) {
		case 1:
		switch (d_dir) {
			case 1:
			return 2;
			case 3:
			return 1;
			//case 0 or 2:
			default:
			return 0;
		}
		case 2:
		return 0;
		//case 3 to 7:
		default:
		if (d_dir == 1) return 1; else return 0;
	}
}

//�������� https://blog.csdn.net/icy_ybk/article/details/79285245 
//��ò��Ҳû�����
void clearScreen()
{
    HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO bufInfo;
    SMALL_RECT scroll;
    COORD newCursorPointer;
    CHAR_INFO ciFill;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if(!GetConsoleScreenBufferInfo(hStdOut, &bufInfo))
        return;

    scroll.Left = 0;
    scroll.Top = 0;
    scroll.Right = bufInfo.dwSize.X;
    scroll.Bottom = bufInfo.dwSize.Y;
    newCursorPointer.X = 0;
    newCursorPointer.Y = -bufInfo.dwSize.Y;
    ciFill.Char.UnicodeChar = L' ';
    ciFill.Attributes = bufInfo.wAttributes;
    ScrollConsoleScreenBufferW(hStdOut, &scroll, NULL, newCursorPointer, &ciFill);
    newCursorPointer.Y = 0;
    SetConsoleCursorPosition(hStdOut, newCursorPointer);
}

//���ƶ������������������������
void quickCls() {
	HANDLE hout;
	COORD coord;
	coord.X=0;
	coord.Y=0;
	hout=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout,coord);

}