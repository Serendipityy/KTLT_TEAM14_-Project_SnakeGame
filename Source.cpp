
#include "nkea.h"
//constants
#define MAX_SIZE_SNAKE 40
#define MAX_SIZE_FOOD 4
#define MAX_SIZE_OBSTACLE 5
#define MAX_SIZE_GATE 5
#define MAX_SPEED 2
mutex m;
//GLOBAL variables
POINT snake[40];
POINT food[4];
POINT obstacle[5];
POINT gate[5];
int MSSV[40] = { 2,1,1,2,0,4,4,9,
2,1,1,2,0,4,5,8,
2,1,1,2,0,4,6,4,
2,1,1,2,0,4,7,5,
2,1,1,2,0,4,8,5 };
int CHAR_LOCK;
int MOVING;
int SPEED = 1;
int HEIGH_CONSOLE = 29, WIDTH_CONSOLE = 118;
int FOOD_INDEX;
int GATE_INDEX;
int SIZE_SNAKE;
int STATE;
int LEVEL;
int Score;
int threadrun = 1;
// Dieu kien de chay menu
int menu_run = 1;
int ESC = 27;
int back_to_menu = 0;
char Name[10];
// Thong so Spider
int nhen_x = 50;
int nhen_y = 10;
// sound on off
int sound_ = 1;
// Thong tin file in ra phan load
typedef struct save_info {
	char name[16];
	char timestr[11];
	int level, score;
};
void animationDead() {
	int n = 0;
	while (n <= 3) {
		for (int i = 0; i < SIZE_SNAKE; i++) {
			GotoXY(snake[i].x, snake[i].y);
			printf("%c",' ');	
		}
		Sleep(50);
		for (int i = 0; i < SIZE_SNAKE; i++) {
			GotoXY(snake[i].x, snake[i].y);
			printf("%d", MSSV[SIZE_SNAKE - 1 - i]);
		}
		n++;
	}
	Sleep(100);
}
// Kiểm tra vị trí của thức ăn
bool IsValid(int x, int y) {
	for (int i = 0; i < SIZE_SNAKE; i++)
		if (snake[i].x == x && snake[i].y == y)
			return false;
	return true;
}
// Kiem tra vi tri thuc an cach chuong ngai vat level 1 toi thieu 1 o
bool Food_Obstacle(int x, int y, int width, int height) {
	for (int i = 0; i < MAX_SIZE_OBSTACLE; i++)
		if ((x >= obstacle[i].x - 1)
			&& (x <= obstacle[i].x + width + 1)
			&& (y >= obstacle[i].y - 1)
			&& (y <= obstacle[i].y + height + 1))
			return false;
	return true;
}
//Cong khong trung chuong ngai vat level 1
bool Gate_Obstacle(int x, int y) {
	for (int i = 0; i < MAX_SIZE_OBSTACLE; i++)
		if ((x + 3 >= obstacle[i].x) && (x <= obstacle[i].x + 5)
			&& (y + 3 >= obstacle[i].y) && (y <= obstacle[i].y + 8))
			return false;
	return true;
}
//Kiem tra cong cach dau ran toi thieu 5 o
bool Gate_Snake(int x, int y) {
	if (snake[SIZE_SNAKE - 1].y >= y - 5 && snake[SIZE_SNAKE - 1].y <= y + 7
		&& snake[SIZE_SNAKE - 1].x >= x - 5 && snake[SIZE_SNAKE - 1].x <= x + 8)
		return false;
	return true;
}
// Khoi tao
void GenerateFood(int level_index) {
	int x, y;
	if (level_index == 1) {
		srand(time(NULL));
		do {
			x = rand() % (WIDTH_CONSOLE - 3) + 10;
			y = rand() % (HEIGH_CONSOLE - 3) + 2;
		} while (IsValid(x, y) == false);
		food[FOOD_INDEX] = { x,y };
	}
	if (level_index == 2) {
		srand(time(NULL));
		do {
			x = rand() % (WIDTH_CONSOLE - 3) + 10;
			y = rand() % (HEIGH_CONSOLE - 3) + 2;
		} while (IsValid(x, y) == false || Food_Obstacle(x, y, 5, 7) == false);
		food[FOOD_INDEX] = { x,y };
	}
	if (level_index == 3) {
		srand(time(NULL));
		do {
			x = rand() % (WIDTH_CONSOLE - 3) + 10;
			y = rand() % (HEIGH_CONSOLE - 3) + 2;
		} while (IsValid(x, y) == false
			|| (x == 40 && y > 4 && y < 8)
			|| (x == 46 && y > 12 && y < 16)
			|| (x > 30 && x < 40 && y == 8)
			|| (x > 46 && x < 56 && y == 12)
			|| (x == 46 && y > 4 && y < 8)
			|| (x == 40 && y > 12 && y < 16)
			|| (x > 30 && x < 40 && y == 12)
			|| (x > 46 && x < 56 && y == 8));
		food[FOOD_INDEX] = { x,y };
	}
	if (level_index == 4) {
		srand(time(NULL));
		do {
			x = rand() % (WIDTH_CONSOLE - 3) + 10;
			y = rand() % (HEIGH_CONSOLE - 3) + 2;
		} while (IsValid(x, y) == false
			|| (x > 23 && x < 35 && y == 5) || (x > 50 && x < 68 && y == 5)
			|| (x > 23 && x < 35 && y == 15) || (x > 50 && x < 68 && y == 15)
			|| (x == 15 && y == 2) || (x == 71 && y == 2)
			|| (x == 15 && y == 18) || (x == 71 && y == 18));
		food[FOOD_INDEX] = { x,y };
	}
}
void GenerateGate(int width, int height) {
	int x, y;
	srand(time(NULL));
	do {
		x = rand() % (WIDTH_CONSOLE - 1 - width) + 9;
		y = rand() % (HEIGH_CONSOLE - 2 - height) + 1;
	} while (Gate_Snake(x, y) == false || Gate_Obstacle(x, y) == false);
	gate[GATE_INDEX] = { x,y };
	if (LEVEL == 3) {
		do {
			x = rand() % (WIDTH_CONSOLE - 1 - width) + 9;
			y = rand() % (HEIGH_CONSOLE - 2 - height) + 1;
		} while (Gate_Snake(x, y) == false || (x < 39 && x > 57 && y < 7 && y > 17));
		gate[GATE_INDEX] = { x,y };
	}
}
// Chuc nang 
void ResetData() {
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1; FOOD_INDEX = 0, GATE_INDEX = 0,
		WIDTH_CONSOLE = 70, HEIGH_CONSOLE = 20, SIZE_SNAKE = 6; Score = 0, LEVEL = 1;
	snake[0] = { 10, 10 }; snake[1] = { 11, 10 };
	snake[2] = { 12, 10 }; snake[3] = { 13, 10 };
	snake[4] = { 14, 10 }; snake[5] = { 15, 10 };
	obstacle[0] = { 18, 1 }; obstacle[1] = { 41, 1 };
	obstacle[2] = { 64, 1 }; obstacle[3] = { 29, HEIGH_CONSOLE - 8 }; obstacle[4] = { 53, HEIGH_CONSOLE - 8 };
	DrawMapLv(LEVEL);
	GenerateFood(LEVEL);
}
void StartGame() {
	system("cls");
	ResetData();
	fontsize(16, 16);
	// Vẽ khung
	DrawBoard(81, 0, 32, HEIGH_CONSOLE);
	DrawBoard(8, HEIGH_CONSOLE + 4, WIDTH_CONSOLE, 5);
	DrawBoard(81, HEIGH_CONSOLE + 2, 32, 7);
	DrawBoard(8, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);

	setTextColor(2);
	DrawSnake_Game();

	setTextColor(13);
	DrawSnake_Text();

	setTextColor(14);
	DrawTeam14();
	DrawHowToPlay();

	GotoXY(87, HEIGH_CONSOLE - 5);
	setTextColor(7);
	cout << "Score:" << Score;
	GotoXY(87, HEIGH_CONSOLE - 3);
	cout << "LEVEL:" << LEVEL;
	ShowCur(0);
	STATE = 1;
}
void ExitGame() {
	system("cls");
	/*TerminateThread(t, 0);*/
	//threadrun = 0;
	_endthreadex(0);
}
void PauseGame(HANDLE t) {
	SuspendThread(t);
}
// Ve
void DrawObstacle(int x, int y, int width, int height) {
	GotoXY(x, y);
	for (int i = x; i < width + x; i++) {
		for (int j = y; j <= height + y; j++) {
			GotoXY(i, j); cout << char(219);
		}
	}
}
void DrawMapLv(int level_index) {
	switch (level_index) {
	case 1:
		break;
	case 2:
		//Draw Obstacle
		for (int i = 0; i < MAX_SIZE_OBSTACLE; i++) {
			DrawObstacle(obstacle[i].x, obstacle[i].y, 5, 7);
		}
		break;
	case 3:
		// Draw Obstacle
		for (int i = 4; i < 8; i++) {
			GotoXY(40, i);
			cout << char(219);
			GotoXY(46, i);
			cout << char(219);
		}
		for (int i = 12; i < 16; i++) {
			GotoXY(40, i);
			cout << char(219);
			GotoXY(46, i);
			cout << char(219);
		}
		for (int i = 30; i <= 40; i++) {
			GotoXY(i, 8);
			cout << char(219);
			GotoXY(i, 12);
			cout << char(219);
		}
		for (int i = 46; i <= 56; i++) {
			GotoXY(i, 8);
			cout << char(219);
			GotoXY(i, 12);
			cout << char(219);
		}
		break;
	case 4:
		DrawSpider();
		// tường ngang
		for (int i = 23; i <= 35; i++) {
			GotoXY(i, 5);
			cout << char(219);
			GotoXY(i, 15);
			cout << char(219);
		}
		for (int i = 51; i <= 63; i++) {
			GotoXY(i, 5);
			cout << char(219);
			GotoXY(i, 15);
			cout << char(219);
		}
		// 4 cục đá
		GotoXY(8 + 7, 0 + 2);
		cout << char(219);
		GotoXY(78 - 7, 0 + 2);
		cout << char(219);
		GotoXY(8 + 7, 20 - 2);
		cout << char(219);
		GotoXY(78 - 7, 20 - 2);
		cout << char(219);
		//MoveSpider;
	}
}
bool DrawSpider() {
	bool flag = true;
	if (flag) {
		GotoXY(nhen_x + 2, nhen_y - 2);
		cout << "/ _ \\";
		GotoXY(nhen_x, nhen_y - 1);
		cout << "\\_\\(_)/_/";
		GotoXY(nhen_x + 1, nhen_y);
		cout << "_//o\\\\_";
		GotoXY(nhen_x + 2, nhen_y + 1);
		cout << "/   \\";
	}
	return flag;
}
void DrawGate(int x, int y, int width, int height) {
	GotoXY(x, y);
	for (int i = 0; i <= width; i++)
		cout << char(219);
	for (int i = y + 1; i < y + height; i++) {
		GotoXY(x, i);
		cout << char(219);
	}
	for (int i = y + 1; i < y + height; i++) {
		GotoXY(x + width, i);
		cout << char(219);
	}
}
void DrawSnakeAndFood(char str) {
	if (FOOD_INDEX != -1) {
		GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
		printf("%c", str);
	}
	for (int i = 0; i < SIZE_SNAKE; i++) {
		GotoXY(snake[i].x, snake[i].y);
		printf("%d", MSSV[SIZE_SNAKE - 1 - i]);
	}
}
// Xoa cong
void ClearObstacle(int x, int y, int width, int height) {
	GotoXY(x, y);
	for (int i = x; i < width + x; i++) {
		for (int j = y; j <= height + y; j++) {
			GotoXY(i, j); printf("%c", ' ');
		}
	}
}
void ClearSnakeAndFood(char str) {
	GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	printf("%c", str);
	for (int i = 0; i < SIZE_SNAKE; i++) {
		GotoXY(snake[i].x, snake[i].y);
		printf("%c", str);
	}
}

void Ghim() {
	for (int i = 0; i < SIZE_SNAKE; i++) {
		snake[i] = { 10 + i,10 };
	}
	MOVING = 'D';
	CHAR_LOCK = 'A';
}
void ProcessDead() {
	STATE = 0;
	//PlaySound(TEXT("oh no sound.wav"), NULL, SND_ASYNC);
	//GotoXY(0, HEIGH_CONSOLE + 2);
	animationDead();
	DrawGameOver();
}
void Eat() {
	//PlaySound(TEXT("eat.wav"), NULL, SND_ASYNC);
	GotoXY(93, HEIGH_CONSOLE - 5);
	Score++;
	cout << Score;
	snake[SIZE_SNAKE] = food[FOOD_INDEX];
	
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1)
	{
		FOOD_INDEX = -1;
		//Create Gate
		GenerateGate(2, 2);
		DrawGate(gate[GATE_INDEX].x, gate[GATE_INDEX].y, 2, 2);
	}
	else {
		FOOD_INDEX++;
		if (SPEED == MAX_SPEED) SPEED = 1;
		else SPEED++;
		GenerateFood(LEVEL);
		SIZE_SNAKE++;
	}
}
void LevelUp(int level_index) {
	system("cls");
	fontsize(16, 16);

	DrawBoard(81, 0, 32, HEIGH_CONSOLE);
	DrawBoard(8, HEIGH_CONSOLE + 4, WIDTH_CONSOLE, 5);
	DrawBoard(81, HEIGH_CONSOLE + 2, 32, 7);
	DrawBoard(8, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);

	setTextColor(2);
	DrawSnake_Game();

	setTextColor(13);
	DrawSnake_Text();

	setTextColor(14);
	DrawTeam14();

	DrawHowToPlay();

	LEVEL++;
	GotoXY(87, HEIGH_CONSOLE - 5);
	setTextColor(7);
	cout << "Score:" << Score;
	GotoXY(87, HEIGH_CONSOLE - 3);
	cout << "LEVEL:" << LEVEL;
	STATE = 1;
	FOOD_INDEX = 0;
	GenerateFood(LEVEL);
	SPEED = 1;
	GATE_INDEX++;
	if (level_index == 2) {
		obstacle[0] = { 18, 1 }; obstacle[1] = { 41, 1 };
		obstacle[2] = { 64, 1 }; obstacle[3] = { 29, HEIGH_CONSOLE - 8 };
		obstacle[4] = { 53, HEIGH_CONSOLE - 8 };
	}
	Ghim();
	DrawMapLv(LEVEL);
}
/*Rắn chạm*/

bool SnakeTouchBody(int x, int y)
{
	for (int i = 1; i < SIZE_SNAKE; i++)
	{
		if (x == snake[i].x && y == snake[i].y)
		{
			return true;
		}
	}
	return false;
}
bool SnakeTouchObstacle(int x, int y, int width, int height)
{
	for (int i = 0; i < MAX_SIZE_OBSTACLE; i++) 
		if (x >= obstacle[i].x && x < obstacle[i].x + width
			&& y >= obstacle[i].y && y <= obstacle[i].y + height)
			return true;
	return false;
}
bool SnakeTouch_Lv3(int x, int y) {
	bool flag2 = false;
	if (
		// cham thanh doc tren
		(x == 40 && y >= 4 && y <= 8)
		|| (x == 46 && y >= 4 && y <= 8)
		// cham thanh doc duoi
		|| (x == 40 && y >= 12 && y < 16)
		|| (x == 46 && y >= 12 && y < 16)
		// cham thanh ngang trai
		|| (x >= 30 && x <= 40 && y == 8)
		|| (x >= 30 && x <= 40 && y == 12)
		// cham thanh ngang phai
		|| (x >= 46 && x <= 56 && y == 8)
		|| (x >= 46 && x <= 56 && y == 12)
		) {
		flag2 = true;
	}
	return flag2;
}
bool SnakeTouch_Lv4(int x, int y) {
	bool flag3 = false;
	if ((x >= 23 && x <= 35 && y == 5)
		|| (x >= 51 && x <= 63 && y == 15)
		|| (x == 15 && y == 2) || (x == 71 && y == 2)
		|| (x == 15 && y == 18) || (x == 71 && y == 18)) {
		flag3 = true;
	}
	return flag3;
}
bool SnakeTouchGate(int x, int y, int width, int height) {
	if (FOOD_INDEX < MAX_SIZE_FOOD - 1 && FOOD_INDEX != -1) return false;
	if (x == gate[GATE_INDEX].x + 1 && y == gate[GATE_INDEX].y)
	{
		return true;
	}
	if ((x == gate[GATE_INDEX].x || x == gate[GATE_INDEX].x + width)
		&& (y >= gate[GATE_INDEX].y && y < gate[GATE_INDEX].y + height))
	{
		return true;
	}
	return false;
}
bool SnakeTouchSpider(int x, int y) {
	bool flag = false;
	if ((x == nhen_x && y == nhen_y - 2) || (x == nhen_x && y == nhen_y) || (x==nhen_x && y==nhen_y-1)
		|| (x == nhen_x && y == nhen_y + 1) || (x == nhen_x + 1 && y == nhen_y + 1)
		|| (x == nhen_x + 2 && y == nhen_y + 1) || (x == nhen_x + 2 && y == nhen_y)
		|| (x == nhen_x + 2 && y == nhen_y - 2) || (x == nhen_x + 1 && y == nhen_y - 1)
		|| (x == nhen_x+2 && y ==nhen_y -1)||(x==nhen_x+2&&y==nhen_y +2)) {
		flag = true;
	}
	return flag;
}
/*----------------*/
// Di chuyen
void MoveSpider() {
	DrawSpider();
	//Sleep(90);
}
void MoveRight() {
	if (LEVEL == 1) {
		if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE + 8
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y, 2, 2 == true)) {
			ProcessDead();
		}
		else {
			if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].x++;
		}
	}
	if (LEVEL == 2) {
		if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE + 8
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouchObstacle(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y, 5, 7) == true
			|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y, 2, 2) == true) {
			ProcessDead();
		}
		else {
			if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].x++;
		}
	}
	if (LEVEL == 3) {
		if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE + 8
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouch_Lv3(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y, 2, 2) == true) {
			ProcessDead();
		}
		else {
			if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].x++;
		}
	}
	if (LEVEL == 4) {
		if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE + 8
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouchSpider(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouch_Lv4(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y) == true) {
			ProcessDead();
		}
		else {
			if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].x++;
		}
	}

}
void MoveLeft() {
	if (LEVEL == 1) {
		if (snake[SIZE_SNAKE - 1].x - 1 == 8
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y, 2, 2) == true) {
			ProcessDead();
		}
		else {
			if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].x--;
		}
	}
	if (LEVEL == 2) {
		if (snake[SIZE_SNAKE - 1].x - 1 == 8
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouchObstacle(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y, 5, 7) == true
			|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y, 2, 2) == true) {
			ProcessDead();
		}
		else {
			if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].x--;
		}
	}
	if (LEVEL == 3) {
		if (snake[SIZE_SNAKE - 1].x - 1 == 8
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouch_Lv3(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y, 2, 2) == true) {
			ProcessDead();
		}
		else {
			if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].x--;
		}
	}
	if (LEVEL == 4) {
		if (snake[SIZE_SNAKE - 1].x - 1 == 8
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouchSpider(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouch_Lv4(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y) == true) {
			ProcessDead();
		}
		else {
			if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].x--;
		}
	}
}
void MoveDown() {
	if (LEVEL == 1) {
		if (snake[SIZE_SNAKE - 1].y + 1 == HEIGH_CONSOLE
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1) == true
			|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1, 2, 2) == true) {
			ProcessDead();
		}
		else {
			if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y + 1 == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].y++;
		}
	}
	if (LEVEL == 2) {
		if (snake[SIZE_SNAKE - 1].y + 1 == HEIGH_CONSOLE
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1) == true
			|| SnakeTouchObstacle(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1, 5, 7) == true
			|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1, 2, 2) == true) {
			ProcessDead();
		}
		else {
			if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y + 1 == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].y++;
		}
	}
	if (LEVEL == 3) {
		if (snake[SIZE_SNAKE - 1].y + 1 == HEIGH_CONSOLE
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1) == true
			|| SnakeTouch_Lv3(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1, 2, 2) == true) {
			ProcessDead();
		}
		else {
			if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y + 1 == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].y++;
		}
	}
	if (LEVEL == 4) {
		if (snake[SIZE_SNAKE - 1].y + 1 == HEIGH_CONSOLE
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1) == true
			|| SnakeTouchSpider(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouch_Lv4(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y) == true) {
			ProcessDead();
		}
		else {
			if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y + 1 == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].y++;
		}
	}
}
void MoveUp() {
	if (LEVEL == 1) {
		if (snake[SIZE_SNAKE - 1].y - 1 == 0
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y - 1) == true
			|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y, 2, 2) == true) {
			ProcessDead();
		}
		else if (snake[SIZE_SNAKE - 1].x == gate[GATE_INDEX].x + 1 && snake[SIZE_SNAKE - 1].y == gate[GATE_INDEX].y + 1 && FOOD_INDEX == -1) {
			GotoXY(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y);
			printf("%c", ' ');
			SIZE_SNAKE--;
			if (SIZE_SNAKE == 0) {
				//Level up
				SIZE_SNAKE = 10;
				LevelUp(LEVEL);
			}
		}
		else {
			if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y - 1 == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].y--;
		}
	}
	else if (LEVEL == 2) {
		if (snake[SIZE_SNAKE - 1].y - 1 == 0
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y - 1) == true
			|| SnakeTouchObstacle(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y - 1, 5, 7) == true
			|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y, 2, 2) == true) {
			ProcessDead();
		}
		else if (snake[SIZE_SNAKE - 1].x == gate[GATE_INDEX].x + 1 && snake[SIZE_SNAKE - 1].y == gate[GATE_INDEX].y + 1 && FOOD_INDEX == -1) {
			GotoXY(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y);
			printf("%c", ' ');
			SIZE_SNAKE--;
			if (SIZE_SNAKE == 0) {
				//Level up
				SIZE_SNAKE = 14;
				LevelUp(LEVEL);
			}
		}
		else {
			if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y - 1 == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].y--;
		}
	}
	else if (LEVEL == 3) {
		if (snake[SIZE_SNAKE - 1].y - 1 == 0
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y - 1) == true
			|| SnakeTouch_Lv3(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y, 2, 2) == true) {
			ProcessDead();
		}
		else if (snake[SIZE_SNAKE - 1].x == gate[GATE_INDEX].x + 1 && snake[SIZE_SNAKE - 1].y == gate[GATE_INDEX].y + 1 && FOOD_INDEX == -1) {
			GotoXY(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y);
			printf("%c", ' ');
			SIZE_SNAKE--;
			if (SIZE_SNAKE == 0) {
				//Level up
				SIZE_SNAKE = 18;
				LevelUp(LEVEL);
				//thread f(MoveSpider);
				//f.detach();
			}
		}
		else {
			if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y - 1 == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].y--;
		}
	}
	else if (LEVEL == 4) {
		if (snake[SIZE_SNAKE - 1].y - 1 == 0
			|| SnakeTouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y - 1) == true
			|| SnakeTouchSpider(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y) == true
			|| SnakeTouch_Lv4(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y) == true) {
			ProcessDead();
		}
		else if (snake[SIZE_SNAKE - 1].x == gate[GATE_INDEX].x + 1 && snake[SIZE_SNAKE - 1].y == gate[GATE_INDEX].y + 1 && FOOD_INDEX == -1) {
			GotoXY(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y);
			printf("%c", ' ');
			SIZE_SNAKE--;
		}
		else {
			if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y - 1 == food[FOOD_INDEX].y) {
				Eat();
			}
			for (int i = 0; i < SIZE_SNAKE - 1; i++) {
				snake[i].x = snake[i + 1].x;
				snake[i].y = snake[i + 1].y;
			}
			snake[SIZE_SNAKE - 1].y--;
		}
	}
}
void ProcessEndgame() {
	back_to_menu = 1;
	system("cls");
	draw_gameover();
}
// Thread
void ThreadFunc() {
	while (back_to_menu == 0) {
		if (STATE == 1)
		{
			ClearSnakeAndFood(' ');
			switch (MOVING) {
			case 'A':
				MoveLeft();
				break;
			case 'D':
				MoveRight();
				break;
			case 'S':
				MoveDown();
				break;
			case 'W':
				MoveUp();
				break;
			}
			DrawSnakeAndFood('*');

			if (LEVEL == 4) {
				MoveSpider();
				//Sleep(10);
				Sleep(160 - 10 * LEVEL);
				m.lock();
				GotoXY(nhen_x + 2, nhen_y - 2);
				cout << "     ";
				GotoXY(nhen_x, nhen_y - 1);
				cout << "         ";
				GotoXY(nhen_x + 1, nhen_y);
				cout << "       ";
				GotoXY(nhen_x + 2, nhen_y + 1);
				cout << "     ";
				nhen_x++;
				m.unlock();
				if (nhen_x == 68) {
					nhen_x = 10;
				}
			}
			else Sleep(160 - 15*LEVEL);
			if (SIZE_SNAKE==0&&LEVEL==4)
			{
				ProcessEndgame();
			}
		}
	}
}
//overload 
istream& operator >> (istream& in, POINT& a)
{
	in >> a.x >> a.y;
	return in;
}
ostream& operator << (ostream& out, POINT a)
{
	out << a.x << " " << a.y << " ";
	return out;
}
// Save and load data
void LoadData(string name) {
	ifstream Data;
	name = "save/" + name;
	Data.open(name);
	Data >> LEVEL >> Score >>MOVING>> SIZE_SNAKE >> FOOD_INDEX >> SPEED;
	for (int i = 0; i < SIZE_SNAKE; i++) Data >> snake[i];
	Data >> food[FOOD_INDEX];
	if (FOOD_INDEX == -1) Data >> gate[GATE_INDEX];
	Data.close();
}
void SaveData(string name) {
	ofstream Data;
	name = "save/" + name;
	Data.open(name);
	Data << LEVEL << " " << Score <<" "<<MOVING<<" " << SIZE_SNAKE << " " << FOOD_INDEX << " " << SPEED << " ";
	for (int i = 0; i < SIZE_SNAKE; i++) Data << snake[i] << " ";
	Data << food[FOOD_INDEX]<<" ";
	if (FOOD_INDEX == -1) Data << gate[GATE_INDEX];
	Data.close();
}
void ProcessSave() {
	int HEIGH_CONSOLE = 20;
	char name[20];
	GotoXY(0, HEIGH_CONSOLE + 2);
	cout << "Enter name: ";
	//cin.ignore();
	//_getch();
	fflush(stdin);
	cin.getline(name, 20);
	cout << name;
	//strcat_s(name, ".txt");
	SaveData(name);
}
//Load stuff
vector<save_info> file;
vector<string> savename;
int x_menu = 4, y_menu = 6, main_menu_width = 14, main_menu_height = 22, x_filesave = x_menu + 20,
y_filesave = y_menu + 5;
//so luong file hien thi tren man hinh
int max_file_shown = 5;//0->5=6
//doc info in len man hinh
save_info tempfile;
void readinfo() {
	ifstream Data;
	char tempname[16] = "";
	strcat(tempname, "save/");
	strcat(tempname, tempfile.name);
	Data.open(tempname);
	Data >> tempfile.level >> tempfile.score;
	struct stat buf;
	if (!stat(tempname, &buf)) {
		strftime(tempfile.timestr, 100, "%d-%m-%Y", localtime(&buf.st_mtime));
	}
	Data.close();
}
//nhap sanh sach file luu
void listFiles(const char* dirname) {
	DIR* dir = opendir(dirname);
	if (dir == NULL) {
		return;
	}
	struct dirent* entity;
	entity = readdir(dir);
	entity = readdir(dir);
	entity = readdir(dir);
	while (entity != NULL) {
		strcpy(tempfile.name, entity->d_name);
		readinfo();
		file.push_back(tempfile);
		entity = readdir(dir);
	}
	closedir(dir);
}
//in danh sach file saved
void printlist(int begin, int end) {
	for (int i = begin; i <= end; i++) {
		GotoXY(x_filesave, y_filesave + (i - begin) * 2);
		cout << setw(14) << file[i].name << setw(20) << file[i].level << setw(20) << file[i].score << setw(26) << file[i].timestr << "  ";
	}
}
void Delete_detail_board() {
	string blank = "                                                                                   ";
	for (int i = 0; i < 15; i++) {
		GotoXY(x_filesave, y_filesave - 4 + i);
		cout << blank;
	}
}
void ProcessLoad() {
	char k;
	int Cur_Choice = 0, Cur_element = 0;
	int num = file.size();
	
	if (num-1 < max_file_shown) max_file_shown = num - 1;
	GotoXY(0, 0); cout << num << max_file_shown;
	//in cot 
	setTextColor(15);
	GotoXY(x_filesave, y_filesave - 2);
	cout << setw(14) << "NAME" << setw(20) << "LEVEL" << setw(20) << "SCORE" << setw(26) << "DATE";
	while (true)
	{
		setTextColor(15);
		printlist(Cur_element - Cur_Choice, Cur_element - Cur_Choice + max_file_shown);
		setTextColor(11);
		GotoXY(x_filesave, y_filesave + Cur_Choice * 2);
		cout << ">>" << setw(14) << file[Cur_element].name << setw(20) << file[Cur_element].level << setw(20) << file[Cur_element].score << setw(26) << file[Cur_element].timestr;
		k = toupper(_getch());
		if (k == 'W') {
			Cur_element--;
			if (Cur_Choice > 0) Cur_Choice--;
		}
		if (k == 'S') {
			Cur_element++;
			if (Cur_Choice < max_file_shown) Cur_Choice++;
		}
		if (k == '\r' || k == 'D') {
			strcpy(Name,file[Cur_element].name);
			LoadData(Name);
			system("cls");
			setTextColor(7);
			if (FOOD_INDEX == -1) DrawGate(gate[GATE_INDEX].x, gate[GATE_INDEX].y, 2, 2);
			DrawMapLv(LEVEL);
			Start();
			DrawSnakeAndFood('*');
			getch();
			return;
		}
		if (k == ESC || k == 'A') {
			Delete_detail_board();
			return;
		}
		if (Cur_element < 0) {
			Cur_element = num - 1;
			Cur_Choice = max_file_shown;
		}
		if (Cur_element > num - 1) {
			Cur_element = 0;
			Cur_Choice = 0;
		}
		Sleep(50);
	}
	return;
}

// menu stuff
string menu_list[5] = { "Play","Load game","Setting","Instruction","Exit" };
int menu_list_element_count;
int menu_choice;
void NewGame() {

	GotoXY(x_filesave, y_filesave + 11);
	cout << "limit 10 character";
	GotoXY(x_filesave, y_filesave + 10);
	cout << "Enter name : ";
	cin >> Name;
	system("cls");
	ResetData();
}
void Start() {
	fontsize(16, 16);
	// Vẽ khung
	DrawBoard(81, 0, 32, HEIGH_CONSOLE);
	DrawBoard(8, HEIGH_CONSOLE + 4, WIDTH_CONSOLE, 5);
	DrawBoard(81, HEIGH_CONSOLE + 2, 32, 7);
	DrawBoard(8, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);

	setTextColor(2);
	DrawSnake_Game();

	setTextColor(13);
	DrawSnake_Text();

	setTextColor(14);
	DrawTeam14();
	DrawHowToPlay();

	GotoXY(87, HEIGH_CONSOLE - 5);
	setTextColor(7);
	cout << "Score:" << Score;
	GotoXY(87, HEIGH_CONSOLE - 3);
	cout << "LEVEL:" << LEVEL;
	STATE = 1;
	return;
}
void ProcessStart() {
	int x_mid_detail_board = x_filesave + 35, y_mid_detail_board = y_filesave;
	if (Score == 0) {
		GotoXY(x_mid_detail_board, y_mid_detail_board);
		setTextColor(11);
		cout << ">> New Game <<";
		while (1) {
			char c = _toupper(getch());
			if (c == '\r' || c == 'D') {
				NewGame();
				//system("cls");
				break;
			}
			else if (c == ESC || c == 'A') {
				Delete_detail_board();
				return;
			}
			Sleep(50);
		}
	}
	else {
		int cur_choice = 0;
		string choice[2] = { "   New Game   ",
							  "   Continue   " };
		string choice_[2] = { ">> New Game <<",
							  ">> Continue <<" };
		while (1)
		{
			GotoXY(x_mid_detail_board, y_mid_detail_board + 3 * cur_choice);
			setTextColor(11);
			cout << choice_[cur_choice];
			GotoXY(x_mid_detail_board, y_mid_detail_board + 3 * (1 - cur_choice));
			setTextColor(15);
			cout << choice[1 - cur_choice];
			char c = _toupper(getch());
			if (c == 'S') cur_choice++;
			else if (c == 'W') cur_choice--;
			if (c == '\r' || c == 'D') {
				if (cur_choice == 0) {
					NewGame();
				}
				else {
					system("cls");
					if (FOOD_INDEX == -1) DrawGate(gate[GATE_INDEX].x, gate[GATE_INDEX].y, 2, 2);
					DrawMapLv(LEVEL);
				}
				break;
			}
			else if (c == ESC || c == 'A') {
				Delete_detail_board();
				return;
			}
			if (cur_choice > 1) cur_choice = 0;
			else if (cur_choice < 0) cur_choice = 1;
			Sleep(50);
		}
	}
	Start();

}
void ProcessSetting() {
	int x_mid_detail_board = x_filesave + 35, y_mid_detail_board = y_filesave;
	GotoXY(x_mid_detail_board, y_mid_detail_board);
	cout << "Sound";
	char c;
	while (1) {
		GotoXY(x_mid_detail_board + 5, y_mid_detail_board);
		setTextColor(11);
		if (sound_ == 1) cout << "  on ";
		else cout << "  off";
		c = _toupper(getch());
		if (c == '\r' || c == 'D') {
			sound_ = 1 - sound_;
		}
		else if (c == ESC || c == 'A') {
			Delete_detail_board();
			return;
		}
		Sleep(50);
	}
}
void ProcessInstruction() {
	setTextColor(11);
	GotoXY(x_filesave, y_filesave - 2);
	cout << char(30) << "       : W";
	GotoXY(x_filesave, y_filesave);
	cout << char(31) << "       : S";
	GotoXY(x_filesave, y_filesave + 2);
	cout << char(16) << "       : A";
	GotoXY(x_filesave, y_filesave + 4);
	cout << char(17) << "       : D";
	GotoXY(x_filesave, y_filesave + 6);
	cout << "Confirm : D/Enter";
	GotoXY(x_filesave, y_filesave + 8);
	cout << "Back    : A/Esc";
	char c;
	c = _toupper(getch());
	if (c == ESC || c == 'A') {
		Delete_detail_board();
		return;
	}
}
void Menu()
{
	//tch();
	int HEIGH_CONSOLE = 29, WIDTH_CONSOLE = 118;
	//PlaySound(TEXT("menu.wav"), NULL, SND_ASYNC);
	int menu_choice = 0;
	char c;
	int check = 0;
	Draw_Board(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
	//draw main menu board(left)
	//Draw_Board(x_menu, y_menu, main_menu_width, main_menu_height, 0, 0);
	Draw_menu_board();
	setTextColor(15);
	for (int i = 0; i < 5; i++) {
		GotoXY(12 - pos_calc(menu_list[i]), y_menu + 3 + i * 4);
		cout << menu_list[i];
	}
	//draw detail menu board (right)
	Draw_Board(x_menu + 18, y_menu, WIDTH_CONSOLE - 26, HEIGH_CONSOLE - 7);
	//divide detail table 
	setTextColor(9);
	GotoXY(x_filesave - 2, y_filesave + (6) * 2);
	cout << char(204);
	GotoXY(x_filesave - 2 + WIDTH_CONSOLE - 26, y_filesave + (6) * 2);
	cout << char(185);
	GotoXY(x_filesave - 1, y_filesave + (6) * 2);
	for (int i = 1; i < WIDTH_CONSOLE - 26; i++) cout << char(205);
	while (menu_run == 1)
	{
		setTextColor(11);
		GotoXY(12 - pos_calc(menu_list[menu_choice]), y_menu + 3 + menu_choice * 4);
		cout << menu_list[menu_choice];
		setTextColor(15);
		if (_kbhit() == true)
		{
			c = toupper(_getch());
			if (c == 'W') {
				GotoXY(12 - pos_calc(menu_list[menu_choice]), y_menu + 3 + menu_choice * 4);
				cout << menu_list[menu_choice];
				menu_choice--;
			}
			if (c == 'S') {
				GotoXY(12 - pos_calc(menu_list[menu_choice]), y_menu + 3 + menu_choice * 4);
				cout << menu_list[menu_choice];
				menu_choice++;
			}
			if (c == '\r' || c == 'D') {
				if (menu_choice == 4)//EXIT
					exit(0);
				if (menu_choice == 0) {//PLAY
					ProcessStart();
					if (STATE == 1) return;
				}
				if (menu_choice == 1) {//LOAD GAME 
					ProcessLoad();
					if (STATE == 1) return;
				}
				if (menu_choice == 2)//setting
				{
					ProcessSetting();
				}
				//go to option - chua lam
				if (menu_choice == 3)//instruction
				{
					ProcessInstruction();
				}
				//print high scoore - chua lam
			}
		}
		if (menu_choice < 0)
			menu_choice = 4;
		if (menu_choice > 4)
			menu_choice = 0;
		Sleep(50);
	}
}
void DeadOption() {
	string death_option[3] = { " Play again "," Menu "," Exit " };
	string death_option_[3] = { "  Play again  ","  Menu  ","  Exit  " };
	int x_over = 15, y_over = 15;
	int cur_choice = 0;
	system("cls");
	draw_gameover();
	char c;
	while (1) {
		setTextColor(14);
		for (int i = 0; i < 3; i++) {
			GotoXY(x_over + 10 - pos_calc(death_option_[i]), y_over + i * 3 + 4);
			cout << death_option_[i];
		}
		GotoXY(x_over + 10 - pos_calc(death_option_[cur_choice]), y_over + cur_choice * 3 + 4);
		cout << char(175) << death_option[cur_choice] << char(174);
		c = toupper(getch());
		if (c == 'W') {
			cur_choice--;
		}
		else if (c == 'S') {
			cur_choice++;
		}
		else if (c == '\r' || c == 'D') {
			if (cur_choice == 2) {
				ExitGame();
			}
			else if (cur_choice == 0) {
				system("cls");
				ResetData();
				Start();
			}
			else if (cur_choice == 1)
			{
				system("cls");
				back_to_menu = 1;
			}
			return;
		}
		if (cur_choice < 0) cur_choice = 2;
		if (cur_choice > 2) cur_choice = 0;
		Sleep(50);
	}
	//TerminateThread(handle_t1, 0);
	setTextColor(12);

}
void Run() {
	int temp;
	thread t(ThreadFunc);
	HANDLE handle_t = t.native_handle();
	back_to_menu = 0;
	while (1) {
		temp = toupper(_getch());
		if (STATE == 1) {
			if (temp == 'P') {
				PauseGame(handle_t);
			}
			else if (temp == 'M') {
				STATE = 0;
				system("cls");
				back_to_menu = 1;
				t.detach();
				return;
			}
			else if (temp == 'L') {
				SaveData(Name);
			}
			else if (temp == 27) {
				ExitGame();
				return;
			}
			else {
				ResumeThread(handle_t);
				if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S'))
				{
					if (temp == 'D') CHAR_LOCK = 'A';
					else if (temp == 'W') CHAR_LOCK = 'S';
					else if (temp == 'S') CHAR_LOCK = 'W';
					else CHAR_LOCK = 'D';
					MOVING = temp;
					Sleep(160 - 10 * LEVEL);
				}
			}
		}
		else {
			 DeadOption();
			if (back_to_menu == 1) {

				t.detach();
				//TerminateThread(handle_t, 0);
				return;
			}
		}
	}
}
//
void main() {
	setTextColor(10);
	ShowCur(0);
	Draw_Newgame_intro();
	FixconsoleWindow();
	ResetData();
	while (1) {
		listFiles("save");
		Menu();
		file.clear();
		Run();
		
	}
}
