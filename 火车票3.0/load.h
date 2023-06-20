#pragma once
#ifndef LOAD_H
#define LOAD_H

#include "iostream"
#include <fstream>
#include <time.h>
#include <graphics.h>
#include <conio.h>
#include <string.h>
#include <regex>// ʹ����������ʽ

#include"directory.h"

#define FILENAME "user.txt" // �洢�û���Ϣ���ļ���
struct User {
	string username;// �˺�
	string password;// ����
};
bool isPasswordValid(const string& password); // �ж������ʽ
bool isUserExist(User user); // �ж��û��Ƿ��Ѵ���
bool isUserValid(User user);// �ж��û��������Ƿ���ȷ
void registerUser(User user);// �û�ע��
bool loginUser(User user); // �û���¼
void drawLoadWindow();// ������Ļ
void loadWindow();// ע���¼����

#endif // !LOAD_H
