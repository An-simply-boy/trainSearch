// 车票查询界面 索引号为5
#include "search.h"

IMAGE chooseBG;// 背景图片
Json::Value searchResult(Json::arrayValue);//新建一个数组存储搜索匹配的结果
extern Json::Value result;// 选票与人的信息
string date = "2023-05-16", endplace = "GZ";// 用户要求（日期，到达地，座位类型）
int sortChoose = 0, Choose = 0;// 查询结果排序方式(出发时间，运行时长，价格)
bool UpDown = true;// 升降序
unsigned int page = 0;// 选票索引
extern int jumpInterfact;//页面索引

extern bool isRun;// 控制定时器开关
std::condition_variable cv;  // 条件变量，用于线程等待
bool ready = false; // 标志位，表示是否可以启动子线程
std::mutex mtx;  // 共享资源互斥锁
std::mutex m;	// 子线程互斥量
std::mutex bubble_mutex;	// 冒泡排序互斥锁

// 将utf-8字符编码转化为ANSI编码
string Utf8ToAnsi(const std::string& utf8Str)
{
	int len = strlen(utf8Str.c_str()) + 1;
	WCHAR* wChar = new WCHAR[len];
	wChar[0] = 0;
	MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), len, wChar, len);

	char outch[MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, wChar, len, outch, len, 0, 0);
	delete[] wChar;
	return std::string(outch);
}

// 鼠标点击动画
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
	flushmessage(); // 清空鼠标消息缓冲区,鼠标防抖
	fun();
}

// 初始化赋予随机票数
void randomVoteNum() {
	static bool randomcnt = true; // 判断是否为第一次随机赋值票
	std::lock_guard<std::mutex> lock(mtx);  // 加锁
	//打开json文件
	std::ifstream file("高铁票.json");
	Json::Value root;// 创建json对象
	file >> root;// 从文件中读取数据到json对象中
	file.close();// 关闭文件
	Json::Value newRoot(Json::arrayValue);
	for (auto const& obj : root) // 使用迭代器遍历数组中的所有对象
	{
		Json::Value newObj = obj; // 复制对象，使其可修改
		int voteNum;
		// 创建随机数引擎和分布器
		std::random_device rd;  // 用于获取真随机数种子
		std::mt19937 gen(rd());  // Mersenne Twister 随机数引擎
		if (randomcnt) {// 如果是第一次调用，随机赋值
			string ticketTime = obj["startTime"].asString();
			// 获取当前时间
			time_t nowTime;
			time(&nowTime);
			// 转换为本地时间结构体
			struct tm local_time;
			localtime_s(&local_time, &nowTime);
			string times = std::to_string(local_time.tm_hour) + ":" + std::to_string(local_time.tm_min);
			if (timeSubtract(ticketTime, times) >= 0)// 假如该票的发车时间小于现实时间则票数为0
			{
				voteNum = 0;
			}
			else
			{
				std::uniform_int_distribution<int> dist(0, 120);  // 均匀分布器，范围是0到120
				// 生成随机数
				voteNum = dist(gen);
			}
		}
		else {// 如果不是第一次调用，则在每15s内随机将票数减去0-3张
			voteNum = newObj["voteNum"].asInt();
			if (voteNum > 0) {
				std::uniform_int_distribution<int> dist(0, 3);  // 均匀分布器，范围是0到3
				int sub = dist(gen);
				voteNum -= sub;
				if (voteNum < 0) {
					voteNum = 0;
				}
			}
		}
		newObj["voteNum"] = voteNum;
		newRoot.append(newObj);// 将匹配的搜索结果添加到数组里
	}
	randomcnt = false;
	// 重新写入文档
	/*std::ofstream outFire("高铁票.json");
	if (outFire.is_open()) {
		outFire << newRoot.toStyledString();
		outFire.close();
	}*/
	ofstream outFire("高铁票.json", std::ios::binary);
	if (outFire.is_open()) {
		Json::StreamWriterBuilder builder;
		builder.settings_["emitUTF8"] = true;
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(newRoot, &outFire);
		outFire.close();
	}
}

// 在json文件的数据中匹配结果
void match(string date, string endplace) {
	//打开json文件
	std::ifstream file("高铁票.json");
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
				if ( (date == obj["date"].asString()) && ( (endplace == obj["endPlace"].asString()) || (endplace == obj["endPlaceCname"].asString()) ) ) {//匹配搜索结果
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
	std::lock_guard<std::mutex> lock(bubble_mutex);  // 加锁
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
			textCenter(obj["endPlaceCname"].asString().c_str(), 244, 252 + (id % 6) * 48, 50, 86);// 目的地
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

// 判断中文输入
void GetIMEString(HWND hWnd, string& str)
{
	HIMC hIMC = ImmGetContext(hWnd);//获取HIMC 
	if (hIMC)
	{
		//这里先说明一下，以输入“中国”为例 
		//切换到中文输入法后，输入“zhongguo”，这个字符串称作IME组成字符串
		//而在输入法列表中选择的字符串“中国”则称作IME结果字符串 
		static bool flag = false;//输入完成标记：在输入中时，IME组成字符串不为空，置true；输入完成后，IME组成字符串为空，置false 
		DWORD dwSize = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, NULL, 0); //获取IME组成输入的字符串的长度 
		if (dwSize > 0)//如果IME组成字符串不为空，且没有错误（此时dwSize为负值），则置输入完成标记为true 
		{
			if (flag == false)
			{
				flag = true;
			}
		}
		else if (dwSize == 0 && flag) //如果IME组成字符串为空，并且标记为true，则获取IME结果字符串 
		{
			int iSize; //IME结果字符串的大小 
			LPSTR pszMultiByte = NULL;//IME结果字符串指针 
			int ChineseSimpleAcp = 936;//宽字节转换时中文的编码 
			WCHAR* lpWideStr = NULL;//宽字节字符数组 
			dwSize = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);//获取IME结果字符串的大小 
			if (dwSize > 0) //如果IME结果字符串不为空，且没有错误 
			{
				dwSize += sizeof(WCHAR);//大小要加上NULL结束符 
				//为获取IME结果字符串分配空间 
				if (lpWideStr)
				{
					delete[]lpWideStr;
					lpWideStr = NULL;
				}
				lpWideStr = new WCHAR[dwSize];
				memset(lpWideStr, 0, dwSize); //清空结果空间 
				ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, lpWideStr, dwSize);//获取IME结果字符串，这里获取的是宽字节 
				iSize = WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, NULL, 0, NULL, NULL);//计算将IME结果字符串转换为ASCII标准字节后的大小 
				//为转换分配空间 
				if (pszMultiByte)
				{
					delete[] pszMultiByte;
					pszMultiByte = NULL;
				}
				pszMultiByte = new char[iSize + 1];
				WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, pszMultiByte, iSize, NULL, NULL);//宽字节转换 
				pszMultiByte[iSize] = '\0';
				str += pszMultiByte;//添加到string中 
				//释放空间 
				if (lpWideStr)
				{
					delete[]lpWideStr;
					lpWideStr = NULL;
				}
				if (pszMultiByte)
				{
					delete[] pszMultiByte;
					pszMultiByte = NULL;
				}
			}
			flag = false;
		}
		ImmReleaseContext(hWnd, hIMC);//释放HIMC 
	}
}

// 输入框的实现，与键盘交互
void inputOutput(string& toWrite, void (*drawWindow)(), int x1, int y1, int x2, int y2) {
	static bool CorE = true;		// 判断中英文输入
	while (1)
	{
		SHORT my_CAPITAL = GetKeyState(VK_CAPITAL);	// 大小写
		bool bigSmall = my_CAPITAL;
		ExMessage mmsg1;	// 消息结构体
		mmsg1 = getmessage(EX_KEY | EX_MOUSE);	// 获取键盘消息，堵塞函数语句
		BeginBatchDraw();	// 暂停绘图
		if (CorE) {	// 中文输入
			HWND hWnd = GetHWnd();
			HDC hdc = GetDC(hWnd);
			GetIMEString(hWnd, toWrite);
			if (mmsg1.message == WM_KEYUP) {
				bool isBreak = false;
				switch (mmsg1.vkcode)
				{
				case 0x10:		// 按下SHIFT键
					CorE = false;
					break;
				case 0x08:		// 按下Backspace键
					if (toWrite.length() > 0)
					{
						if (toWrite.at(toWrite.length() - 1) & 0x8000)
							toWrite.erase(toWrite.end() - 1);
						toWrite.erase(toWrite.end() - 1);
					}
					else {
						continue;
					}
					break;
				case 0x0D:		// 按下Enter键
					isBreak = true;
					break;
				default:
					break;
				}
				if (isBreak) {
					break;
				}
			}
		}else{
			if (mmsg1.message == WM_KEYDOWN) {
				char narrowChar = chTurn(mmsg1);
				// 处理键盘消息的字符编码
				if (mmsg1.vkcode == 0x11) {// 点击ctrl键
					continue;
				}
				else if (mmsg1.vkcode == 0x10) {// 点击SHIFT键
					bool isUp = true;
					while (1)
					{
						ExMessage mmsg2 = getmessage(EX_KEY);	// 获取键盘消息
						if (mmsg2.message == WM_KEYUP) {	// 如果是键盘弹起消息
							if (mmsg2.vkcode == 0x10) {	// 如果消息是shift键触发的
								if(isUp)CorE = true;
								break;
							}
						}
						else if (mmsg2.message == WM_KEYDOWN) {
							isUp = false;
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
				else if (mmsg1.vkcode == 0x08) {	// 点击Backspace键
					//toWrite = toWrite.substr(0, toWrite.length() - 1);
					if (toWrite.length() > 0)
					{
						if (toWrite.at(toWrite.length() - 1) & 0x8000)
							toWrite.erase(toWrite.end() - 1);
						toWrite.erase(toWrite.end() - 1);
					}
				}
				else if (mmsg1.vkcode == 0x0D) {	// 点击Enter键
					break;
				}
				else if (mmsg1.vkcode == 0x14 || mmsg1.vkcode == 0x12) {	// 点击caps键或者alt键
					continue;
				}
				else if (mmsg1.vkcode == 0x09) {
					toWrite = toWrite + "  ";
				}
				else if (narrowChar > 60 && narrowChar <= 122 && bigSmall) {	// 如果caps处于大写状态
					toWrite = toWrite + (char)toupper(narrowChar);
				}
				else {
					toWrite = toWrite + narrowChar;
				}
			}
		}
		drawWindow();// 绘图任务
		FlushBatchDraw();// 将未完成的绘图任务完成
		EndBatchDraw();// 结束绘图缓冲
		// 点击非输入框区域跳出循环
		if (mmsg1.message == WM_LBUTTONDOWN) {
			int x = mmsg1.x;
			int y = mmsg1.y;
			if (!(x > x1 && x < x2 && y > y1 && y < y2)) {
				clickAnimate(x, y, drawWindow);
				break;
			}
		}
	}
}

// 定时器（每5秒票数随机减少0-3张，以模拟其他乘客购票的情况）
void timerFunction(int seconds) {
	randomVoteNum();
	while (isRun) {
		std::unique_lock<std::mutex> lk(m); // 获取互斥锁
		cv.wait(lk, []() {return ready; }); // 等待主线程的信号
		if (!isRun) {// 如果程序关闭，则直接结束子线程
			break;
		}
		randomVoteNum();	// 票数随机减少，以模拟
		drawSearchWindow();		// 更新窗口
		std::this_thread::sleep_for(std::chrono::seconds(seconds));	// 定时器暂停5秒
	}
}

// 高铁票查询页面
void SearchWindow() {
	settextcolor(BLACK);
	drawSearchWindow();// 初始化界面

	// 启动子线程
	{
		std::lock_guard<std::mutex> lk(m); // 获取互斥锁
		ready = true; // 将标志位设为 true，表示可以启动子线程
		cv.notify_one(); // 发出信号，唤醒子线程
	}

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
				inputOutput(endplace, drawSearchWindow, 118, 158, 244, 200);
			}
			else if (x >= 250 && x <= 452 && y >= 158 && y <= 200) {// 点击出发输入框
				page = 0;
				inputOutput(date, drawSearchWindow, 250, 158, 452, 200);
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

	result = searchResult[Choose + page * 6];
	ready = false; // 将标志位设为 false，表示子线程需要再次等待主线程的信号
}
