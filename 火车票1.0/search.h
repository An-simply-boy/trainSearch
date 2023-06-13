#pragma once
#ifndef SEARCH_H
#define SEARCH_H

#include "iostream"
#include <fstream>
#include <time.h>
#include <graphics.h>
#include <conio.h>
#include <string.h>

#include "json/json.h"
#include "people.h"
#include "directory.h"
#include "ticketBook.h"

using namespace std;

void clickAnimate(int x,int y, void (*fun)());// 鼠标点击动画
void randomVoteNum();// 初始化赋予随机票数
void match(string date, string endplace);// 匹配高铁票数据
double timeSubtract(string time1, string time2);// 计算时间差
void textCenter(const char* title, int x, int y, int hei, int wid);// 文字居中输出
void bubble_sort(Json::Value& arr, bool UD, int choose);// 冒泡排序

void drawSearchWindow();// 绘制屏幕
char chTurn(ExMessage mmsg);// 字符编码的转化
void inputOutput(string& toWrite, void(*drawWindow)());// 键盘交互功能
void SearchWindow();// 高铁票查询窗口

#endif // !SEARCH_H
