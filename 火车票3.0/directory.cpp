// Ŀ¼���� ������Ϊ2
#include"directory.h"

IMAGE directoryBG;
extern int jumpInterfact;// ������ת����
extern bool isRun;

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
		msg = getmessage(EX_MOUSE | EX_KEY);// ��ȡ�����Ϣ�ͼ�����Ϣ���������
		if (msg.lbutton == 1) {// �ж��������Ƿ���
			int x = msg.x;
			int y = msg.y;
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
		else if (msg.vkcode == 0x1B) {
			isRun = false;
			break;
		}
	}
}