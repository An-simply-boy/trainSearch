#include "iostream"
#include <fstream>			
#include <time.h>			// 用到了时间结构体
#include<graphics.h>		// easyx图形库
#include <windows.h>		// 关闭控制台窗口

#include "load.h"
#include "directory.h"

using namespace std;

int jumpInterfact = 1;// 界面跳转索引
bool isRun = true; // 判断程序是否停止
extern std::condition_variable cv;  // 条件变量，用于线程等待
extern bool ready; // 标志位，表示是否可以启动子线程
extern std::mutex m;	// 子线程互斥量

// 关闭控制台窗口
void HideWindow() {
	HWND hwnd = GetForegroundWindow();
	if (hwnd) {
		ShowWindow(hwnd, SW_HIDE);
	}
}

int main() {
	HideWindow();// 关闭生成的控制台窗口
	FreeConsole();// 阻止进程被创建时默认分配窗口的行为
	initgraph(960, 540);//创建登录窗口

	int seconds = 5; // 设置定时器的时间5秒
	std::thread timerThread(timerFunction, seconds);// 创建并开始定时器线程

	while (isRun) {
		// 根据跳转的索引选择调用的界面函数
		switch (jumpInterfact)
		{
		case 1:
			loadWindow();// 调用登录界面函数
			break;
		case 2:
			directoryWindow();// 目录界面
			break;
		case 3:
			addPeopleWindow();// 信息录入界面
			break;
		case 4:
			peopleListWindow();// 信息管理界面
			break;
		case 5:
			SearchWindow();// 高铁票查询界面
			break;
		case 6:
			ticketBookWindow();// 订票界面
			break;
		case 7:
			ticketMSGWindow();// 订单管理界面
			break;
		case 8:
			ticketShow();// 电子票据查询界面
			break;
		default:
			break;
		}
	}

	// 再次启动子线程以彻底关闭子线程
	{
		std::lock_guard<std::mutex> lk(m); // 获取互斥锁
		ready = true; // 将标志位设为 true，表示可以启动子线程
		cv.notify_one(); // 发出信号，唤醒子线程
	}
	timerThread.join(); // 分离定时器线程

	return 0;
}
