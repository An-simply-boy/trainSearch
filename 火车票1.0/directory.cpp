// Ŀ¼���� ������Ϊ2
#include"directory.h"

IMAGE directoryBG;
extern int jumpInterfact;// ������ת����

void drawDirectoryWindow() {
	BeginBatchDraw();// ��ͣ��ͼ
	cleardevice();// ����
	putimage(0, 0, &directoryBG);

	FlushBatchDraw();// ��δ��ɵĻ�ͼ�������
	EndBatchDraw();// ������ͼ����
}

void directoryWindow() {
	loadimage(&directoryBG, "picture/catalogue.jpg");
	drawDirectoryWindow();

	ExMessage msg;
	while (1)
	{
		msg = getmessage(EX_MOUSE);// ��ȡ�����Ϣ���������
		int x = msg.x;
		int y = msg.y;
		if (msg.lbutton == 1) {// �ж��������Ƿ���
			clickAnimate(x, y, drawDirectoryWindow);
			if (x >= 520 && x <= 733 && y >= 222 && y <= 287) { // �����Ϣ¼�밴ť
				jumpInterfact = 3;
				break;
			}
			else if (x >= 734 && x <= 946 && y >= 222 && y <= 287) {// �����Ϣ����ť
				jumpInterfact = 4;
				break;
			}
			else if (x >= 520 && x <= 733 && y >= 288 && y <= 352) {// �����Ʊ��ѯ��ť
				jumpInterfact = 5;
				break;
			}
			else if (x >= 734 && x <= 946 && y >= 288 && y <= 352) {// �����������ť
				jumpInterfact = 7;
				break;
			}
		}
	}

	if (jumpInterfact == 5) {
		SearchWindow();// ��ת����Ʊ��ѯ����
	}
	else if (jumpInterfact == 7) {
		ticketMSGWindow();// ��ת�������������
	}
	else if (jumpInterfact == 4) {
		peopleListWindow();// ��ת����Ϣ������
	}
	else if (jumpInterfact == 3) {
		addPeopleWindow();// ��ת����Ϣ¼�봰��
	}
}