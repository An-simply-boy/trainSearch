// 登陆界面 索引为1
#include "load.h"

IMAGE loadBG;
bool registerOrLoad = false;// 注册或登录(true是注册)
User myUser;// 临时储存用户输入的信息
extern int jumpInterfact;// 界面跳转索引

// 匹配密码格式，要求包括：密码需要包含6到18个字符，可以是大小写字母、数字以及特殊字符!@#$%^&*，但密码必须同时包含字母和数字，而字母的大小写不限。
bool isPasswordValid(const string& password) {
    // 正则表达式模式
    const regex pattern(R"(^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d!@#$%^&*]{6,18}$)");

    // 使用正则表达式匹配
    return regex_match(password, pattern);
}

bool isUserExist(User user) { // 判断用户是否已存在
    ifstream infile(FILENAME);
    string name, pwd;
    while (infile >> name >> pwd) {
        if (name == user.username) {
            infile.close();
            return true;
        }
    }
    infile.close();
    return false;
}

bool isUserValid(User user) { // 判断用户名密码是否正确
    ifstream infile(FILENAME);
    string name, pwd;
    while (infile >> name >> pwd) {
        if (name == user.username && pwd == user.password) {
            infile.close();
            return true;
        }
    }
    infile.close();
    return false;
}

void registerUser(User user) { // 用户注册
    if (isUserExist(user)) {
        settextcolor(RED);
        textCenter("用户名已存在，请重新输入！", 402, 374, 30, 158);
        return;
    }

    ofstream outfile(FILENAME, ios::app);
    outfile << user.username << " " << user.password << endl;
    settextcolor(GREEN);
    textCenter("注册成功！", 402, 374, 30, 158);
    outfile.close();
}

bool loginUser(User user) { // 用户登录
    if (!isUserExist(user)) {
        settextcolor(RED);
        outtextxy(654, 219, "用户名不存在，请先注册！");
        return false;
    }
    if (isUserValid(user)) {
        settextcolor(GREEN);
        textCenter("登陆成功！", 402, 374, 30, 158);
        Sleep(1000);
        return true;
    }
    else {
        settextcolor(RED);
        outtextxy(654, 314, "密码不正确，请重新输入！");
        return false;
    }
}

void drawLoadWindow() {
	BeginBatchDraw();// 暂停绘图
	cleardevice();// 清屏
	putimage(0, 0, &loadBG);// 加载图片
    // 输出文字
    settextcolor(WHITE);
    settextstyle(50, 0, "Consolas");
    setbkmode(TRANSPARENT); 
    if(registerOrLoad){
        textCenter("注册", 408, 142, 48, 132);
    }
    else {
        textCenter("登录", 408, 142, 48, 132);
    }
    settextstyle(36, 0, "Consolas");
    textCenter(myUser.username.c_str(), 408, 219, 47, 227);
    textCenter(myUser.password.c_str(), 408, 302, 56, 227);
    settextcolor(BLACK);
    settextstyle(16, 0, "Consolas");
    textCenter("确定", 365, 408, 31, 97);
    textCenter("切换", 516, 408, 31, 97);
	FlushBatchDraw();// 将未完成的绘图任务完成
	EndBatchDraw();// 结束绘图缓冲
}

void loadWindow() {
	loadimage(&loadBG, "picture/load.png");
	drawLoadWindow();

	ExMessage msg;
	while (1) {
        msg = getmessage(EX_MOUSE);// 获取鼠标消息，堵塞语句
		int x = msg.x;
		int y = msg.y;
		if (msg.lbutton == 1) {// 判断鼠标左键是否按下
            clickAnimate(x, y, drawLoadWindow);
			if (x >= 408 && x < 635 && y >= 219 && y < 266) {// 账号输入框
                inputOutput(myUser.username, drawLoadWindow);
			}
            else if (x >= 408 && x < 635 && y >= 302 && y < 358) {// 密码输入框
                inputOutput(myUser.password, drawLoadWindow);
            }
            else if (x >= 365 && x < 462 && y >= 408 && y < 439) {
                if (myUser.username != "" && myUser.password != "" && registerOrLoad == true) {// 注册确定键
                    if(isPasswordValid(myUser.password)){// 如果密码格式正确
                        registerUser(myUser);
                    }
                    else {
                        settextcolor(RED);
                        textCenter("密码要求6到18个的大小写字母、数字以及特殊字符", 402, 374, 30, 158);// 密码格式错误提示
                    }
                }
                else if (myUser.username != "" && myUser.password != "" && registerOrLoad == false) {// 登录确定键
                    if (loginUser(myUser)) {
                        jumpInterfact = 2;
                        break;
                    }
                }
                else {
                    settextcolor(RED);
                    textCenter("请输入账号和密码", 402, 374, 30, 158);
                }
            }
            else if (x >= 516 && x < 613 && y >= 408 && y < 439) {// 注册登录切换键
                registerOrLoad = !registerOrLoad;
                myUser.password = "";
                myUser.username = "";
                drawLoadWindow();
            }
		}
	}
    if (jumpInterfact == 2) {
        directoryWindow();
    }
}
