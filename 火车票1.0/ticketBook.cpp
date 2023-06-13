// ��Ʊ���� ������Ϊ6
#include "ticketBook.h"

IMAGE ticketBG;
IMAGE DeleteButton;
extern int jumpInterfact;//ҳ������
Json::Value peopleMsg;// ����¼����˵���Ϣ
Json::Value result;// ѡƱ���˵���Ϣ
int arr[3] = { 0, 0, 0 };//��¼��ť״̬

// �����ַ���ת��Ϊutf-8�����ʽ���ַ���
std::string convertToUTF8(const std::wstring& input, const std::locale& loc) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(input);
}
// ��ԭʼ����ת��Ϊ���ַ���
std::wstring convertFromUTF8(const std::string& input, const std::locale& loc) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.from_bytes(input);
}

// ���ƶ�Ʊ����
void drawTicketBookWindow() {
	BeginBatchDraw();// ��ͣ��ͼ
	cleardevice();// ����
	putimage(0, 0, &ticketBG);
	settextcolor(BLACK);
	// ��Ⱦѡ�и���Ʊ������
	textCenter((result["trainNum"].asString()).c_str(), 38, 223, 58, 119);
	textCenter("��ݸ", 157, 223, 58, 117);
	textCenter((result["endPlace"].asString()).c_str(), 274, 223, 58, 92);
	textCenter((result["startTime"].asString()).c_str(), 366, 223, 58, 129);
	textCenter((result["endTime"].asString()).c_str(), 495, 223, 58, 120);
	textCenter((result["price"].asString()).c_str(), 615, 223, 58, 68);
	textCenter((result["voteNum"].asString()).c_str(), 682, 223, 58, 150);
	textCenter("��Ʊ", 832, 223, 58, 123);
	//��ȡ¼����˵���Ϣ�ļ�
	std::ifstream file("afterIsChoose.json");
	file >> peopleMsg;// ���ļ��ж�ȡ���ݵ�json������
	file.close();// �ر��ļ�

	// �� utf-8 �ı����ʽ���ļ� �� ��ȡ����
	//std::ifstream file("afterIsChoose.json");
	//if (file.is_open()) {
	//	// ��ȡ�ļ�����
	//	std::stringstream buffer;
	//	buffer << file.rdbuf();
	//	std::string fileContent = buffer.str();
	//	// ���ļ����ݴ�ԭʼ���루����GB2312��ת��Ϊ���ַ�����UTF-16��UTF-32��
	//	std::wstring wideContent = convertFromUTF8(fileContent, std::locale(""));  // �ɸ���ʵ�ʱ�����������locale
	//	// �����ַ���ת��ΪUTF-8����
	//	std::string utf8Content = convertToUTF8(wideContent, std::locale(""));
	//	// ʹ��JsonCpp����UTF-8���������
	//	Json::CharReaderBuilder reader;
	//	std::istringstream jsonStream(utf8Content);
	//	std::string errors;
	//	if (Json::parseFromStream(reader, jsonStream, &peopleMsg, &errors))
	//	file.close();
	//}

	// ��Ⱦ��ȡ����Ϣ
	int index = 0;
	for (auto const& obj : peopleMsg) {// ����ƥ����
		string name = obj["name"].asString();
		textCenter(name.c_str(), 157, 324 + index * 53, 53, 118);// ��һλ�˵�����
		textCenter((obj["identityCardNum"].asString()).c_str(), 273, 324 + index * 53, 53, 687);// ��һλ�˵����֤
		if (arr[index]) {
			putimage(36, 324 + index * 53, &DeleteButton);
		}
		index++;
	}
	FlushBatchDraw();// ��δ��ɵĻ�ͼ�������
	EndBatchDraw();// ������ͼ����
}

// ѡ����ɾ��
void selectDelete(int i) {
	arr[i] = ~arr[i];// ��ʱ�����Ӧλ�õ������0��-1 �Դ��жϸ����Ƿ�ѡ��
	// ���»�ͼ
	drawTicketBookWindow();
	for (int Index = 0; Index < 3; Index++) {// �����Ƿ�洢ѡ�е��˵���Ϣ
		if (arr[Index]) {
			putimage(35, 324 + Index * 53, &DeleteButton);//����Ϊɾ����ť
		}
	}
	// ����result������¼��ѡ�˵���Ϣ
	if (arr[i])
	{
		result["man"].append(peopleMsg[i]);
	}
	else {
		// ɾ��ѡ���˵���Ϣ
		for (unsigned int index = 0; index < result["man"].size(); index++) {
			if (result["man"][index] == peopleMsg[i]) {
				result["man"].removeIndex(index, &peopleMsg[i]);
				break;
			}
		}
	}
}

// ��Ʊ����
void ticketBookWindow() {
	loadimage(&ticketBG, "picture/voteChoose.png");// ����ͼƬ
	loadimage(&DeleteButton, "picture/delete.jpg", 118, 53, true);// ɾ��ͼ��
	drawTicketBookWindow();

	result["man"] = {};
	ExMessage msg;// ����һ����Ϣ�ṹ��
	while (1) {
		msg = getmessage(EX_MOUSE);// ��ȡ�����Ϣ���������
		int x = msg.x;
		int y = msg.y;
		if (msg.lbutton == 1) {// �ж��������Ƿ���
			clickAnimate(x, y, drawTicketBookWindow);
			if (x >= 40 && x <= 158 && y >= 324 && y < 380) {// ���µ�һ�����/ɾ����ť
				selectDelete(0);
			}
			else if (x >= 40 && x <= 158 && y >= 380 && y < 430) {// ���µڶ������/ɾ����ť
				selectDelete(1);
			}
			else if (x >= 40 && x <= 158 && y >= 430 && y <= 485) {// ���µ��������/ɾ����ť
				selectDelete(2);
			}
			else if (x >= 367 && x <= 454 && y >= 489 && y <= 540) {// ����ȷ����ť ����Ϣд���ļ�
				std::ofstream outFire("beChoose.json");// ���ļ�
				if (outFire.is_open()) {
					outFire << result.toStyledString();// д����Ϣ
					outFire.close();
				}
				jumpInterfact = 7;// ��ת�������������
				break;
			}
			else if (x >= 537 && x <= 625 && y >= 489 && y <= 540) {// ����ȡ����ť
				jumpInterfact = 5;// ���ظ���Ʊ��ѯҳ��
				break;
			}
			else if (x > 0 && x < 155 && y > 125 && y < 158) {
				jumpInterfact = 2;
				break;
			}
		}
	}

	if (jumpInterfact == 5) {
		SearchWindow();// ���ظ���Ʊ��ѯҳ��
	}
	else if (jumpInterfact == 2) {
		directoryWindow();
	}
	else if (jumpInterfact == 7) {// ��ת������������
		ticketMSGWindow();
	}
}