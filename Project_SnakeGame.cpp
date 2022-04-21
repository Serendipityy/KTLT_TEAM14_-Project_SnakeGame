#include <windows.h> //for HWND
#include <iostream>
#include <time.h> //for srand(time(NULL));
#include <conio.h>//for getch()
#include <thread>
#include <fstream>
#include <string.h>
#include <string>
#include <filesystem>
namespace fs = std::experimental::filesystem;
using namespace std;
/*typedef struct _COORD {
	SHORT X;
	SHORT Y;
} COORD, * PCOORD;*/
//constants
#define MAX_SIZE_SNAKE 10
#define MAX_SIZE_FOOD 4
#define MAX_SPEED 3
//GLOBAL variables
POINT snake[10];
POINT food[4];
int CHAR_LOCK;
int MOVING;
int SPEED;
int HEIGH_CONSOLE, WIDTH_CONSOLE;
int FOOD_INDEX;
int SIZE_SNAKE;
int STATE;
int Score;
int threadrun = 1;
void FixconsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
bool IsValid(int x, int y) {
	for (int i = 0; i < SIZE_SNAKE; i++)
		if (snake[i].x == x && snake[i].y == y)
			return false;
	return true;
}
void GenerateFood() {
	int x, y;
	srand(time(NULL));
	for (int i = 0; i < MAX_SIZE_FOOD; i++) {
		do {
			x = rand() % (WIDTH_CONSOLE - 1) + 1;
			y = rand() % (HEIGH_CONSOLE - 1) + 1;
		} while (IsValid(x, y) == false);
		food[i] = { x,y };
	}
}
void ResetData() {
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1; FOOD_INDEX = 0, WIDTH_CONSOLE = 70,
		HEIGH_CONSOLE = 20, SIZE_SNAKE = 6; Score = 0;
	snake[0] = { 10, 5 }; snake[1] = { 11, 5 };
	snake[2] = { 12, 5 }; snake[3] = { 13, 5 };
	snake[4] = { 14, 5 }; snake[5] = { 15, 5 };
	GenerateFood();
}
//Color func
HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
void Color(int color_code) {
	SetConsoleTextAttribute(out, color_code);
}
//change fontsize
void fontsize(int a, int b) {
	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
	lpConsoleCurrentFontEx->dwFontSize.X = a;
	lpConsoleCurrentFontEx->dwFontSize.Y = b;
	SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
}
// hidecur
void ShowCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConCurInf;

	ConCurInf.dwSize = 10;
	ConCurInf.bVisible = CursorVisibility;

	SetConsoleCursorInfo(handle, &ConCurInf);
}
void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0) {
	GotoXY(x, y);
	Color(9);
	char aa = char(219);
	cout << char(201);
	for (int i = 1; i < width; i++) cout << char(205);
	cout << char(187);
	GotoXY(x, height + y); cout << char(200);
	for (int i = 1; i < width; i++) cout << char(205);
	cout << char(188);
	for (int i = y + 1; i < height + y; i++) {
		GotoXY(x, i); cout << char(186);
		GotoXY(x + width, i); cout << char(186);
	}
	Color(7);
	GotoXY(curPosX, curPosY);
}
void StartGame() {
	system("cls");
	ResetData();
	fontsize(16, 16);
	DrawBoard(75, 0, 15, 15);
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
	GotoXY(77, 1);
	cout << "Score:" << Score;
	ShowCur(0);
	STATE = 1;
}
void ExitGame(HANDLE t) {
	system("cls");
	/*TerminateThread(t, 0);*/
	//threadrun = 0;
	_endthreadex(0);
}
void PauseGame(HANDLE t) {
	SuspendThread(t);
}
void Eat() {
	GotoXY(83, 1);
	Score++;
	cout << Score;
	snake[SIZE_SNAKE] = food[FOOD_INDEX];
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1)
	{
		FOOD_INDEX = 0;
		SIZE_SNAKE = 6;
		if (SPEED == MAX_SPEED) SPEED = 1;
		else SPEED++;
		GenerateFood();
	}
	else {
		FOOD_INDEX++;
		SIZE_SNAKE++;
	}
}
void ProcessDead() {
	STATE = 0;
	GotoXY(0, HEIGH_CONSOLE + 2);
	printf("Dead, type y to continue or anykey to exit");
}
void DrawSnakeAndFood(char str) {
	GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	printf("%c", str);
	for (int i = 0; i < SIZE_SNAKE; i++) {
		GotoXY(snake[i].x, snake[i].y);
		printf("%c", str);
	}
}
void MoveRight() {
	if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE) {
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
}void MoveLeft() {
	if (snake[SIZE_SNAKE - 1].x - 1 == 0) {
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
void MoveDown() {
	if (snake[SIZE_SNAKE - 1].y + 1 == HEIGH_CONSOLE) {
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
void MoveUp() {
	if (snake[SIZE_SNAKE - 1].y - 1 == 0) {
		ProcessDead();
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
void ThreadFunc() {
	while (1) {
		if (STATE == 1)
		{
			DrawSnakeAndFood(' ');
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
			DrawSnakeAndFood('0');
			Sleep(100 / SPEED);
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
void SaveData(char name[20]) {
	ofstream Data;
	Data.open(name);
	Data << SIZE_SNAKE << " " << FOOD_INDEX << " " << SPEED << " " << Score << " ";
	for (int i = 0; i < SIZE_SNAKE; i++) Data << snake[i];
	Data << food[FOOD_INDEX];
	Data.close();
}
void LoadData(char name[20]) {
	ifstream Data;
	Data.open(name);
	Data >> SIZE_SNAKE >> FOOD_INDEX >> SPEED >> Score;
	for (int i = 0; i < SIZE_SNAKE; i++) Data >> snake[i];
	Data >> food[FOOD_INDEX];
	Data.close();
}
void ProcessLoad() {
	//std::string path=
}
void ProcessSave() {
	char name[20];
	GotoXY(0, HEIGH_CONSOLE + 2);
	cout << "Enter name ";
	//cin.ignore();
	//_getch();
	fflush(stdin);
	cin.getline(name, 20);
	cout << name;
	strcat_s(name, ".txt");
	SaveData(name);
}
void main() {
	int temp;
	FixconsoleWindow();
	StartGame();
	thread t1(ThreadFunc);
	HANDLE handle_t1 = t1.native_handle();
	while (1) {
		temp = toupper(_getch());
		if (STATE == 1) {
			if (temp == 'P') {
				PauseGame(handle_t1);
			}
			else if (temp == 'L') {
				PauseGame(handle_t1);
				ProcessSave();
				ResumeThread(handle_t1);
			}
			else if (temp == 'T') {
			}
			else if (temp == 27) {
				ExitGame(handle_t1);
				return;
			}
			else {
				ResumeThread(handle_t1);
				if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S'))
				{
					if (temp == 'D') CHAR_LOCK = 'A';
					else if (temp == 'W') CHAR_LOCK = 'S';
					else if (temp == 'S') CHAR_LOCK = 'W';
					else CHAR_LOCK = 'D';
					MOVING = temp;
				}
			}
		}
		else {
			if (temp == 'Y') StartGame();
			else {
				ExitGame(handle_t1);
				return;
			}
		}
	}
}
