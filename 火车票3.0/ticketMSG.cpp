// 订单管理界面 索引号7
#include"ticketMSG.h"

IMAGE ticketMSGBG;// 背景图片
IMAGE ticketShowBG;// 订单界面
Json::Value people_ticketMsg;// 储存订单信息数据
int index;// 订单索引号
extern int jumpInterfact;// 页面索引
ExMessage msg;// 消息结构体 储存消息

// 绘制订单管理界面窗口
void drawTicketMSGWindow() {
	BeginBatchDraw();// 暂停绘图
	cleardevice();
	putimage(0, 0, &ticketMSGBG);// 绘制背景图片
	settextstyle(24, 0, "Consolas");
	settextcolor(BLACK);
	for (unsigned int i = 0; i < people_ticketMsg["man"].size(); i++) {// 循环渲染数据
		textCenter(people_ticketMsg["trainNum"].asString().c_str(), 33, 279 + i * 87, 87, 110);
		textCenter("东莞", 141, 279 + i * 87, 87, 109);
		textCenter(people_ticketMsg["endPlaceCname"].asString().c_str(), 249, 279 + i * 87, 87, 92);
		textCenter(people_ticketMsg["startTime"].asString().c_str(), 339, 279 + i * 87, 87, 102);
		textCenter(people_ticketMsg["endTime"].asString().c_str(), 442, 279 + i * 87, 87, 121);
		textCenter(people_ticketMsg["price"].asString().c_str(), 562, 279 + i * 87, 87, 72);
		textCenter(people_ticketMsg["man"][i]["name"].asString().c_str(), 632, 279 + i * 87, 87, 129);
	}
	FlushBatchDraw();// 将未完成的绘图任务完成
	EndBatchDraw();// 结束绘图缓冲
}

//订单管理界面窗口
void ticketMSGWindow() {
	loadimage(&ticketMSGBG, "picture/ticketMSG.png");
	//打开json文件 读取数据
	std::ifstream file("beChoose.json");
	file >> people_ticketMsg;// 从文件中读取数据到json对象中
	file.close();// 关闭文件
	drawTicketMSGWindow();// 绘图

	while (1)
	{
		msg = getmessage(EX_MOUSE);// 获取鼠标消息，堵塞语句
		int x = msg.x;
		int y = msg.y;
		if (msg.lbutton == 1) {// 判断鼠标左键是否按下
			clickAnimate(x, y, drawTicketMSGWindow);
			if (x > 789 && x < 946 && y > 290 && y < 368) {// 查看第一张电子票据
				index = 0;// 第一张票索引
				if (people_ticketMsg["man"].size() >= index + 1) {
					jumpInterfact = 8;// 跳转至窗口-查看电子票据
					break;
				}
			}
			else if (x > 789 && x < 946 && y > 375 && y < 450) {// 查看第二张电子票据
				index = 1;// 第二张票索引
				if (people_ticketMsg["man"].size() >= index + 1) {
					jumpInterfact = 8;// 跳转至窗口-查看电子票据
					break;
				}
			}
			else if (x > 789 && x < 946 && y > 460 && y < 536) {// 查看第二张电子票据
				index = 2;// 第三张票索引
				if (people_ticketMsg["man"].size() >= index + 1) {
					jumpInterfact = 8;// 跳转至窗口-查看电子票据
					break;
				}
			}
			else if (x > 12 && x < 161 && y > 131 && y < 171) {// 返回目录
				jumpInterfact = 2;// 跳转至窗口-查看电子票据
				break;
			}
		}
	}
}

// 电子票据界面 索引为8

// 绘制电子票据窗口
void drawTicketShow() {
	BeginBatchDraw();// 暂停绘图
	cleardevice();
	putimage(0, 0, &ticketShowBG);

	settextstyle(76, 0, "Consolas");
	string time = people_ticketMsg["startTime"].asString() + "-" + people_ticketMsg["endTime"].asString();
	string price = "￥" + people_ticketMsg["price"].asString() + "元";
	textCenter("东莞", 167,102,78,153);// 出发地
	textCenter(people_ticketMsg["endPlaceCname"].asString().c_str(), 627, 102, 78, 200);// 到达地
	settextstyle(50, 0, "Consolas");
	textCenter(people_ticketMsg["trainNum"].asString().c_str(), 353, 107, 47, 186);// 车次
	settextstyle(36, 0, "Consolas");
	textCenter(time.c_str(), 337, 182, 52, 262);// 起始-结束时间
	textCenter(price.c_str(), 101, 240, 60, 195);// 价格
	textCenter(people_ticketMsg["date"].asString().c_str(), 379, 252, 50, 229);// 日期
	string lastSixNum = people_ticketMsg["man"][index]["identityCardNum"].asString().substr(11);// 身份证后六位
	textCenter(lastSixNum.c_str(), 309, 459, 76, 170);
	textCenter(people_ticketMsg["man"][index]["name"].asString().c_str(), 528, 459, 76, 269);// 姓名

	FlushBatchDraw();// 将未完成的绘图任务完成
	EndBatchDraw();// 结束绘图缓冲
}

// 电子票据窗口
void ticketShow() {
	loadimage(&ticketShowBG, "picture/ticketShow.jpg");// 加载图片
	drawTicketShow();// 绘制窗口
	while (1) {
		msg = getmessage(EX_MOUSE);// 获取鼠标消息，堵塞语句
		int x = msg.x;
		int y = msg.y;
		if (msg.lbutton == 1) {// 判断鼠标左键是否按下
			clickAnimate(x, y, drawTicketMSGWindow);
			jumpInterfact = 7;
			break;
		}
	}
}
