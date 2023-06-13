// ��Ϣ������� ������Ϊ4
#include "people.h"

IMAGE peopleListBG;// ����ͼƬ
IMAGE addPeopleBG;// ����ͼƬ
extern int jumpInterfact;// ҳ����ת����
int peopleIndex;// ��Ϣ����
bool isModify = false;// �Ƿ������޸���Ϣ�ķ�ʽ�򿪺���
Json::Value peopleList(Json::arrayValue);// ����¼��������˵���Ϣ
Json::Value peopleWrite;// ������Ҫ�޸ģ���ӣ�����Ϣ
string name, sex, address, mailbox, PhoneNum, IdentityCardNum;

void drawPeopleListWindow() {
	BeginBatchDraw();// ��ͣ��ͼ
	cleardevice();// ����
	putimage(0, 0, &peopleListBG);// ����ͼƬ
	settextstyle(16, 0, "Consolas");
	settextcolor(BLACK);
	for (unsigned int i = 0; i < peopleList.size(); i++) {// ѭ����Ⱦ����
		textCenter(peopleList[i]["name"].asString().c_str(), 0, 318 + i * 74, 74, 58);// ����
		textCenter(peopleList[i]["sex"].asString().c_str(), 58, 318 + i * 74, 74, 54);// �Ա�
		textCenter(peopleList[i]["phoneNum"].asString().c_str(), 110, 318 + i * 74, 74, 134);// �ֻ�����
		textCenter(peopleList[i]["identityCardNum"].asString().c_str(), 243, 318 + i * 74, 74, 186);// ���֤����
		textCenter(peopleList[i]["mailbox"].asString().c_str(), 427, 318 + i * 74, 74, 143);// ����
		textCenter(peopleList[i]["address"].asString().c_str(), 569, 318 + i * 74, 74, 117);// ��ַ
	}
	FlushBatchDraw();// ��δ��ɵĻ�ͼ�������
	EndBatchDraw();// ������ͼ����
}
void peopleDelete() {
	peopleList.removeIndex(peopleIndex, NULL);// ɾ����Ӧ�±��Ԫ��
	std::ofstream outFire("afterIsChoose.json");// ���¸����ĵ�
	if (outFire.is_open()) {
		outFire << peopleList.toStyledString();
		outFire.close();
	}
	drawPeopleListWindow();
}
void peopleListWindow() {
	//��json�ļ�
	std::ifstream file("afterIsChoose.json");
	file >> peopleList;// ���ļ��ж�ȡ���ݵ�json������
	file.close();// �ر��ļ�
	loadimage(&peopleListBG, "picture/peopleList.png");
	drawPeopleListWindow();// ������Ϣ¼�봰��

	ExMessage msg;
	while (1)
	{
		if (peekmessage(&msg, EX_MOUSE, true)) {// ��ȡ�����Ϣ���Ƕ������
			int x = msg.x;
			int y = msg.y;
			if (msg.lbutton == 1) {// �ж��������Ƿ���
				clickAnimate(x, y, drawPeopleListWindow);// �������
				if (x >= 0 && x < 174 && y > 168 && y < 212) {// ����Ŀ¼����
					jumpInterfact = 2;
					break;
				}
				else if (x >= 739 && x < 934 && y > 211 && y < 267) {// �����Ӱ�ť
					jumpInterfact = 3;
					break;
				}
				else if (x >= 735 && x < 838 && y > 322 && y < 389) {// ��һ���޸İ�ť
					isModify = true;
					peopleIndex = 0;
					jumpInterfact = 3;
					break;
				}
				else if (x >= 839 && x < 940 && y > 322 && y < 389) {// ��һ��ɾ����ť
					peopleIndex = 0;
					peopleDelete();
				}
				else if (x >= 735 && x < 838 && y > 397 && y < 464) {// �ڶ����޸İ�ť
					isModify = true;
					peopleIndex = 1;
					jumpInterfact = 3;
					break;
				}
				else if (x >= 839 && x < 940 && y > 397 && y < 464) {// �ڶ���ɾ����ť
					peopleIndex = 1;
					peopleDelete();
				}
				else if (x >= 735 && x < 838 && y > 469 && y < 535) {// �������޸İ�ť
					isModify = true;
					peopleIndex = 2;
					jumpInterfact = 3;
					break;
				}
				else if (x >= 839 && x < 940 && y > 469 && y < 535) {// ������ɾ����ť
					peopleIndex = 2;
					peopleDelete();
				}
			}
		}
	}
	if (jumpInterfact == 2) {// ����Ŀ¼����
		directoryWindow();
	}
	else if (jumpInterfact == 3) {
		addPeopleWindow();
	}
}

// ��Ϣ¼����� ����Ϊ3

bool isPhoneNumberValid(const string& phoneNumber) {// �ж��ֻ������ʽ�Ƿ�������ȷ
	// ������ʽģʽ
	const regex pattern(R"(^1\d{10}$)");

	// ʹ��������ʽƥ��
	return regex_match(phoneNumber, pattern);
}

bool isIDCardValid(const string& idCard) {// �ж����֤��ʽ�Ƿ�������ȷ
	// ������ʽģʽ
	const regex pattern(R"(^\d{17}[\dX]$)");

	// ʹ��������ʽƥ��
	return regex_match(idCard, pattern);
}

bool isEmailValid(const string& email) {
	// ������ʽģʽ
	const regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

	// ʹ��������ʽƥ��
	return regex_match(email, pattern);
}

// ������Ϣ¼�����
void drawAddPeopleWindow() {
	BeginBatchDraw();// ��ͣ��ͼ
	cleardevice();// ����
	putimage(0, 0, &addPeopleBG);// ����ͼƬ

	// �����������ʽ
	settextstyle(16, 0, "Consolas");
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	// �������
	textCenter(name.c_str(), 295, 205, 37, 367);
	textCenter(sex.c_str(), 295, 254, 37, 367);
	textCenter(PhoneNum.c_str(), 295, 303, 37, 367);
	textCenter(IdentityCardNum.c_str(), 295, 352, 37, 367);
	textCenter(address.c_str(), 295, 402, 37, 367);
	textCenter(mailbox.c_str(), 295, 452, 37, 367);
	FlushBatchDraw();// ��δ��ɵĻ�ͼ�������
	EndBatchDraw();// ������ͼ����
}

// ��Ϣ¼�����
void addPeopleWindow() {
	loadimage(&addPeopleBG, "picture/addPeople.png");
	//��json�ļ�
	std::ifstream file("afterIsChoose.json");
	file >> peopleList;// ���ļ��ж�ȡ���ݵ�json������
	file.close();// �ر��ļ�
	peopleWrite.clear();
	if(isModify){
		peopleWrite = peopleList[peopleIndex];// ������޸ģ����ȡ�޸ĵ�ֵ
	}
	// ��ȡֵ,��ʼ��
	name = peopleWrite["name"].asString();
	sex = peopleWrite["sex"].asString();
	PhoneNum = peopleWrite["phoneNum"].asString();
	IdentityCardNum = peopleWrite["identityCardNum"].asString();
	address = peopleWrite["address"].asString();
	mailbox = peopleWrite["mailbox"].asString();
	drawAddPeopleWindow();// ���ƴ���

	ExMessage msg;// ��Ϣ�ṹ��
	while (1)
	{
		msg = getmessage(EX_MOUSE);// ��ȡ�����Ϣ���������
		int x = msg.x;
		int y = msg.y;
		if (msg.lbutton == 1) {// �ж��������Ƿ���
			clickAnimate(x, y, drawAddPeopleWindow);// �������
			if (x >= 0 && x < 245 && y > 150 && y < 178) {// �������Ŀ¼��ť
				jumpInterfact = 2;
				break;
			}
			else if (x >= 295 && x < 662 && y > 205 && y < 242) {// ������������
				inputOutput(name, drawAddPeopleWindow);
			}
			else if (x >= 295 && x < 662 && y > 254 && y < 291) {// ����Ա������
				inputOutput(sex, drawAddPeopleWindow);
			}
			else if (x >= 295 && x < 662 && y > 303 && y < 340) {// ����ֻ����������
				inputOutput(PhoneNum, drawAddPeopleWindow);
			}
			else if (x >= 295 && x < 662 && y > 352 && y < 389) {// ������֤���������
				inputOutput(IdentityCardNum, drawAddPeopleWindow);
			}
			else if (x >= 295 && x < 662 && y > 402 && y < 439) {// �����ַ�����
				inputOutput(address, drawAddPeopleWindow);
			}
			else if (x >= 295 && x < 662 && y > 452 && y < 489) {// ������������
				inputOutput(mailbox, drawAddPeopleWindow);
			}
			else if (x >= 394 && x < 540 && y > 504 && y < 536) {// ���ȷ������
				if (!name.empty() && !sex.empty() && !address.empty() && !mailbox.empty() && !IdentityCardNum.empty() && !PhoneNum.empty()) {
					settextcolor(RED);
					if (!isPhoneNumberValid(PhoneNum)) {
						outtextxy(714, 309, "�ֻ��ų��Ȳ���11λ��");
					}
					if (!isIDCardValid(IdentityCardNum)) {
						outtextxy(714, 358, "���֤���Ȳ���18λ��");
					}
					if (!isEmailValid(mailbox)) {
						outtextxy(714, 460, "�����ʽ����ȷ��");
					}
					if (isPhoneNumberValid(PhoneNum) && isIDCardValid(IdentityCardNum) && isEmailValid(mailbox)) {
						// ��ֵ
						peopleWrite["name"] = name;
						peopleWrite["sex"] = sex;
						peopleWrite["address"] = address;
						peopleWrite["mailbox"] = mailbox;
						peopleWrite["identityCardNum"] = IdentityCardNum;// ���ַ���ת��Ϊ����
						peopleWrite["phoneNum"] = PhoneNum;
						if (isModify) {
							peopleList[peopleIndex] = peopleWrite;// ������޸ģ���������
							isModify = false;
						}
						else
						{
							peopleList.append(peopleWrite);// �������ӣ���Ӷ���
						}
						std::ofstream outFire("afterIsChoose.json");// ���¸����ĵ�
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
					textCenter("��������������Ϣ!!!", 576, 504, 31, 211);
				}
			}
		}
	}
	if (jumpInterfact == 2) {// ����Ŀ¼����
		directoryWindow();
	}
	else if (jumpInterfact == 4) {// ��ת����Ϣ�������
		peopleListWindow();
	}
}
