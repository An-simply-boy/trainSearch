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

void clickAnimate(int x,int y, void (*fun)());// ���������
void randomVoteNum();// ��ʼ���������Ʊ��
void match(string date, string endplace);// ƥ�����Ʊ����
double timeSubtract(string time1, string time2);// ����ʱ���
void textCenter(const char* title, int x, int y, int hei, int wid);// ���־������
void bubble_sort(Json::Value& arr, bool UD, int choose);// ð������

void drawSearchWindow();// ������Ļ
char chTurn(ExMessage mmsg);// �ַ������ת��
void inputOutput(string& toWrite, void(*drawWindow)());// ���̽�������
void SearchWindow();// ����Ʊ��ѯ����

#endif // !SEARCH_H
