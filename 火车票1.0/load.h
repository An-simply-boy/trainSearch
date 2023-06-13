#pragma once
#ifndef LOAD_H
#define LOAD_H

#include "iostream"
#include <fstream>
#include <time.h>
#include <graphics.h>
#include <conio.h>
#include <string.h>
#include <regex>// 使用了正则表达式

#include"directory.h"

#define FILENAME "user.txt" // 存储用户信息的文件名
struct User {
	string username;// 账号
	string password;// 密码
};
bool isPasswordValid(const string& password); // 判断密码格式
bool isUserExist(User user); // 判断用户是否已存在
bool isUserValid(User user);// 判断用户名密码是否正确
void registerUser(User user);// 用户注册
bool loginUser(User user); // 用户登录
void drawLoadWindow();// 绘制屏幕
void loadWindow();// 注册登录窗口

#endif // !LOAD_H
