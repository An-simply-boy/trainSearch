#include "iostream"
#include <fstream>			
#include <time.h>			// �õ���ʱ��ṹ��
#include<graphics.h>		// easyxͼ�ο�
#include <windows.h>		// �رտ���̨����

#include "load.h"
#include "directory.h"

using namespace std;

int jumpInterfact = 1;// ������ת����

// �رտ���̨����
void HideWindow() {
	HWND hwnd = GetForegroundWindow();
	if (hwnd) {
		ShowWindow(hwnd, SW_HIDE);
	}
}

int main() {
	HideWindow();// �ر����ɵĿ���̨����
	FreeConsole();// ��ֹ���̱�����ʱĬ�Ϸ��䴰�ڵ���Ϊ
	initgraph(960, 540);//������¼����

	randomVoteNum();// ��ʼ���������Ʊ��

	loadWindow();// ���õ�¼���溯��

	return 0;
}
