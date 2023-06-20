#pragma once
#ifndef SEARCH_H
#define SEARCH_H

#include <iostream>
#include <fstream>
#include <time.h>			// ʱ��ṹ��
#include <graphics.h>		// easyxͼ�ο�
#include <conio.h>			// sleep����
#include <string.h>			// �ַ���ͷ�ļ�
#include <windows.h>		// �رտ���̨����
#include <random>
#include <thread>	// ���߳�
#include <condition_variable>	// ������mutexͷ�ļ�

#include "json/json.h"		// jsoncppͷ�ļ�
#include "people.h"
#include "directory.h"
#include "ticketBook.h"
#pragma comment(lib,"Imm32.lib")

using namespace std;

string Utf8ToAnsi(const std::string& utf8Str);
void clickAnimate(int x,int y, void (*fun)());// ���������
void randomVoteNum();// ��ʼ���������Ʊ��
void match(string date, string endplace);// ƥ�����Ʊ����
double timeSubtract(string time1, string time2);// ����ʱ���
void textCenter(const char* title, int x, int y, int hei, int wid);// ���־������
void bubble_sort(Json::Value& arr, bool UD, int choose);// ð������

void drawSearchWindow();// ������Ļ
char chTurn(ExMessage mmsg);// �ַ������ת��
void GetIMEString(HWND hWnd, string& str); // �ж���������
void inputOutput(string& toWrite, void(*drawWindow)(), int x1, int y1, int x2, int y2);// ���̽�������
void SearchWindow();// ����Ʊ��ѯ����

void timerFunction(int seconds);// ��ʱ��

#endif // !SEARCH_H
