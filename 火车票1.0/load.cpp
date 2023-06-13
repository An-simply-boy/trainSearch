// ��½���� ����Ϊ1
#include "load.h"

IMAGE loadBG;
bool registerOrLoad = false;// ע����¼(true��ע��)
User myUser;// ��ʱ�����û��������Ϣ
extern int jumpInterfact;// ������ת����

// ƥ�������ʽ��Ҫ�������������Ҫ����6��18���ַ��������Ǵ�Сд��ĸ�������Լ������ַ�!@#$%^&*�����������ͬʱ������ĸ�����֣�����ĸ�Ĵ�Сд���ޡ�
bool isPasswordValid(const string& password) {
    // ������ʽģʽ
    const regex pattern(R"(^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d!@#$%^&*]{6,18}$)");

    // ʹ��������ʽƥ��
    return regex_match(password, pattern);
}

bool isUserExist(User user) { // �ж��û��Ƿ��Ѵ���
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

bool isUserValid(User user) { // �ж��û��������Ƿ���ȷ
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

void registerUser(User user) { // �û�ע��
    if (isUserExist(user)) {
        settextcolor(RED);
        textCenter("�û����Ѵ��ڣ����������룡", 402, 374, 30, 158);
        return;
    }

    ofstream outfile(FILENAME, ios::app);
    outfile << user.username << " " << user.password << endl;
    settextcolor(GREEN);
    textCenter("ע��ɹ���", 402, 374, 30, 158);
    outfile.close();
}

bool loginUser(User user) { // �û���¼
    if (!isUserExist(user)) {
        settextcolor(RED);
        outtextxy(654, 219, "�û��������ڣ�����ע�ᣡ");
        return false;
    }
    if (isUserValid(user)) {
        settextcolor(GREEN);
        textCenter("��½�ɹ���", 402, 374, 30, 158);
        Sleep(1000);
        return true;
    }
    else {
        settextcolor(RED);
        outtextxy(654, 314, "���벻��ȷ�����������룡");
        return false;
    }
}

void drawLoadWindow() {
	BeginBatchDraw();// ��ͣ��ͼ
	cleardevice();// ����
	putimage(0, 0, &loadBG);// ����ͼƬ
    // �������
    settextcolor(WHITE);
    settextstyle(50, 0, "Consolas");
    setbkmode(TRANSPARENT); 
    if(registerOrLoad){
        textCenter("ע��", 408, 142, 48, 132);
    }
    else {
        textCenter("��¼", 408, 142, 48, 132);
    }
    settextstyle(36, 0, "Consolas");
    textCenter(myUser.username.c_str(), 408, 219, 47, 227);
    textCenter(myUser.password.c_str(), 408, 302, 56, 227);
    settextcolor(BLACK);
    settextstyle(16, 0, "Consolas");
    textCenter("ȷ��", 365, 408, 31, 97);
    textCenter("�л�", 516, 408, 31, 97);
	FlushBatchDraw();// ��δ��ɵĻ�ͼ�������
	EndBatchDraw();// ������ͼ����
}

void loadWindow() {
	loadimage(&loadBG, "picture/load.png");
	drawLoadWindow();

	ExMessage msg;
	while (1) {
        msg = getmessage(EX_MOUSE);// ��ȡ�����Ϣ���������
		int x = msg.x;
		int y = msg.y;
		if (msg.lbutton == 1) {// �ж��������Ƿ���
            clickAnimate(x, y, drawLoadWindow);
			if (x >= 408 && x < 635 && y >= 219 && y < 266) {// �˺������
                inputOutput(myUser.username, drawLoadWindow);
			}
            else if (x >= 408 && x < 635 && y >= 302 && y < 358) {// ���������
                inputOutput(myUser.password, drawLoadWindow);
            }
            else if (x >= 365 && x < 462 && y >= 408 && y < 439) {
                if (myUser.username != "" && myUser.password != "" && registerOrLoad == true) {// ע��ȷ����
                    if(isPasswordValid(myUser.password)){// ��������ʽ��ȷ
                        registerUser(myUser);
                    }
                    else {
                        settextcolor(RED);
                        textCenter("����Ҫ��6��18���Ĵ�Сд��ĸ�������Լ������ַ�", 402, 374, 30, 158);// �����ʽ������ʾ
                    }
                }
                else if (myUser.username != "" && myUser.password != "" && registerOrLoad == false) {// ��¼ȷ����
                    if (loginUser(myUser)) {
                        jumpInterfact = 2;
                        break;
                    }
                }
                else {
                    settextcolor(RED);
                    textCenter("�������˺ź�����", 402, 374, 30, 158);
                }
            }
            else if (x >= 516 && x < 613 && y >= 408 && y < 439) {// ע���¼�л���
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
