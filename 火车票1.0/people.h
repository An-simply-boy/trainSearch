#pragma once
#ifndef PEOPLE_H
#define PEOPLE_H

#include "directory.h"
#include <regex>// 使用了正则表达式

void drawPeopleListWindow();// 绘制信息录入界面
void peopleDelete();// 信息删除
void peopleListWindow();// 信息录入界面

void drawAddPeopleWindow();// 绘制信息录入界面
void addPeopleWindow();// 信息录入界面

#endif // !PEOPLE_H
