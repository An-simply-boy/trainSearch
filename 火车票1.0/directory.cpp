// 目录界面 索引号为2
#include"directory.h"

IMAGE directoryBG;
extern int jumpInterfact;// 界面跳转索引

void drawDirectoryWindow() {
	BeginBatchDraw();// 暂停绘图
	cleardevice();// 清屏
	putimage(0, 0, &directoryBG);

	FlushBatchDraw();// 将未完成的绘图任务完成
	EndBatchDraw();// 结束绘图缓冲
}

void directoryWindow() {
	loadimage(&directoryBG, "picture/catalogue.jpg");
	drawDirectoryWindow();

	ExMessage msg;
	while (1)
	{
		msg = getmessage(EX_MOUSE);// 获取鼠标消息，堵塞语句
		int x = msg.x;
		int y = msg.y;
		if (msg.lbutton == 1) {// 判断鼠标左键是否按下
			clickAnimate(x, y, drawDirectoryWindow);
			if (x >= 520 && x <= 733 && y >= 222 && y <= 287) { // 点击信息录入按钮
				jumpInterfact = 3;
				break;
			}
			else if (x >= 734 && x <= 946 && y >= 222 && y <= 287) {// 点击信息管理按钮
				jumpInterfact = 4;
				break;
			}
			else if (x >= 520 && x <= 733 && y >= 288 && y <= 352) {// 点击车票查询按钮
				jumpInterfact = 5;
				break;
			}
			else if (x >= 734 && x <= 946 && y >= 288 && y <= 352) {// 点击订单管理按钮
				jumpInterfact = 7;
				break;
			}
		}
	}

	if (jumpInterfact == 5) {
		SearchWindow();// 跳转至车票查询界面
	}
	else if (jumpInterfact == 7) {
		ticketMSGWindow();// 跳转至订单管理界面
	}
	else if (jumpInterfact == 4) {
		peopleListWindow();// 跳转至信息管理窗口
	}
	else if (jumpInterfact == 3) {
		addPeopleWindow();// 跳转至信息录入窗口
	}
}