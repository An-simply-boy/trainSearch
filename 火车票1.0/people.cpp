// 信息管理界面 索引号为4
#include "people.h"

IMAGE peopleListBG;// 背景图片
IMAGE addPeopleBG;// 背景图片
extern int jumpInterfact;// 页面跳转索引
int peopleIndex;// 信息索引
bool isModify = false;// 是否是以修改信息的方式打开函数
Json::Value peopleList(Json::arrayValue);// 储存录入的所有人的信息
Json::Value peopleWrite;// 储存需要修改（添加）的信息
string name, sex, address, mailbox, PhoneNum, IdentityCardNum;

void drawPeopleListWindow() {
	BeginBatchDraw();// 暂停绘图
	cleardevice();// 清屏
	putimage(0, 0, &peopleListBG);// 加载图片
	settextstyle(16, 0, "Consolas");
	settextcolor(BLACK);
	for (unsigned int i = 0; i < peopleList.size(); i++) {// 循环渲染数据
		textCenter(peopleList[i]["name"].asString().c_str(), 0, 318 + i * 74, 74, 58);// 姓名
		textCenter(peopleList[i]["sex"].asString().c_str(), 58, 318 + i * 74, 74, 54);// 性别
		textCenter(peopleList[i]["phoneNum"].asString().c_str(), 110, 318 + i * 74, 74, 134);// 手机号码
		textCenter(peopleList[i]["identityCardNum"].asString().c_str(), 243, 318 + i * 74, 74, 186);// 身份证号码
		textCenter(peopleList[i]["mailbox"].asString().c_str(), 427, 318 + i * 74, 74, 143);// 邮箱
		textCenter(peopleList[i]["address"].asString().c_str(), 569, 318 + i * 74, 74, 117);// 地址
	}
	FlushBatchDraw();// 将未完成的绘图任务完成
	EndBatchDraw();// 结束绘图缓冲
}
void peopleDelete() {
	peopleList.removeIndex(peopleIndex, NULL);// 删除对应下标的元素
	std::ofstream outFire("afterIsChoose.json");// 重新更新文档
	if (outFire.is_open()) {
		outFire << peopleList.toStyledString();
		outFire.close();
	}
	drawPeopleListWindow();
}
void peopleListWindow() {
	//打开json文件
	std::ifstream file("afterIsChoose.json");
	file >> peopleList;// 从文件中读取数据到json对象中
	file.close();// 关闭文件
	loadimage(&peopleListBG, "picture/peopleList.png");
	drawPeopleListWindow();// 绘制信息录入窗口

	ExMessage msg;
	while (1)
	{
		if (peekmessage(&msg, EX_MOUSE, true)) {// 获取鼠标消息，非堵塞语句
			int x = msg.x;
			int y = msg.y;
			if (msg.lbutton == 1) {// 判断鼠标左键是否按下
				clickAnimate(x, y, drawPeopleListWindow);// 点击动画
				if (x >= 0 && x < 174 && y > 168 && y < 212) {// 返回目录界面
					jumpInterfact = 2;
					break;
				}
				else if (x >= 739 && x < 934 && y > 211 && y < 267) {// 点击添加按钮
					jumpInterfact = 3;
					break;
				}
				else if (x >= 735 && x < 838 && y > 322 && y < 389) {// 第一个修改按钮
					isModify = true;
					peopleIndex = 0;
					jumpInterfact = 3;
					break;
				}
				else if (x >= 839 && x < 940 && y > 322 && y < 389) {// 第一个删除按钮
					peopleIndex = 0;
					peopleDelete();
				}
				else if (x >= 735 && x < 838 && y > 397 && y < 464) {// 第二个修改按钮
					isModify = true;
					peopleIndex = 1;
					jumpInterfact = 3;
					break;
				}
				else if (x >= 839 && x < 940 && y > 397 && y < 464) {// 第二个删除按钮
					peopleIndex = 1;
					peopleDelete();
				}
				else if (x >= 735 && x < 838 && y > 469 && y < 535) {// 第三个修改按钮
					isModify = true;
					peopleIndex = 2;
					jumpInterfact = 3;
					break;
				}
				else if (x >= 839 && x < 940 && y > 469 && y < 535) {// 第三个删除按钮
					peopleIndex = 2;
					peopleDelete();
				}
			}
		}
	}
	if (jumpInterfact == 2) {// 返回目录界面
		directoryWindow();
	}
	else if (jumpInterfact == 3) {
		addPeopleWindow();
	}
}

// 信息录入界面 索引为3

bool isPhoneNumberValid(const string& phoneNumber) {// 判断手机号码格式是否输入正确
	// 正则表达式模式
	const regex pattern(R"(^1\d{10}$)");

	// 使用正则表达式匹配
	return regex_match(phoneNumber, pattern);
}

bool isIDCardValid(const string& idCard) {// 判断身份证格式是否输入正确
	// 正则表达式模式
	const regex pattern(R"(^\d{17}[\dX]$)");

	// 使用正则表达式匹配
	return regex_match(idCard, pattern);
}

bool isEmailValid(const string& email) {
	// 正则表达式模式
	const regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

	// 使用正则表达式匹配
	return regex_match(email, pattern);
}

// 绘制信息录入界面
void drawAddPeopleWindow() {
	BeginBatchDraw();// 暂停绘图
	cleardevice();// 清屏
	putimage(0, 0, &addPeopleBG);// 加载图片

	// 设置字体的样式
	settextstyle(16, 0, "Consolas");
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	// 输出字体
	textCenter(name.c_str(), 295, 205, 37, 367);
	textCenter(sex.c_str(), 295, 254, 37, 367);
	textCenter(PhoneNum.c_str(), 295, 303, 37, 367);
	textCenter(IdentityCardNum.c_str(), 295, 352, 37, 367);
	textCenter(address.c_str(), 295, 402, 37, 367);
	textCenter(mailbox.c_str(), 295, 452, 37, 367);
	FlushBatchDraw();// 将未完成的绘图任务完成
	EndBatchDraw();// 结束绘图缓冲
}

// 信息录入界面
void addPeopleWindow() {
	loadimage(&addPeopleBG, "picture/addPeople.png");
	//打开json文件
	std::ifstream file("afterIsChoose.json");
	file >> peopleList;// 从文件中读取数据到json对象中
	file.close();// 关闭文件
	peopleWrite.clear();
	if(isModify){
		peopleWrite = peopleList[peopleIndex];// 如果是修改，则获取修改的值
	}
	// 获取值,初始化
	name = peopleWrite["name"].asString();
	sex = peopleWrite["sex"].asString();
	PhoneNum = peopleWrite["phoneNum"].asString();
	IdentityCardNum = peopleWrite["identityCardNum"].asString();
	address = peopleWrite["address"].asString();
	mailbox = peopleWrite["mailbox"].asString();
	drawAddPeopleWindow();// 绘制窗口

	ExMessage msg;// 消息结构体
	while (1)
	{
		msg = getmessage(EX_MOUSE);// 获取鼠标消息，堵塞语句
		int x = msg.x;
		int y = msg.y;
		if (msg.lbutton == 1) {// 判断鼠标左键是否按下
			clickAnimate(x, y, drawAddPeopleWindow);// 点击动画
			if (x >= 0 && x < 245 && y > 150 && y < 178) {// 点击返回目录按钮
				jumpInterfact = 2;
				break;
			}
			else if (x >= 295 && x < 662 && y > 205 && y < 242) {// 点击姓名输入框
				inputOutput(name, drawAddPeopleWindow);
			}
			else if (x >= 295 && x < 662 && y > 254 && y < 291) {// 点击性别输入框
				inputOutput(sex, drawAddPeopleWindow);
			}
			else if (x >= 295 && x < 662 && y > 303 && y < 340) {// 点击手机号码输入框
				inputOutput(PhoneNum, drawAddPeopleWindow);
			}
			else if (x >= 295 && x < 662 && y > 352 && y < 389) {// 点击身份证号码输入框
				inputOutput(IdentityCardNum, drawAddPeopleWindow);
			}
			else if (x >= 295 && x < 662 && y > 402 && y < 439) {// 点击地址输入框
				inputOutput(address, drawAddPeopleWindow);
			}
			else if (x >= 295 && x < 662 && y > 452 && y < 489) {// 点击邮箱输入框
				inputOutput(mailbox, drawAddPeopleWindow);
			}
			else if (x >= 394 && x < 540 && y > 504 && y < 536) {// 点击确定函数
				if (!name.empty() && !sex.empty() && !address.empty() && !mailbox.empty() && !IdentityCardNum.empty() && !PhoneNum.empty()) {
					settextcolor(RED);
					if (!isPhoneNumberValid(PhoneNum)) {
						outtextxy(714, 309, "手机号长度不足11位！");
					}
					if (!isIDCardValid(IdentityCardNum)) {
						outtextxy(714, 358, "身份证长度不足18位！");
					}
					if (!isEmailValid(mailbox)) {
						outtextxy(714, 460, "邮箱格式不正确！");
					}
					if (isPhoneNumberValid(PhoneNum) && isIDCardValid(IdentityCardNum) && isEmailValid(mailbox)) {
						// 赋值
						peopleWrite["name"] = name;
						peopleWrite["sex"] = sex;
						peopleWrite["address"] = address;
						peopleWrite["mailbox"] = mailbox;
						peopleWrite["identityCardNum"] = IdentityCardNum;// 将字符串转化为整形
						peopleWrite["phoneNum"] = PhoneNum;
						if (isModify) {
							peopleList[peopleIndex] = peopleWrite;// 如果是修改，更新数组
							isModify = false;
						}
						else
						{
							peopleList.append(peopleWrite);// 如果是添加，添加对象
						}
						std::ofstream outFire("afterIsChoose.json");// 重新更新文档
						if (outFire.is_open()) {
							outFire << peopleList.toStyledString();
							outFire.close();
						}
						jumpInterfact = 4;
						break;
					}

				}
				else
				{
					settextcolor(RED);
					textCenter("请输入完整的信息!!!", 576, 504, 31, 211);
				}
			}
		}
	}
	if (jumpInterfact == 2) {// 返回目录界面
		directoryWindow();
	}
	else if (jumpInterfact == 4) {// 跳转至信息管理界面
		peopleListWindow();
	}
}
