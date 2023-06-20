// ����������� ������7
#include"ticketMSG.h"

IMAGE ticketMSGBG;// ����ͼƬ
IMAGE ticketShowBG;// ��������
Json::Value people_ticketMsg;// ���涩����Ϣ����
int index;// ����������
extern int jumpInterfact;// ҳ������
ExMessage msg;// ��Ϣ�ṹ�� ������Ϣ

// ���ƶ���������洰��
void drawTicketMSGWindow() {
	BeginBatchDraw();// ��ͣ��ͼ
	cleardevice();
	putimage(0, 0, &ticketMSGBG);// ���Ʊ���ͼƬ
	settextstyle(24, 0, "Consolas");
	settextcolor(BLACK);
	for (unsigned int i = 0; i < people_ticketMsg["man"].size(); i++) {// ѭ����Ⱦ����
		textCenter(people_ticketMsg["trainNum"].asString().c_str(), 33, 279 + i * 87, 87, 110);
		textCenter("��ݸ", 141, 279 + i * 87, 87, 109);
		textCenter(people_ticketMsg["endPlaceCname"].asString().c_str(), 249, 279 + i * 87, 87, 92);
		textCenter(people_ticketMsg["startTime"].asString().c_str(), 339, 279 + i * 87, 87, 102);
		textCenter(people_ticketMsg["endTime"].asString().c_str(), 442, 279 + i * 87, 87, 121);
		textCenter(people_ticketMsg["price"].asString().c_str(), 562, 279 + i * 87, 87, 72);
		textCenter(people_ticketMsg["man"][i]["name"].asString().c_str(), 632, 279 + i * 87, 87, 129);
	}
	FlushBatchDraw();// ��δ��ɵĻ�ͼ�������
	EndBatchDraw();// ������ͼ����
}

//����������洰��
void ticketMSGWindow() {
	loadimage(&ticketMSGBG, "picture/ticketMSG.png");
	//��json�ļ� ��ȡ����
	std::ifstream file("beChoose.json");
	file >> people_ticketMsg;// ���ļ��ж�ȡ���ݵ�json������
	file.close();// �ر��ļ�
	drawTicketMSGWindow();// ��ͼ

	while (1)
	{
		msg = getmessage(EX_MOUSE);// ��ȡ�����Ϣ���������
		int x = msg.x;
		int y = msg.y;
		if (msg.lbutton == 1) {// �ж��������Ƿ���
			clickAnimate(x, y, drawTicketMSGWindow);
			if (x > 789 && x < 946 && y > 290 && y < 368) {// �鿴��һ�ŵ���Ʊ��
				index = 0;// ��һ��Ʊ����
				if (people_ticketMsg["man"].size() >= index + 1) {
					jumpInterfact = 8;// ��ת������-�鿴����Ʊ��
					break;
				}
			}
			else if (x > 789 && x < 946 && y > 375 && y < 450) {// �鿴�ڶ��ŵ���Ʊ��
				index = 1;// �ڶ���Ʊ����
				if (people_ticketMsg["man"].size() >= index + 1) {
					jumpInterfact = 8;// ��ת������-�鿴����Ʊ��
					break;
				}
			}
			else if (x > 789 && x < 946 && y > 460 && y < 536) {// �鿴�ڶ��ŵ���Ʊ��
				index = 2;// ������Ʊ����
				if (people_ticketMsg["man"].size() >= index + 1) {
					jumpInterfact = 8;// ��ת������-�鿴����Ʊ��
					break;
				}
			}
			else if (x > 12 && x < 161 && y > 131 && y < 171) {// ����Ŀ¼
				jumpInterfact = 2;// ��ת������-�鿴����Ʊ��
				break;
			}
		}
	}
}

// ����Ʊ�ݽ��� ����Ϊ8

// ���Ƶ���Ʊ�ݴ���
void drawTicketShow() {
	BeginBatchDraw();// ��ͣ��ͼ
	cleardevice();
	putimage(0, 0, &ticketShowBG);

	settextstyle(76, 0, "Consolas");
	string time = people_ticketMsg["startTime"].asString() + "-" + people_ticketMsg["endTime"].asString();
	string price = "��" + people_ticketMsg["price"].asString() + "Ԫ";
	textCenter("��ݸ", 167,102,78,153);// ������
	textCenter(people_ticketMsg["endPlaceCname"].asString().c_str(), 627, 102, 78, 200);// �����
	settextstyle(50, 0, "Consolas");
	textCenter(people_ticketMsg["trainNum"].asString().c_str(), 353, 107, 47, 186);// ����
	settextstyle(36, 0, "Consolas");
	textCenter(time.c_str(), 337, 182, 52, 262);// ��ʼ-����ʱ��
	textCenter(price.c_str(), 101, 240, 60, 195);// �۸�
	textCenter(people_ticketMsg["date"].asString().c_str(), 379, 252, 50, 229);// ����
	string lastSixNum = people_ticketMsg["man"][index]["identityCardNum"].asString().substr(11);// ���֤����λ
	textCenter(lastSixNum.c_str(), 309, 459, 76, 170);
	textCenter(people_ticketMsg["man"][index]["name"].asString().c_str(), 528, 459, 76, 269);// ����

	FlushBatchDraw();// ��δ��ɵĻ�ͼ�������
	EndBatchDraw();// ������ͼ����
}

// ����Ʊ�ݴ���
void ticketShow() {
	loadimage(&ticketShowBG, "picture/ticketShow.jpg");// ����ͼƬ
	drawTicketShow();// ���ƴ���
	while (1) {
		msg = getmessage(EX_MOUSE);// ��ȡ�����Ϣ���������
		int x = msg.x;
		int y = msg.y;
		if (msg.lbutton == 1) {// �ж��������Ƿ���
			clickAnimate(x, y, drawTicketMSGWindow);
			jumpInterfact = 7;
			break;
		}
	}
}
