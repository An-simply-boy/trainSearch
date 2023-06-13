// 车票查询界面 索引号为5
#include "search.h"

IMAGE chooseBG;// 背景图片
Json::Value searchResult(Json::arrayValue);//新建一个数组存储搜索匹配的结果
extern Json::Value result;// 选票与人的信息
string date = "2023-05-16", endplace = "GZ";// 用户要求（日期，到达地，座位类型）
int sortChoose = 0, Choose = 0;// 查询结果排序方式(出发时间，运行时长，价格)
unsigned int page = 0;// 选票索引
bool UpDown = true;// 升降序
extern int jumpInterfact;//页面索引
bool bigSmall = false;// 大小写

void clickAnimate(int x, int y, void (*fun)()) {
	for (int i = 0; i < 10; i++)
	{
		BeginBatchDraw();// 暂停绘图
		fun();
		setlinecolor(BLACK);
		circle(x, y, i);
		FlushBatchDraw();// 将未完成的绘图任务完成
		EndBatchDraw();// 结束绘图缓冲
		Sleep(10);
	}
	flushmessage();// 清除消息队列的其他消息
	fun();
}

// 初始化赋予随机票数
void randomVoteNum() {
	//打开json文件
	std::ifstream file("高铁票数据.json");
	Json::Value root;// 创建json对象
	file >> root;// 从文件中读取数据到json对象中
	file.close();// 关闭文件
	Json::Value newRoot(Json::arrayValue);
	for (auto const& obj : root) // 使用迭代器遍历数组中的所有对象
	{
		Json::Value newObj = obj; // 复制对象，使其可修改
		int voteNum = rand() % 121;// 随机生成0-120的票数
		newObj["voteNum"] = voteNum;
		newRoot.append(newObj);// 将匹配的搜索结果添加到数组里
	}	
	std::ofstream outFire("高铁票数据.json");
	if (outFire.is_open()) {
		outFire << newRoot.toStyledString();
		outFire.close();
	}
}

// 在json文件的数据中匹配结果
void match(string date, string endplace) {
	//打开json文件
	std::ifstream file("高铁票数据.json");
	Json::Value root;// 创建json对象
	file >> root;// 从文件中读取数据到json对象中
	file.close();// 关闭文件

	if (root.isArray()) // 检查JSON文件是否是一个数组
	{
		searchResult.clear();
		// 遍历匹配符合要求的对象
		for (auto const& obj : root) // 使用迭代器遍历数组中的所有对象
		{
			if (obj.isObject()) // 检查对象是否是JSON对象
			{
				if ((date == obj["date"].asString()) && (endplace == obj["endPlace"].asString())) {//匹配搜索结果
					if (searchResult.isArray()) {
						searchResult.append(obj);// 将匹配的搜索结果添加到数组里
					}
				}
			}
		}
	}
}

// 计算时间差
double timeSubtract(string t1, string t2)
{
	//将时间字符串转化为时间结构体
	std::stringstream s1(t1), s2(t2);
	struct tm time1 = { 0 }, time2 = { 0 };
	char delimiter;
	s1 >> time1.tm_hour >> delimiter >> time1.tm_min;
	s2 >> time2.tm_hour >> delimiter >> time2.tm_min;
	// 获取当前时间
	time_t now;
	time(&now);
	// 转换为本地时间结构体
	struct tm local_time;
	localtime_s(&local_time, &now);
	// 将日期与时间结合
	time1.tm_year = local_time.tm_year;
	time1.tm_mon = local_time.tm_mon;
	time1.tm_mday = local_time.tm_mday;
	time2.tm_year = local_time.tm_year;
	time2.tm_mon = local_time.tm_mon;
	time2.tm_mday = local_time.tm_mday;
	// 将时间结构体转换为时间戳
	time_t timestamp1 = mktime(&time1);
	time_t timestamp2 = mktime(&time2);
	// 计算时间差
	double time_diff = difftime(timestamp2, timestamp1);
	return time_diff / 60;
}

// 文字居中输出
void textCenter(const char* title, int x, int y, int hei, int wid) {
	int strWidth = textwidth(title);//获取文字的宽度
	int strHeight = textheight(title);//获取文字的高度
	int strX = (wid - strWidth) / 2 + x;//实际文字的左上角坐标x的值
	int strY = (hei - strHeight) / 2 + y;//实际文字的右上角坐标y的值
	//输出文字
	setbkmode(TRANSPARENT);//设置文字的输出背景为透明
	outtextxy(strX, strY, title);//输出按钮文字
};

//冒泡排序
void bubble_sort(Json::Value& arr, bool UD, int sortChoose) {
	int n = arr.size(); // 获取数组的长度
	// 冒泡排序的基本代码实现
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			Json::Value& obj1 = arr[j];
			Json::Value& obj2 = arr[j + 1];
			string t1 = obj1["startTime"].asString();// 获取前一个json对象的起始时间
			double run1 = timeSubtract(t1, obj1["endTime"].asString());// 获取前一个对象的运行时间
			double price1 = obj1["price"].asDouble();// 获取前一个对象的价格
			string t2 = obj2["startTime"].asString();// 获取后一个json对象的起始时间
			double run2 = timeSubtract(t2, obj2["endTime"].asString());// 获取后一个对象的运行时间
			double price2 = obj2["price"].asDouble();// 获取后一个对象的价格
			switch (sortChoose)
			{
			case 0:// 出发时间
				if (UD ? timeSubtract(t1, t2) <= 0 : timeSubtract(t1, t2) > 0) { // 三元表达式判断升降序的实现
					Json::Value temp = obj1;
					obj1 = obj2;
					obj2 = temp;
				}
				break;
			case 1://运行时长
				if (UD ? run2 - run1 <= 0 : run2 - run1 > 0) { // 三元表达式判断升降序的实现
					Json::Value temp = obj1;
					obj1 = obj2;
					obj2 = temp;
				}
				break;
			case 2:
				if (UD ? price2 - price1 <= 0 : price2 - price1 > 0) { // 三元表达式判断升降序的实现
					Json::Value temp = obj1;
					obj1 = obj2;
					obj2 = temp;
				}
				break;
			default:
				break;
			}
		}
	}
	// 给重新排序后的数组id再赋值
	int id = 0;
	for (auto const& obj : searchResult) {// 遍历匹配结果
		Json::Value newObj = obj; // 复制对象，使其可修改
		newObj["id"] = id;
		searchResult[id] = newObj;
		id++;
	}
}

// 重绘界面
void drawSearchWindow() {
	BeginBatchDraw();// 暂停绘图
	cleardevice();// 清空之前的内容以达到更换界面的需求
	// 加载背景图片
	loadimage(&chooseBG, "picture/choose.png", 960, 540, true);
	putimage(0, 0, &chooseBG);
	// 设置字体的样式
	settextstyle(16, 0, "Consolas");
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	outtextxy(195, 173, endplace.c_str());// 输出文字
	outtextxy(350, 170, date.c_str());// 输出文字
	// 读取数据并匹配
	match(date, endplace);
	// 原有数据遍历结束，现在对匹配的搜索结果进行排序
	bubble_sort(searchResult, UpDown, sortChoose);// 使用冒泡排序
	settextstyle(16, 0, "Consolas");
	// 迭代匹配结果以得到高铁票数据
	for (auto const& obj : searchResult) {// 遍历匹配结果
		int voteNum = obj["voteNum"].asInt();// 获取票数
		int id = obj["id"].asInt();// 获取票的id索引号
		string state = "有票";
		if (voteNum == 0) {
			state = "无票";
		}
		settextcolor(BLACK);
		// 根据页面输出对应的六个高铁票数据
		if (id / 6 == page) {
			textCenter(obj["trainNum"].asString().c_str(), 30, 252 + (id % 6) * 48, 50, 109);// json对象的起始时间
			textCenter("东莞", 139, 252 + (id % 6) * 48, 50, 105);
			textCenter(obj["endPlace"].asString().c_str(), 244, 252 + (id % 6) * 48, 50, 86);// 目的地
			textCenter(obj["startTime"].asString().c_str(), 330, 252 + (id % 6) * 48, 50, 100);
			textCenter(obj["endTime"].asString().c_str(), 429, 252 + (id % 6) * 48, 50, 118);// 到达时间
			textCenter(obj["price"].asString().c_str(), 547, 252 + (id % 6) * 48, 50, 70);// 价格
			textCenter(std::to_string(voteNum).c_str(), 617, 252 + (id % 6) * 48, 50, 109);
			textCenter(state.c_str(), 727, 252 + (id % 6) * 48, 50, 124);
		}
	}
	string showPage = std::to_string(page + 1) + "/" + std::to_string((searchResult.size() / 6) + 1);// 显示当前页数和总页数
	textCenter(showPage.c_str(), 861, 137, 20, 93);
	FlushBatchDraw();// 将未完成的绘图任务完成
	EndBatchDraw();// 结束绘图缓冲
}

// 字符编码转化
char chTurn(ExMessage mmsg) {
	// 将特殊键盘消息返回的字符编码转化
	BYTE keyboardState[256];
	WCHAR unicodeChar;
	UINT scanCode = (mmsg.lParam >> 16) & 0xFF;
	// 获取键盘状态
	GetKeyboardState(keyboardState);
	// 将 WCHAR 转换为 char
	char narrowChar = '1';
	// 将多字节字符编码转换为 Unicode
	if (ToUnicode(mmsg.wParam, scanCode, keyboardState, &unicodeChar, 1, 0) > 0)
	{
		WideCharToMultiByte(CP_UTF8, 0, &unicodeChar, 1, &narrowChar, sizeof(narrowChar), nullptr, nullptr);
	}
	return narrowChar;
}

// 输入框的实现，与键盘交互
void inputOutput(string& toWrite, void (*drawWindow)()) {
	while (1)
	{
		ExMessage mmsg1;// 消息结构体
		mmsg1 = getmessage(EX_KEY);// 获取键盘消息，堵塞函数语句
		if (mmsg1.message == WM_KEYDOWN) {
			char narrowChar = chTurn(mmsg1);
			// 处理键盘消息的字符编码
			BeginBatchDraw();// 暂停绘图
			if (mmsg1.vkcode == 0x11) {// 点击ctrl键
				continue;
			}
			else if (mmsg1.vkcode == 0x10) {// 点击SHIFT键
				while (1)
				{
					ExMessage mmsg2 = getmessage(EX_KEY);// 获取键盘消息，堵塞语句
					if (mmsg2.message == WM_KEYUP) {// 如果是键盘弹起消息
						if (mmsg2.vkcode == 0x10) {// 如果消息是shift键触发的
							break;
						}
					}
					else if (mmsg2.message == WM_KEYDOWN) {
						char narrow_Char = chTurn(mmsg2);
						if (mmsg2.vkcode == 0x31) {
							toWrite = toWrite + "!";
						}
						else if (mmsg2.vkcode == 0x32) {
							toWrite = toWrite + "@";
						}
						else if (mmsg2.vkcode == 0x33) {
							toWrite = toWrite + "#";
						}
						else if (mmsg2.vkcode == 0x34) {
							toWrite = toWrite + "$";
						}
						else if (mmsg2.vkcode == 0x35) {
							toWrite = toWrite + "%";
						}
						else if (mmsg2.vkcode == 0x36) {
							toWrite = toWrite + "^";
						}
						else if (mmsg2.vkcode == 0x37) {
							toWrite = toWrite + "&";
						}
						else if (mmsg2.vkcode == 0x38) {
							toWrite = toWrite + "*";
						}
						else if (mmsg2.vkcode == 0x39) {
							toWrite = toWrite + "(";
						}
						else if (mmsg2.vkcode == 0x30) {
							toWrite = toWrite + ")";
						}
						else if (mmsg2.vkcode == 0xBA) {
							toWrite = toWrite + ":";
						}
						else if (mmsg2.vkcode == 0xBF) {
							toWrite = toWrite + "?";
						}
						else if (mmsg2.vkcode == 0xC0) {
							toWrite = toWrite + "~";
						}
						else if (mmsg2.vkcode == 0xDB) {
							toWrite = toWrite + "{";
						}
						else if (mmsg2.vkcode == 0xDC) {
							toWrite = toWrite + "|";
						}
						else if (mmsg2.vkcode == 0xDD) {
							toWrite = toWrite + "}";
						}
						else if (mmsg2.vkcode == 0xDE) {
							toWrite = toWrite + "\"";
						}
						else if (narrow_Char > 60 && narrow_Char <= 122) {
							toWrite = toWrite + (char)toupper(narrow_Char);
						}
						drawWindow();// 绘图任务
						FlushBatchDraw();// 将未完成的绘图任务完成
						EndBatchDraw();// 结束绘图缓冲
					}
				}
				continue;
			}
			else if (mmsg1.vkcode == 0x08) {// 点击Backspace键
				toWrite = toWrite.substr(0, toWrite.length() - 1);
			}
			else if (mmsg1.vkcode == 0x0D) {// 点击Enter键
				break;
			}
			else {
				toWrite = toWrite + narrowChar;
			}
			drawWindow();// 绘图任务
			FlushBatchDraw();// 将未完成的绘图任务完成
			EndBatchDraw();// 结束绘图缓冲
		}
	}
}

// 高铁票查询页面
void SearchWindow() {
	settextcolor(BLACK);
	drawSearchWindow();// 初始化界面

	ExMessage msg;// 创建一个消息结构体
	//while无限循环监听鼠标和键盘事件以调用函数
	while (1)
	{
		msg = getmessage(EX_MOUSE);// 获取鼠标消息，堵塞语句
		int x = msg.x;
		int y = msg.y;
		if(msg.lbutton == 1){// 判断鼠标左键是否按下
			clickAnimate(x, y, drawSearchWindow);
			if (x >= 118 && x <= 244 && y >= 158 && y <= 200) { // 点击目的地输入框
				page = 0;
				inputOutput(endplace, drawSearchWindow);
			}
			else if (x >= 250 && x <= 452 && y >= 158 && y <= 200) {// 点击出发输入框
				page = 0;
				inputOutput(date, drawSearchWindow);
			}
			else if (x >= 559 && x <= 584 && y >= 160 && y <= 178) {// 点击发车时间排序的升序键
				sortChoose = 0;
				UpDown = true;
				drawSearchWindow();
			}
			else if (x >= 559 && x <= 584 && y >= 180 && y <= 199) {// 点击发车时间排序的降序键
				sortChoose = 0;
				UpDown = false;
				drawSearchWindow();
			}
			else if (x >= 692 && x <= 718 && y >= 160 && y <= 178) {// 点击运行时间排序的升序键
				sortChoose = 1;
				UpDown = true;
				drawSearchWindow();
			}
			else if (x >= 628 && x <= 718 && y >= 180 && y <= 199) {// 点击运行时间排序的降序键
				sortChoose = 1;
				UpDown = false;
				drawSearchWindow();
			}
			else if (x >= 824 && x <= 850 && y >= 160 && y <= 178) {// 点击价格排序的升序键
				sortChoose = 2;
				UpDown = true;
				drawSearchWindow();
			}
			else if (x >= 824 && x <= 850 && y >= 180 && y <= 199) {// 点击车票价格排序的降序键
				sortChoose = 2;
				UpDown = false;
				drawSearchWindow();
			}
			else if (x >= 862 && x <= 909 && y >= 161 && y <= 196) {// 点击前一页
				if (page-- >= 1)
					drawSearchWindow();
				else
					page = 1;
			}
			else if (x > 909 && x <= 956 && y >= 161 && y <= 196) {// 点击前一页
				if (++page < ((searchResult.size() / 6) + 1))
					drawSearchWindow();
				else
					page--;
			}
			else if (x > 861 && x <= 940 && y >= 258 && y <= 298) {// 点击第一个订票按钮
				Choose = 0;// 选择了当前第一张高铁票
				if (searchResult[Choose + page * 6]["voteNum"].asInt() != 0) {
					jumpInterfact = 6;// 选票界面索引
					break;
				}
			}
			else if (x > 861 && x <= 940 && y >= 305 && y <= 345) {// 点击第二个订票按钮
				Choose = 1;// 选择了当前第二张高铁票
				if (searchResult[Choose + page * 6]["voteNum"].asInt() != 0) {
					jumpInterfact = 6;// 选票界面索引
					break;
				}
			}
			else if (x > 861 && x <= 940 && y >= 353 && y <= 393) {// 点击第三个订票按钮
				Choose = 2;// 选择了当前第三张高铁票
				if (searchResult[Choose + page * 6]["voteNum"].asInt() != 0) {
					jumpInterfact = 6;// 选票界面索引
					break;
				}
			}
			else if (x > 861 && x <= 940 && y >= 400 && y <= 440) {// 点击第四个订票按钮
				Choose = 3;// 选择了当前第四张高铁票
				if (searchResult[Choose + page * 6]["voteNum"].asInt() != 0) {
					jumpInterfact = 6;// 选票界面索引
					break;
				}
			}
			else if (x > 861 && x <= 940 && y >= 448 && y <= 488) {// 点击第五个订票按钮
				Choose = 4;// 选择了当前第五张高铁票
				if (searchResult[Choose + page * 6]["voteNum"].asInt() != 0) {
					jumpInterfact = 6;// 选票界面索引
					break;
				}
			}
			else if (x > 861 && x <= 940 && y >= 500 && y <= 540) {// 点击第六个订票按钮
				Choose = 5;// 选择了当前第六张高铁票
				if (searchResult[Choose + page * 6]["voteNum"].asInt() != 0) {
					jumpInterfact = 6;// 选票界面索引
					break;
				}
			}
			else if (x > 0 && x <= 155 && y >= 126 && y <= 156) {// 点击返回目录按钮
				jumpInterfact = 2;
				break;
			}
		}
	}
	// 根据跳转的索引选择调用的界面函数
	if (jumpInterfact == 6) {
		result = searchResult[Choose + page * 6];
		ticketBookWindow();
	}
	else if (jumpInterfact == 2) {
		directoryWindow();
	}
}
