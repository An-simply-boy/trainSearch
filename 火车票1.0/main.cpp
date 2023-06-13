#include "iostream"
#include <fstream>			
#include <time.h>			// 用到了时间结构体
#include<graphics.h>		// easyx图形库
#include <windows.h>		// 关闭控制台窗口

#include "load.h"
#include "directory.h"

using namespace std;

int jumpInterfact = 1;// 界面跳转索引

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

	randomVoteNum();// 初始化赋予随机票数

	loadWindow();// 调用登录界面函数

	return 0;
}
