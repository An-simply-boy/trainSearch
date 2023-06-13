// 订票界面 索引号为6
#include "ticketBook.h"

IMAGE ticketBG;
IMAGE DeleteButton;
extern int jumpInterfact;//页面索引
Json::Value peopleMsg;// 储存录入的人的信息
Json::Value result;// 选票与人的信息
int arr[3] = { 0, 0, 0 };//记录按钮状态

// 将宽字符串转化为utf-8编码格式的字符串
std::string convertToUTF8(const std::wstring& input, const std::locale& loc) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(input);
}
// 将原始编码转化为宽字符串
std::wstring convertFromUTF8(const std::string& input, const std::locale& loc) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.from_bytes(input);
}

// 绘制订票窗口
void drawTicketBookWindow() {
	BeginBatchDraw();// 暂停绘图
	cleardevice();// 清屏
	putimage(0, 0, &ticketBG);
	settextcolor(BLACK);
	// 渲染选中高铁票的数据
	textCenter((result["trainNum"].asString()).c_str(), 38, 223, 58, 119);
	textCenter("东莞", 157, 223, 58, 117);
	textCenter((result["endPlace"].asString()).c_str(), 274, 223, 58, 92);
	textCenter((result["startTime"].asString()).c_str(), 366, 223, 58, 129);
	textCenter((result["endTime"].asString()).c_str(), 495, 223, 58, 120);
	textCenter((result["price"].asString()).c_str(), 615, 223, 58, 68);
	textCenter((result["voteNum"].asString()).c_str(), 682, 223, 58, 150);
	textCenter("有票", 832, 223, 58, 123);
	//读取录入的人的信息文件
	std::ifstream file("afterIsChoose.json");
	file >> peopleMsg;// 从文件中读取数据到json对象中
	file.close();// 关闭文件

	// 以 utf-8 的编码格式打开文件 并 读取数据
	//std::ifstream file("afterIsChoose.json");
	//if (file.is_open()) {
	//	// 读取文件内容
	//	std::stringstream buffer;
	//	buffer << file.rdbuf();
	//	std::string fileContent = buffer.str();
	//	// 将文件内容从原始编码（例如GB2312）转换为宽字符串（UTF-16或UTF-32）
	//	std::wstring wideContent = convertFromUTF8(fileContent, std::locale(""));  // 可根据实际编码类型设置locale
	//	// 将宽字符串转换为UTF-8编码
	//	std::string utf8Content = convertToUTF8(wideContent, std::locale(""));
	//	// 使用JsonCpp解析UTF-8编码的数据
	//	Json::CharReaderBuilder reader;
	//	std::istringstream jsonStream(utf8Content);
	//	std::string errors;
	//	if (Json::parseFromStream(reader, jsonStream, &peopleMsg, &errors))
	//	file.close();
	//}

	// 渲染读取的信息
	int index = 0;
	for (auto const& obj : peopleMsg) {// 遍历匹配结果
		string name = obj["name"].asString();
		textCenter(name.c_str(), 157, 324 + index * 53, 53, 118);// 第一位人的姓名
		textCenter((obj["identityCardNum"].asString()).c_str(), 273, 324 + index * 53, 53, 687);// 第一位人的身份证
		if (arr[index]) {
			putimage(36, 324 + index * 53, &DeleteButton);
		}
		index++;
	}
	FlushBatchDraw();// 将未完成的绘图任务完成
	EndBatchDraw();// 结束绘图缓冲
}

// 选人与删除
void selectDelete(int i) {
	arr[i] = ~arr[i];// 此时数组对应位置的数变成0或-1 以此判断改人是否被选中
	// 重新绘图
	drawTicketBookWindow();
	for (int Index = 0; Index < 3; Index++) {// 查找是否存储选中的人的信息
		if (arr[Index]) {
			putimage(35, 324 + Index * 53, &DeleteButton);//更换为删除按钮
		}
	}
	// 操作result对象以录入选人的信息
	if (arr[i])
	{
		result["man"].append(peopleMsg[i]);
	}
	else {
		// 删除选中人的信息
		for (unsigned int index = 0; index < result["man"].size(); index++) {
			if (result["man"][index] == peopleMsg[i]) {
				result["man"].removeIndex(index, &peopleMsg[i]);
				break;
			}
		}
	}
}

// 订票窗口
void ticketBookWindow() {
	loadimage(&ticketBG, "picture/voteChoose.png");// 背景图片
	loadimage(&DeleteButton, "picture/delete.jpg", 118, 53, true);// 删除图标
	drawTicketBookWindow();

	result["man"] = {};
	ExMessage msg;// 创建一个消息结构体
	while (1) {
		msg = getmessage(EX_MOUSE);// 获取鼠标消息，堵塞语句
		int x = msg.x;
		int y = msg.y;
		if (msg.lbutton == 1) {// 判断鼠标左键是否按下
			clickAnimate(x, y, drawTicketBookWindow);
			if (x >= 40 && x <= 158 && y >= 324 && y < 380) {// 按下第一个添加/删除按钮
				selectDelete(0);
			}
			else if (x >= 40 && x <= 158 && y >= 380 && y < 430) {// 按下第二个添加/删除按钮
				selectDelete(1);
			}
			else if (x >= 40 && x <= 158 && y >= 430 && y <= 485) {// 按下第三个添加/删除按钮
				selectDelete(2);
			}
			else if (x >= 367 && x <= 454 && y >= 489 && y <= 540) {// 按下确定按钮 将信息写入文件
				std::ofstream outFire("beChoose.json");// 打开文件
				if (outFire.is_open()) {
					outFire << result.toStyledString();// 写入信息
					outFire.close();
				}
				jumpInterfact = 7;// 跳转到订单管理界面
				break;
			}
			else if (x >= 537 && x <= 625 && y >= 489 && y <= 540) {// 按下取消按钮
				jumpInterfact = 5;// 返回高铁票查询页面
				break;
			}
			else if (x > 0 && x < 155 && y > 125 && y < 158) {
				jumpInterfact = 2;
				break;
			}
		}
	}

	if (jumpInterfact == 5) {
		SearchWindow();// 返回高铁票查询页面
	}
	else if (jumpInterfact == 2) {
		directoryWindow();
	}
	else if (jumpInterfact == 7) {// 跳转到订单管理窗口
		ticketMSGWindow();
	}
}