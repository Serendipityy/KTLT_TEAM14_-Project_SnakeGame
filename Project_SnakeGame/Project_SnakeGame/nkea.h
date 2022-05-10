#pragma once
//#include <mmsystem.h>
#include <windows.h> //for HWND
#pragma comment(lib, "Winmm.lib")
#include <iostream>
#include <time.h> //for srand(time(NULL));
#include <conio.h>//for getch()
#include <thread>
#include "dirent.h"
#include <vector>
#include <iomanip>
#include <mutex>
#include <string>
#include <fstream>//for read file
using namespace std;
#pragma warning(disable: 4996)
//Cố định màn hình Console
void FixconsoleWindow();
// Chỉnh màu chữ 
void setTextColor(int color);
// Chỉnh màu background
void BackGroundColor(WORD color);
// Dịch chuyển
void GotoXY(int x, int y);
// Check if obstacle's position is food's position or not
//bool IsValidObstacle(int x, int y, int width, int height);
// change fontsize - CAN HAY KHONG
void fontsize(int a, int b);
// xoa con tro neu ShowCur(0);
void ShowCur(bool CursorVisibility);
void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0);
//drawboard cho menu
void Draw_Board(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0);

void DrawSnakeGame_Menu(char Text, int x, int y);
void DrawSnake_Game();
void DrawSnake_Text();
void DrawTeam14();
void DrawGameOver();
void DrawHowToPlay();
//tinh toan vi tri in menu_list
int pos_calc(string s);
//ve intro con ran ngau vcl
void Draw_Newgame_intro();
//ve bang menu save game load setiing..
void Draw_menu_board();
//changelog
void ThreadAnimation();
void draw_gameover();