#pragma once
#ifndef SEARCH_H
#define SEARCH_H

#include <iostream>
#include <fstream>
#include <time.h>			// 时间结构体
#include <graphics.h>		// easyx图形库
#include <conio.h>			// sleep函数
#include <string.h>			// 字符串头文件
#include <windows.h>		// 关闭控制台窗口
#include <random>
#include <thread>	// 多线程
#include <condition_variable>	// 包含了mutex头文件

#include "json/json.h"		// jsoncpp头文件
#include "people.h"
#include "directory.h"
#include "ticketBook.h"
#pragma comment(lib,"Imm32.lib")

using namespace std;

string Utf8ToAnsi(const std::string& utf8Str);
void clickAnimate(int x,int y, void (*fun)());// 鼠标点击动画
void randomVoteNum();// 初始化赋予随机票数
void match(string date, string endplace);// 匹配高铁票数据
double timeSubtract(string time1, string time2);// 计算时间差
void textCenter(const char* title, int x, int y, int hei, int wid);// 文字居中输出
void bubble_sort(Json::Value& arr, bool UD, int choose);// 冒泡排序

void drawSearchWindow();// 绘制屏幕
char chTurn(ExMessage mmsg);// 字符编码的转化
void GetIMEString(HWND hWnd, string& str); // 判断中文输入
void inputOutput(string& toWrite, void(*drawWindow)(), int x1, int y1, int x2, int y2);// 键盘交互功能
void SearchWindow();// 高铁票查询窗口

void timerFunction(int seconds);// 定时器

#endif // !SEARCH_H
