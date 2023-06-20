#include "iostream"
#include <fstream>			
#include <time.h>			// �õ���ʱ��ṹ��
#include<graphics.h>		// easyxͼ�ο�
#include <windows.h>		// �رտ���̨����

#include "load.h"
#include "directory.h"

using namespace std;

int jumpInterfact = 1;// ������ת����
bool isRun = true; // �жϳ����Ƿ�ֹͣ
extern std::condition_variable cv;  // ���������������̵߳ȴ�
extern bool ready; // ��־λ����ʾ�Ƿ�����������߳�
extern std::mutex m;	// ���̻߳�����

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

	int seconds = 5; // ���ö�ʱ����ʱ��5��
	std::thread timerThread(timerFunction, seconds);// ��������ʼ��ʱ���߳�

	while (isRun) {
		// ������ת������ѡ����õĽ��溯��
		switch (jumpInterfact)
		{
		case 1:
			loadWindow();// ���õ�¼���溯��
			break;
		case 2:
			directoryWindow();// Ŀ¼����
			break;
		case 3:
			addPeopleWindow();// ��Ϣ¼�����
			break;
		case 4:
			peopleListWindow();// ��Ϣ�������
			break;
		case 5:
			SearchWindow();// ����Ʊ��ѯ����
			break;
		case 6:
			ticketBookWindow();// ��Ʊ����
			break;
		case 7:
			ticketMSGWindow();// �����������
			break;
		case 8:
			ticketShow();// ����Ʊ�ݲ�ѯ����
			break;
		default:
			break;
		}
	}

	// �ٴ��������߳��Գ��׹ر����߳�
	{
		std::lock_guard<std::mutex> lk(m); // ��ȡ������
		ready = true; // ����־λ��Ϊ true����ʾ�����������߳�
		cv.notify_one(); // �����źţ��������߳�
	}
	timerThread.join(); // ���붨ʱ���߳�

	return 0;
}
