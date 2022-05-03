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
//constants
#define MAX_SIZE_SNAKE 10
#define MAX_SIZE_FOOD 8
#define MAX_SIZE_OBSTACLE 8
#define MAX_SIZE_GATE 5
#define MAX_SPEED 3
//GLOBAL variables
POINT snake[40];
POINT food[8];
POINT obstacle[8];
POINT gate[5];
int MSSV[40] = { 2,1,1,2,0,4,4,9,
2,1,1,2,0,4,5,8,
2,1,1,2,0,4,6,4,
2,1,1,2,0,4,7,5,
2,1,1,2,0,4,8,5 };
int CHAR_LOCK;
int MOVING;
int SPEED;
int HEIGH_CONSOLE, WIDTH_CONSOLE;
int FOOD_INDEX;
int OBSTACLE_INDEX;//chi so chuong ngai vat
int GATE_INDEX;
int SIZE_SNAKE;
int STATE;
int LEVEL;
int Score;
int threadrun = 1;
// Cố định màn hình Console
void FixconsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void setTextColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void BackGroundColor(WORD color)
{
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	color <<= 4;
	wAttributes &= 0xff0f;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

// Dịch chuyển
void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// Kiểm tra vị trí của thức ăn
bool IsValid(int x, int y) {
	for (int i = 0; i < SIZE_SNAKE; i++)
		if (snake[i].x == x && snake[i].y == y)
			return false;
	return true;
}
//Check if obstacle's position is food's position or not 
bool IsValidObstacle(int x, int y, int width, int height) {
	if (food[FOOD_INDEX].x >= x && food[FOOD_INDEX].x <= x + width)
		return false;
	return true;
}
void GenerateFood() {
	int x, y;
	srand(time(NULL));
	for (int i = 0; i < MAX_SIZE_FOOD; i++) {
		do {
			x = 8 + rand() % (WIDTH_CONSOLE - 1) + 1; //I don't know why it's calculated like this
			y = rand() % (HEIGH_CONSOLE - 1) + 1;
		} while (IsValid(x, y) == false && IsValidObstacle(obstacle[OBSTACLE_INDEX].x, obstacle[OBSTACLE_INDEX].y, 4, 4) == false);
		food[i] = { x,y };
	}
}
void GenerateObstacle(int width, int height) {
	int x, y;
	srand(time(NULL));
	for (int i = 0; i < MAX_SIZE_OBSTACLE; i++) {
		do {
			x = 8 + rand() % (WIDTH_CONSOLE - 1 - width) + 1;
			y = rand() % (HEIGH_CONSOLE - 1 - height) + 1;
		} while (IsValid(x, y) == false && IsValidObstacle(obstacle[OBSTACLE_INDEX].x, obstacle[OBSTACLE_INDEX].y, 4, 4) == false);
		obstacle[i] = { x,y };
	}
}
void ResetData() {
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1; FOOD_INDEX = 0, OBSTACLE_INDEX = 0, GATE_INDEX = 0,
		WIDTH_CONSOLE = 70, HEIGH_CONSOLE = 20, SIZE_SNAKE = 6; Score = 0, LEVEL = 1;
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
	for (int i = 0; i <= width; i++) cout << char(219);
	GotoXY(x, height + y);
	for (int i = 0; i <= width; i++) cout << char(219);
	for (int i = y + 1; i < height + y; i++) {
		GotoXY(x, i); cout << char(219);
		GotoXY(x + width, i); cout << char(219);
	}
	Color(7);
	GotoXY(curPosX, curPosY);
}
void DrawTeam14() {
	setTextColor(14);
	GotoXY(82, 1);
	cout << "______";
	GotoXY(82, 2);
	cout << "|_   _|";
	GotoXY(84, 3);
	cout << "| |";
	GotoXY(84, 4);
	cout << "|_|";
	GotoXY(89, 1);
	cout << "_____";
	GotoXY(89, 2);
	cout << "| __|";
	GotoXY(89, 3);
	cout << "| `_";
	GotoXY(89, 4);
	cout << "|___,";
	GotoXY(96, 1);
	cout << "__";
	GotoXY(96, 2);
	cout << "/_\\";
	GotoXY(95, 3);
	cout << "/__ \\";
	GotoXY(94, 4);
	cout << "/_/ \\_\\";
	GotoXY(101, 1);
	cout << "___   ___";
	GotoXY(101, 2);
	cout << "|  \\_/  |";
	GotoXY(101, 3);
	cout << "| |\\_/| |";
	GotoXY(101, 4);
	cout << "|_|   |_|";
	GotoXY(92, 6);
	cout << "___";
	GotoXY(92, 7);
	cout << "| |";
	GotoXY(92, 8);
	cout << "| |";
	GotoXY(92, 9);
	cout << "|_|";
	GotoXY(97, 6);
	cout << "___";
	GotoXY(97, 7);
	cout << "/  |";
	GotoXY(96, 8);
	cout << "/_| |";
	GotoXY(98, 9);
	cout << "|_|";
}
void DrawSnakeGame() {
	GotoXY(17, HEIGH_CONSOLE + 5);
	cout << "__,";
	GotoXY(17, HEIGH_CONSOLE + 6);
	cout << "|_,";
	GotoXY(17, HEIGH_CONSOLE + 7);
	cout << "__/";
	GotoXY(21, HEIGH_CONSOLE + 6);
	cout << "||\\ |";
	GotoXY(21, HEIGH_CONSOLE + 7);
	cout << "|| \\|";
	GotoXY(28, HEIGH_CONSOLE + 5);
	cout << "__";
	GotoXY(28, HEIGH_CONSOLE + 6);
	cout << "/-\\";
	GotoXY(27, HEIGH_CONSOLE + 7);
	cout << "//`\\\\";
	GotoXY(33, HEIGH_CONSOLE + 6);
	cout << "||//";
	GotoXY(33, HEIGH_CONSOLE + 7);
	cout << "||\\\\";
	GotoXY(38, HEIGH_CONSOLE + 5);
	cout << "___,";
	GotoXY(38, HEIGH_CONSOLE + 6);
	cout << "|__";
	GotoXY(38, HEIGH_CONSOLE + 7);
	cout << "|__";
	GotoXY(45, HEIGH_CONSOLE + 5);
	cout << "____,";
	GotoXY(45, HEIGH_CONSOLE + 6);
	cout << "|| __";
	GotoXY(45, HEIGH_CONSOLE + 7);
	cout << "||_||";
	GotoXY(52, HEIGH_CONSOLE + 5);
	cout << "__";
	GotoXY(52, HEIGH_CONSOLE + 6);
	cout << "/-\\";
	GotoXY(51, HEIGH_CONSOLE + 7);
	cout << "//`\\\\";
	GotoXY(57, HEIGH_CONSOLE + 6);
	cout << "||\\/||";
	GotoXY(57, HEIGH_CONSOLE + 7);
	cout << "|| '||";
	GotoXY(64, HEIGH_CONSOLE + 5);
	cout << "___,";
	GotoXY(64, HEIGH_CONSOLE + 6);
	cout << "|__";
	GotoXY(64, HEIGH_CONSOLE + 7);
	cout << "|__";
}
void StartGame() {
	system("cls");
	ResetData();
	fontsize(16, 16);
	DrawBoard(81, 0, 30, HEIGH_CONSOLE);
	DrawBoard(8, HEIGH_CONSOLE + 4, WIDTH_CONSOLE, 5);
	DrawBoard(81, HEIGH_CONSOLE + 2, 32, 7);
	DrawBoard(8, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
	DrawTeam14();
	DrawSnakeGame();
	GotoXY(87, HEIGH_CONSOLE - 5);
	cout << "Score:" << Score;
	GotoXY(87, HEIGH_CONSOLE - 3);
	cout << "LEVEL:" << LEVEL;
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
void DrawObstacle(int x, int y, int width, int height) {
	GotoXY(x, y);
	for (int i = x; i < width + x; i++) {
		for (int j = y; j <= height + y; j++) {
			GotoXY(i, j); cout << char(219);
		}
	}
}
void ClearObstacle(int x, int y, int width, int height) {
	GotoXY(x, y);
	for (int i = x; i < width + x; i++) {
		for (int j = y; j <= height + y; j++) {
			GotoXY(i, j); printf("%c", ' ');
		}
	}
}
void GenerateGate(int width, int height) {
	int x, y;
	srand(time(NULL));
	for (int i = 0; i < MAX_SIZE_GATE; i++) {
		do {
			x = 8 + rand() % (WIDTH_CONSOLE - 1 - width) + 1;
			y = rand() % (HEIGH_CONSOLE - 2 - height) + 1;
		} while (IsValid(x, y) == false);
		gate[i] = { x,y };
	}
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
void Level(int level_index) {
	if (level_index == 1) {
		//Draw Obstacle
		GenerateObstacle(4, 4);
		DrawObstacle(obstacle[OBSTACLE_INDEX].x, obstacle[OBSTACLE_INDEX].y, 4, 4);
		OBSTACLE_INDEX++;
	}
	else {

	}
}
void ProcessDead() {
	STATE = 0;
	//PlaySound(TEXT("oh no sound.wav"), NULL, SND_ASYNC);
	GotoXY(0, HEIGH_CONSOLE + 2);
	printf("Dead, type y to continue or anykey to exit");
}
void Eat() {
	//PlaySound(TEXT("eat sound.wav"), NULL, SND_ASYNC);
	GotoXY(93, HEIGH_CONSOLE - 5);
	Score++;
	cout << Score;
	snake[SIZE_SNAKE] = food[FOOD_INDEX];
	GenerateFood();
	//Clear obstacle after eat food
	ClearObstacle(obstacle[OBSTACLE_INDEX - 1].x, obstacle[OBSTACLE_INDEX - 1].y, 4, 4);
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1)
	{
		FOOD_INDEX = 0;
		GotoXY(83, HEIGH_CONSOLE - 3);
		LEVEL++;
		cout << LEVEL;
		if (SPEED == MAX_SPEED) SPEED = 1;
		else SPEED++;
		//Create gate 
		GenerateGate(2, 2);
		DrawGate(gate[GATE_INDEX].x, gate[GATE_INDEX].y, 2, 2);
		GATE_INDEX++;
	}
	else {
		Level(LEVEL);
		GenerateGate(2, 2);
		DrawGate(gate[GATE_INDEX].x, gate[GATE_INDEX].y, 2, 2);
		FOOD_INDEX++;
		SIZE_SNAKE++;
	}
}
void setFontSize(int FontSize)
{
	CONSOLE_FONT_INFOEX info = { 0 };
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = FontSize; // leave X as zero
	info.FontWeight = FW_NORMAL;
	wcscpy_s(info.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}
void ClearSnakeAndFood(char str) {
	GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	printf("%c", str);
	for (int i = 0; i < SIZE_SNAKE; i++) {
		GotoXY(snake[i].x, snake[i].y);
		printf("%c", str);
	}
}
void DrawSnakeAndFood(char str) {
	GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	printf("%c", str);
	for (int i = 0; i < SIZE_SNAKE; i++) {
		GotoXY(snake[i].x, snake[i].y);
		printf("%d", MSSV[SIZE_SNAKE - 1 - i]);
	}
}
// Rắn chạm thân
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
	if (x >= obstacle[OBSTACLE_INDEX - 1].x && x <= obstacle[OBSTACLE_INDEX - 1].x + width
		&& y >= obstacle[OBSTACLE_INDEX - 1].y && y <= obstacle[OBSTACLE_INDEX - 1].y + height)
	{
		return true;
	}
	return false;
}
bool SnakeTouchGate(int x, int y, int width, int height) {
	if (x >= gate[GATE_INDEX].x && x <= gate[GATE_INDEX].x + width
		&& y == gate[GATE_INDEX].y)
	{
		return true;
	}
	if ((x == gate[GATE_INDEX].x || x + width == gate[GATE_INDEX].x)
		&& (y >= gate[GATE_INDEX].y && y <= gate[GATE_INDEX].y + height))
	{
		return true;
	}
	return false;
}
void MoveRight() {
	if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE + 8 || (SnakeTouchBody(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y) == true)
		|| SnakeTouchObstacle(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y, 4, 4) == true
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
void MoveLeft() {
	if (snake[SIZE_SNAKE - 1].x - 1 == 8 || (SnakeTouchBody(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y) == true)
		|| SnakeTouchObstacle(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y, 4, 4) == true
		|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y, 2, 2) == true) {
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
	if (snake[SIZE_SNAKE - 1].y + 1 == HEIGH_CONSOLE || (SnakeTouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1) == true)
		|| SnakeTouchObstacle(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y, 4, 4) == true
		|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y, 2, 2) == true) {
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
	if (snake[SIZE_SNAKE - 1].y - 1 == 0 || (SnakeTouchBody(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y - 1) == true)
		|| SnakeTouchObstacle(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y, 4, 4) == true
		|| SnakeTouchGate(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y, 2, 2) == true) {
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
			Sleep(150 / SPEED);
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
	cout << "Enter name: ";
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