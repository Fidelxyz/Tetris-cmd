#include<iostream>
#include<cstdio>
#include<windows.h>
#include<string>
#include<stdlib.h>
#include <time.h>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#define random(x) (rand()%x)
using namespace std;

string refresh_left(int left);
string refresh_top(int top);
int get_width(int d_type, int d_dir);
int get_height(int d_type, int d_dir);
int get_left(int d_type, int d_dir);
void one_player(string left_str, string top_str, int play_width, int play_height, int tick);
void paint(int d_type, int d_dir, int d_x, int d_y);

int selects = 0;
string left_str, top_str;
int play_width = 10, play_height = 20;
int tick = 50;

int main() {
	int left = 10, top = 3;
	system("mode con cols=100 lines=32");
	system("color F");
	system("chcp 936");

	//禁用快速编辑模式
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE; //移除快速编辑模式
	mode &= ~ENABLE_INSERT_MODE; //移除插入模式
	mode &= ~ENABLE_MOUSE_INPUT;
	SetConsoleMode(hStdin, mode);

	left_str = refresh_left(left);
	top_str = refresh_top(top);
	while (true) {
		system("cls");
		cout << top_str; 
		if (selects == 0) {
			cout << left_str << "┏━━━━┓\n"
			<< left_str << "┃单人模式┃\n"
			<< left_str << "┗━━━━┛\n";
		}else {
			cout << "\n" << left_str << "  单人模式\n\n";
		}
		cout << "\n";
		if (selects == 1) {
			cout << left_str << "┏━━━━┓\n"
			<< left_str << "┃双人对战┃\n"
			<< left_str << "┗━━━━┛\n";
		}else {
			cout << "\n" << left_str << "  双人对战\n\n";
		}
		cout << "\n";
		if (selects == 2) {
			cout << left_str << "┏━━━━┓\n"
			<< left_str << "┃  设置  ┃\n"
			<< left_str << "┗━━━━┛\n";
		}else {
			cout << "\n" << left_str << "    设置\n\n";
		}
		cout << "\n";
		if (selects == 3) {
			cout << left_str << "┏━━━━┓\n"
			<< left_str << "┃  退出  ┃\n"
			<< left_str << "┗━━━━┛\n";
		}else {
			cout << "\n" << left_str << "    退出\n\n";
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
					one_player(left_str, top_str, play_width, play_height, tick);
					break;
					case 1:
						//TODO
					break;
					case 2:
						//TODO
					left_str = refresh_left(left);
					top_str = refresh_top(left);
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

void one_player(string left_str, string top_str, int play_width, int play_height, int tick) {
	srand((unsigned int)(time(NULL)));
	bool refresh = false, down = false, hard_down = false, apply = false, paint_back = false;
	int tmp_dir, tmp_x, tmp_y;
	int tick_count = 0;
	int d_type = 0, d_dir = 0, d_x, d_y;
	bool scr[play_width][play_height];
	bool block[play_width][play_height];
	int check_max_y;
	int last_type = 0;
	
	for(int i = 0; i < play_width; i++) {
		for(int j = 0; j < play_height; j++) {
			scr[i][j] = false;
			block[i][j] = false;
		}
	}
	
	int half_height = (play_height - 3) / 2;
	int half_width_1 = (play_width - 7) / 2;
	int half_width_2 = (play_width - 1) / 2; 
	
	//Count down
	/*for (int count = 3; count > 0; count--) {
		system("cls");
		printf("%s%s┏", top_str.c_str(), left_str.c_str());
		for (int i = 0; i < play_width; i++) printf("━");
		printf("┓\n");
		for (int i = 0; i < half_height; i++) {
			printf("%s┃", left_str.c_str());
			for (int j = 0; j < play_width; j++) printf("  ");
			printf("┃\n");
		}
		printf("%s┃", left_str.c_str());
		for (int i = 0; i < half_width_1; i++) printf("  ");
		printf(" Game start in");
		for (int i = 0; i < play_width - half_width_1 - 7; i++) printf("  ");
		printf("┃\n%s┃", left_str.c_str());
		for (int i = 0; i < play_width; i++) printf("  ");
		printf("┃\n%s┃", left_str.c_str());
		for (int i = 0; i < half_width_2; i++) printf("  ");
		printf(" %d", count);
		for (int i = 0; i < play_width - half_width_2 - 1; i++) printf("  ");
		printf("┃\n");
		for (int i = 0; i < play_height - half_height - 3; i++) {
			printf("%s┃", left_str.c_str());
			for (int j = 0; j < play_width; j++) printf("  ");
			printf("┃\n");
		}
		printf("%s┗", left_str.c_str());
		for (int i = 0; i < play_width; i++) printf("━");
		printf("┛\n");
		Sleep(1000);
	}*/
	
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
				//消除行检测 Clear check
				int clear[get_height(last_type, d_dir)]; //save clear Y
				for (int i = 0; i < get_height(last_type, d_dir); i++) clear[i] = 0;
					int clear_num = 0;
				check_max_y = d_y + get_height(last_type, d_dir) - 1;
				for (int y = d_y; y <= check_max_y; y++) {
					int i;
					for (i = 0; i < play_width; i++) {
						if (!block[i][y]) break;
					}
					if (i != play_width) {//clear
						clear[clear_num] = y;
						clear_num++;
					}
				}
				for (int i = 0; i <= clear_num; i++) {
					for (int x = 0; x < play_width; x++) block[clear[clear_num]][x] = false;
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
	switch (d_type) {
		case 1:
				//长条 
		switch (d_dir) {
			case 0:
			scr[d_x][d_y + 1] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 2][d_y + 1] =
			scr[d_x + 3][d_y + 1] = true;
			break;
			case 1:
			scr[d_x + 2][d_y] =
			scr[d_x + 2][d_y + 1] =
			scr[d_x + 2][d_y + 2] =
			scr[d_x + 2][d_y + 3] = true;
			break;
			case 2:
			scr[d_x][d_y + 2] =
			scr[d_x + 1][d_y + 2] =
			scr[d_x + 2][d_y + 2] =
			scr[d_x + 3][d_y + 2] = true;
			break;
			case 3:
			scr[d_x + 1][d_y] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 1][d_y + 2] =
			scr[d_x + 1][d_y + 3] = true;
			break;
		}
		break;
		case 2:
				//正方形 
		scr[d_x][d_y] =
		scr[d_x + 1][d_y] =
		scr[d_x][d_y + 1] =
		scr[d_x + 1][d_y + 1] = true;
		break;
		case 3:
				//反L形 
		switch (d_dir) {
			case 0:
			scr[d_x][d_y] =
			scr[d_x][d_y + 1] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 2][d_y + 1] = true;
			break;
			case 1:
			scr[d_x + 1][d_y] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 1][d_y + 2] =
			scr[d_x + 2][d_y] = true;
			break;
			case 2:
			scr[d_x][d_y + 1] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 2][d_y + 1] =
			scr[d_x + 2][d_y + 2] = true;
			break;
			case 3:
			scr[d_x + 1][d_y] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 1][d_y + 2] =
			scr[d_x][d_y + 2] = true;
			break;
		}
		break;
		case 4:
				//L形 
		switch (d_dir) {
			case 0:
			scr[d_x + 2][d_y] =
			scr[d_x][d_y + 1] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 2][d_y + 1] = true;
			break;
			case 1:
			scr[d_x + 1][d_y] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 1][d_y + 2] =
			scr[d_x + 2][d_y + 2] = true;
			break; 
			case 2:
			scr[d_x][d_y + 1] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 2][d_y + 1] =
			scr[d_x][d_y + 2] = true;
			break;
			case 3:
			scr[d_x][d_y] =
			scr[d_x + 1][d_y] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 1][d_y + 2] = true;
			break;		
		}
		break;
		case 5:
				//T形 
		switch (d_dir) {
			case 0:
			scr[d_x + 1][d_y] =
			scr[d_x][d_y + 1] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 2][d_y + 1] = true;
			break;
			case 1:
			scr[d_x + 1][d_y] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 1][d_y + 2] =
			scr[d_x + 2][d_y + 1] = true;
			break;
			case 2:
			scr[d_x][d_y + 1] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 2][d_y + 1] =
			scr[d_x + 1][d_y + 2] = true;
			break;
			case 3:
			scr[d_x][d_y + 1] =
			scr[d_x + 1][d_y] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 1][d_y + 2] = true;
			break;
		}
		break;
		case 6:
				//Z形 
		switch (d_dir) {
			case 0:
			scr[d_x][d_y] =
			scr[d_x + 1][d_y] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 2][d_y + 1] = true;
			break;
			case 1:
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 1][d_y + 2] =
			scr[d_x + 2][d_y] =
			scr[d_x + 2][d_y + 1] = true;
			break;
			case 2:
			scr[d_x][d_y + 1] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 1][d_y + 2] =
			scr[d_x + 2][d_y + 2] = true;
			break;
			case 3:
			scr[d_x][d_y + 1] =
			scr[d_x][d_y + 2] =
			scr[d_x + 1][d_y] =
			scr[d_x + 1][d_y + 1] = true;
			break;
		}
		break;
		case 7:
				//反Z型 
		switch (d_dir) {
			case 0:
			scr[d_x + 1][d_y] =
			scr[d_x + 2][d_y] =
			scr[d_x][d_y + 1] =
			scr[d_x + 1][d_y + 1] = true;
			break;
			case 1:
			scr[d_x + 1][d_y] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 2][d_y + 1] =
			scr[d_x + 2][d_y + 2] = true;
			break;
			case 2:
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 2][d_y + 1] =
			scr[d_x][d_y + 2] =
			scr[d_x + 1][d_y + 2] = true;
			break;
			case 3:
			scr[d_x][d_y] =
			scr[d_x][d_y + 1] =
			scr[d_x + 1][d_y + 1] =
			scr[d_x + 1][d_y + 2] = true;
			break;
		}
		break;
	}
	if (paint_back) goto PaintBack;
	
		//Disable
	system("cls");
	printf("%s%s┏", top_str.c_str(), left_str.c_str());
	for (int i = 0; i < play_width; i++) printf("━");
		printf("┓\n");
	for (int i = 0; i < play_height; i++) {
		printf("%s┃", left_str.c_str());
		for (int j = 0; j < play_width; j++) printf(scr[j][i] ? "■" : "  ");
			printf("┃\n");
	}
	printf("%s┗", left_str.c_str());
	for (int i = 0; i < play_width; i++) printf("━");
		printf("┛\n");
	
		//test
	printf("\nBlock width: %d\nBlock height: %d\nBlock left: %d\nBlock type: %d\nBlock direction: %d", get_width(d_type, d_dir), get_height(d_type, d_dir), get_left(d_type, d_dir), d_type, d_dir);
	
	while (true) {
		Sleep(50);
		tick_count += tick;
			//转向检测频率0.1s
			//左转 
		if((tick_count % 100 == 0) && KEY_DOWN('E')) {
			if (d_dir == 3) tmp_dir = 0; else tmp_dir = d_dir + 1;
			if ((d_x + get_width(d_type, d_dir) == play_width) && (get_width(d_type, tmp_dir) > get_width(d_type, d_dir))) tmp_x -= get_width(d_type, tmp_dir) - get_width(d_type, d_dir);
			if ((d_x + get_left(d_type, d_dir) == 0) && (get_left(d_type, tmp_dir) < get_left(d_type, d_dir))) tmp_x += get_left(d_type, d_dir) - get_left(d_type, tmp_dir);
			refresh = true;
		}
			//右转 
		if((tick_count % 100 == 0) && KEY_DOWN('Q')) {
			if (d_dir == 0) tmp_dir = 3; else tmp_dir = d_dir - 1;
			if ((d_x + get_width(d_type, d_dir) == play_width) && (get_width(d_type, tmp_dir) > get_width(d_type, d_dir))) tmp_x -= get_width(d_type, tmp_dir) - get_width(d_type, d_dir);
			if ((d_x + get_left(d_type, d_dir) == 0) && (get_left(d_type, tmp_dir) < get_left(d_type, d_dir))) tmp_x += get_left(d_type, d_dir) - get_left(d_type, tmp_dir);
			refresh = true;
		}
			//左移 
		if(KEY_DOWN('A')) {
			if (d_x + get_left(d_type, d_dir) > 0) {
				tmp_x = d_x - 1;
				refresh = true;
			}
		}
			//右移 
		if(KEY_DOWN('D')) {
			if (d_x + get_width(d_type, d_dir) < play_width) {
				tmp_x = d_x + 1;
				refresh = true;
			}
		}
		if(KEY_DOWN('W')) {
			tmp_y = d_y + 1;
			down = true;
			hard_down = true;
			refresh = true;
		}
		if(KEY_DOWN('S')) {
			tmp_y = d_y + 1;
			down = true;
			refresh = true;
		}
		
		if (tick_count >= 1000) {
			tick_count = 0;
			tmp_y = d_y + 1;
			down = true;
			refresh = true;
		}
		
			//检查变换是否合法 
			//Refresh check
		if (refresh) {
			Refresh_check:
			apply = true;
			switch (d_type) {
				case 1:
						//长条 
				switch (tmp_dir) {
					case 0:
					if (block[tmp_x][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 2][tmp_y + 1]
						|| block[tmp_x + 3][tmp_y + 1])
						apply = false;
					break;
					case 1:
					if (block[tmp_x + 2][tmp_y]
						|| block[tmp_x + 2][tmp_y + 1]
						|| block[tmp_x + 2][tmp_y + 2]
						|| block[tmp_x + 2][tmp_y + 3])
						apply = false;
					break;
					case 2:
					if (block[tmp_x][tmp_y + 2]
						|| block[tmp_x + 1][tmp_y + 2]
						|| block[tmp_x + 2][tmp_y + 2]
						|| block[tmp_x + 3][tmp_y + 2])
						apply = false;
					break;
					case 3:
					if (block[tmp_x + 1][tmp_y]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 2]
						|| block[tmp_x + 1][tmp_y + 3])
						apply = false;
					break;
				}
				break;
				case 2:
						//正方形 
				if (block[tmp_x][tmp_y]
					|| block[tmp_x + 1][tmp_y]
					|| block[tmp_x][tmp_y + 1]
					|| block[tmp_x + 1][tmp_y + 1])
					apply = false;
				break;
				case 3:
						//反L形 
				switch (tmp_dir) {
					case 0:
					if (block[tmp_x][tmp_y]
						|| block[tmp_x][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 2][tmp_y + 1])
						apply = false;
					break;
					case 1:
					if (block[tmp_x + 1][tmp_y]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 2]
						|| block[tmp_x + 2][tmp_y])
						apply = false;
					break;
					case 2:
					if (block[tmp_x][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 2][tmp_y + 1]
						|| block[tmp_x + 2][tmp_y + 2])
						apply = false;
					break;
					case 3:
					if (block[tmp_x + 1][tmp_y]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 2]
						|| block[tmp_x][tmp_y + 2])
						apply = false;
					break;
				}
				break;
				case 4:
						//L形 
				switch (tmp_dir) {
					case 0:
					if (block[tmp_x + 2][tmp_y]
						|| block[tmp_x][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 2][tmp_y + 1])
						apply = false;
					break;
					case 1:
					if (block[tmp_x + 1][tmp_y]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 2]
						|| block[tmp_x + 2][tmp_y + 2])
						apply = false;
					break;
					case 2:
					if (block[tmp_x][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 2][tmp_y + 1]
						|| block[tmp_x][tmp_y + 2])
						apply = false;
					break;
					case 3:
					if (block[tmp_x][tmp_y]
						|| block[tmp_x + 1][tmp_y]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 2])
						apply = false;
					break;
				}
				break;
				case 5:
						//T形 
				switch (tmp_dir) {
					case 0:
					if (block[tmp_x + 1][tmp_y]
						|| block[tmp_x][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 2][tmp_y + 1])
						apply = false;
					break;
					case 1:
					if (block[tmp_x + 1][tmp_y]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 2]
						|| block[tmp_x + 2][tmp_y + 1])
						apply = false;
					break;
					case 2:
					if (block[tmp_x][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 2][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 2])
						apply = false;
					break;
					case 3:
					if (block[tmp_x][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 2])
						apply = false;
					break;
				}
				break;
				case 6:
						//Z形 
				switch (tmp_dir) {
					case 0:
					if (block[tmp_x][tmp_y]
						|| block[tmp_x + 1][tmp_y]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 2][tmp_y + 1])
						apply = false;
					break;
					case 1:
					if (block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 2]
						|| block[tmp_x + 2][tmp_y]
						|| block[tmp_x + 2][tmp_y + 1])
						apply = false;
					break;
					case 2:
					if (block[tmp_x][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 2]
						|| block[tmp_x + 2][tmp_y + 2])
						apply = false;
					break;
					case 3:
					if (block[tmp_x][tmp_y + 1]
						|| block[tmp_x][tmp_y + 2]
						|| block[tmp_x + 1][tmp_y]
						|| block[tmp_x + 1][tmp_y + 1])
						apply = false;
					break;
				}
				break;
				case 7:
						//反Z型 
				switch (tmp_dir) {
					case 0:
					if (block[tmp_x + 1][tmp_y]
						|| block[tmp_x + 2][tmp_y]
						|| block[tmp_x][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 1])
						apply = false;
					break;
					case 1:
					if (block[tmp_x + 1][tmp_y]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 2][tmp_y + 1]
						|| block[tmp_x + 2][tmp_y + 2])
						apply = false;
					break;
					case 2:
					if (block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 2][tmp_y + 1]
						|| block[tmp_x][tmp_y + 2]
						|| block[tmp_x + 1][tmp_y + 2])
						apply = false;
					break;
					case 3:
					if (block[tmp_x][tmp_y]
						|| block[tmp_x][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 1]
						|| block[tmp_x + 1][tmp_y + 2])
						apply = false;
					break;
				}
				break;
			}
			if (d_y + get_height(d_type, d_dir) >= play_height) apply = false;
			if (apply) {
				d_dir = tmp_dir;
				d_x = tmp_x;
				d_y = tmp_y;
					//硬降 
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
