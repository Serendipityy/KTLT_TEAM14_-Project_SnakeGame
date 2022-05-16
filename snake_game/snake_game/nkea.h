#pragma once
#pragma comment(lib, "Winmm.lib")
#pragma warning(disable: 4996)
#include <windows.h> //for HWND
#include <iostream>
#include <time.h> //for srand(time(NULL));
#include <conio.h> //for getch()
#include <thread> //for thread
#include "dirent.h"
#include <vector>
#include <iomanip>
#include <mutex>
#include <string>
#include <fstream> //for read file
using namespace std;

//Cố định màn hình Console
void FixconsoleWindow();

// Chỉnh màu chữ 
void setTextColor(int color);

// Dịch chuyển
void GotoXY(int x, int y);

// Thay đổi font chữ
void fontsize(int a, int b);

// Xóa con trỏ nếu ShowCur(0);
void ShowCur(bool CursorVisibility);

//Vẽ khung game
void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0);

//Drawboard cho menu
void Draw_Board(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0);

//Vẽ chữ SNAKE GAME trong menu
void DrawSnakeGame_Menu(char Text, int x, int y);

//Vẽ con rắn 
void DrawSnake_Game();

//Vẽ chứ SNAKE
void DrawSnake_Text();

//Vẽ chữ TEAM 14
void DrawTeam14();

//Vẽ chữ GAME OVER
void DrawGameOver();

//Vẽ hướng dẫn chơi
void DrawHowToPlay();

//Tính toán vị trí in menu_list
int pos_calc(string s);

//Vẽ intro con rắn
void Draw_Newgame_intro();

//Vẽ bảng menu save game load setiing..
void Draw_menu_board();

//changelog
void ThreadAnimation();

//Vẽ màn hình chờ game over
void draw_gameover();

//Vẽ nhện
bool DrawSpider();

//Khởi chạy rắn
void Run();

//Vẽ màn chơi
void DrawMapLv(int level_index);

//Vẽ màn hình chơi và khởi tạo dữ liệu ban đầu cho game
void Start();

//Vẽ rắn và thức ăn
void DrawSnakeAndFood(char str);

//Xóa rắn và thức ăn
void ClearSnakeAndFood(char str);

//Kết thúc game
void Draw_endgame();
