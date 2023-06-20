// ��Ʊ��ѯ���� ������Ϊ5
#include "search.h"

IMAGE chooseBG;// ����ͼƬ
Json::Value searchResult(Json::arrayValue);//�½�һ������洢����ƥ��Ľ��
extern Json::Value result;// ѡƱ���˵���Ϣ
string date = "2023-05-16", endplace = "GZ";// �û�Ҫ�����ڣ�����أ���λ���ͣ�
int sortChoose = 0, Choose = 0;// ��ѯ�������ʽ(����ʱ�䣬����ʱ�����۸�)
bool UpDown = true;// ������
unsigned int page = 0;// ѡƱ����
extern int jumpInterfact;//ҳ������

extern bool isRun;// ���ƶ�ʱ������
std::condition_variable cv;  // ���������������̵߳ȴ�
bool ready = false; // ��־λ����ʾ�Ƿ�����������߳�
std::mutex mtx;  // ������Դ������
std::mutex m;	// ���̻߳�����
std::mutex bubble_mutex;	// ð�����򻥳���

// ��utf-8�ַ�����ת��ΪANSI����
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

// ���������
void clickAnimate(int x, int y, void (*fun)()) {
	for (int i = 0; i < 10; i++)
	{
		BeginBatchDraw();// ��ͣ��ͼ
		fun();
		setlinecolor(BLACK);
		circle(x, y, i);
		FlushBatchDraw();// ��δ��ɵĻ�ͼ�������
		EndBatchDraw();// ������ͼ����
		Sleep(10);
	}
	flushmessage(); // ��������Ϣ������,������
	fun();
}

// ��ʼ���������Ʊ��
void randomVoteNum() {
	static bool randomcnt = true; // �ж��Ƿ�Ϊ��һ�������ֵƱ
	std::lock_guard<std::mutex> lock(mtx);  // ����
	//��json�ļ�
	std::ifstream file("����Ʊ.json");
	Json::Value root;// ����json����
	file >> root;// ���ļ��ж�ȡ���ݵ�json������
	file.close();// �ر��ļ�
	Json::Value newRoot(Json::arrayValue);
	for (auto const& obj : root) // ʹ�õ��������������е����ж���
	{
		Json::Value newObj = obj; // ���ƶ���ʹ����޸�
		int voteNum;
		// �������������ͷֲ���
		std::random_device rd;  // ���ڻ�ȡ�����������
		std::mt19937 gen(rd());  // Mersenne Twister ���������
		if (randomcnt) {// ����ǵ�һ�ε��ã������ֵ
			string ticketTime = obj["startTime"].asString();
			// ��ȡ��ǰʱ��
			time_t nowTime;
			time(&nowTime);
			// ת��Ϊ����ʱ��ṹ��
			struct tm local_time;
			localtime_s(&local_time, &nowTime);
			string times = std::to_string(local_time.tm_hour) + ":" + std::to_string(local_time.tm_min);
			if (timeSubtract(ticketTime, times) >= 0)// �����Ʊ�ķ���ʱ��С����ʵʱ����Ʊ��Ϊ0
			{
				voteNum = 0;
			}
			else
			{
				std::uniform_int_distribution<int> dist(0, 120);  // ���ȷֲ�������Χ��0��120
				// ���������
				voteNum = dist(gen);
			}
		}
		else {// ������ǵ�һ�ε��ã�����ÿ15s�������Ʊ����ȥ0-3��
			voteNum = newObj["voteNum"].asInt();
			if (voteNum > 0) {
				std::uniform_int_distribution<int> dist(0, 3);  // ���ȷֲ�������Χ��0��3
				int sub = dist(gen);
				voteNum -= sub;
				if (voteNum < 0) {
					voteNum = 0;
				}
			}
		}
		newObj["voteNum"] = voteNum;
		newRoot.append(newObj);// ��ƥ������������ӵ�������
	}
	randomcnt = false;
	// ����д���ĵ�
	/*std::ofstream outFire("����Ʊ.json");
	if (outFire.is_open()) {
		outFire << newRoot.toStyledString();
		outFire.close();
	}*/
	ofstream outFire("����Ʊ.json", std::ios::binary);
	if (outFire.is_open()) {
		Json::StreamWriterBuilder builder;
		builder.settings_["emitUTF8"] = true;
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(newRoot, &outFire);
		outFire.close();
	}
}

// ��json�ļ���������ƥ����
void match(string date, string endplace) {
	//��json�ļ�
	std::ifstream file("����Ʊ.json");
	Json::Value root;// ����json����
	file >> root;// ���ļ��ж�ȡ���ݵ�json������
	file.close();// �ر��ļ�

	if (root.isArray()) // ���JSON�ļ��Ƿ���һ������
	{
		searchResult.clear();
		// ����ƥ�����Ҫ��Ķ���
		for (auto const& obj : root) // ʹ�õ��������������е����ж���
		{
			if (obj.isObject()) // �������Ƿ���JSON����
			{
				if ( (date == obj["date"].asString()) && ( (endplace == obj["endPlace"].asString()) || (endplace == obj["endPlaceCname"].asString()) ) ) {//ƥ���������
					if (searchResult.isArray()) {
						searchResult.append(obj);// ��ƥ������������ӵ�������
					}
				}
			}
		}
	}
}

// ����ʱ���
double timeSubtract(string t1, string t2)
{
	//��ʱ���ַ���ת��Ϊʱ��ṹ��
	std::stringstream s1(t1), s2(t2);
	struct tm time1 = { 0 }, time2 = { 0 };
	char delimiter;
	s1 >> time1.tm_hour >> delimiter >> time1.tm_min;
	s2 >> time2.tm_hour >> delimiter >> time2.tm_min;
	// ��ȡ��ǰʱ��
	time_t now;
	time(&now);
	// ת��Ϊ����ʱ��ṹ��
	struct tm local_time;
	localtime_s(&local_time, &now);
	// ��������ʱ����
	time1.tm_year = local_time.tm_year;
	time1.tm_mon = local_time.tm_mon;
	time1.tm_mday = local_time.tm_mday;
	time2.tm_year = local_time.tm_year;
	time2.tm_mon = local_time.tm_mon;
	time2.tm_mday = local_time.tm_mday;
	// ��ʱ��ṹ��ת��Ϊʱ���
	time_t timestamp1 = mktime(&time1);
	time_t timestamp2 = mktime(&time2);
	// ����ʱ���
	double time_diff = difftime(timestamp2, timestamp1);
	return time_diff / 60;
}

// ���־������
void textCenter(const char* title, int x, int y, int hei, int wid) {
	int strWidth = textwidth(title);//��ȡ���ֵĿ��
	int strHeight = textheight(title);//��ȡ���ֵĸ߶�
	int strX = (wid - strWidth) / 2 + x;//ʵ�����ֵ����Ͻ�����x��ֵ
	int strY = (hei - strHeight) / 2 + y;//ʵ�����ֵ����Ͻ�����y��ֵ
	//�������
	setbkmode(TRANSPARENT);//�������ֵ��������Ϊ͸��
	outtextxy(strX, strY, title);//�����ť����
};

//ð������
void bubble_sort(Json::Value& arr, bool UD, int sortChoose) {
	int n = arr.size(); // ��ȡ����ĳ���
	// ð������Ļ�������ʵ��
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			Json::Value& obj1 = arr[j];
			Json::Value& obj2 = arr[j + 1];
			string t1 = obj1["startTime"].asString();// ��ȡǰһ��json�������ʼʱ��
			double run1 = timeSubtract(t1, obj1["endTime"].asString());// ��ȡǰһ�����������ʱ��
			double price1 = obj1["price"].asDouble();// ��ȡǰһ������ļ۸�
			string t2 = obj2["startTime"].asString();// ��ȡ��һ��json�������ʼʱ��
			double run2 = timeSubtract(t2, obj2["endTime"].asString());// ��ȡ��һ�����������ʱ��
			double price2 = obj2["price"].asDouble();// ��ȡ��һ������ļ۸�
			switch (sortChoose)
			{
			case 0:// ����ʱ��
				if (UD ? timeSubtract(t1, t2) <= 0 : timeSubtract(t1, t2) > 0) { // ��Ԫ���ʽ�ж��������ʵ��
					Json::Value temp = obj1;
					obj1 = obj2;
					obj2 = temp;
				}
				break;
			case 1://����ʱ��
				if (UD ? run2 - run1 <= 0 : run2 - run1 > 0) { // ��Ԫ���ʽ�ж��������ʵ��
					Json::Value temp = obj1;
					obj1 = obj2;
					obj2 = temp;
				}
				break;
			case 2:
				if (UD ? price2 - price1 <= 0 : price2 - price1 > 0) { // ��Ԫ���ʽ�ж��������ʵ��
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
	// ����������������id�ٸ�ֵ
	int id = 0;
	for (auto const& obj : searchResult) {// ����ƥ����
		Json::Value newObj = obj; // ���ƶ���ʹ����޸�
		newObj["id"] = id;
		searchResult[id] = newObj;
		id++;
	}
}

// �ػ����
void drawSearchWindow() {
	std::lock_guard<std::mutex> lock(bubble_mutex);  // ����
	BeginBatchDraw();// ��ͣ��ͼ
	cleardevice();// ���֮ǰ�������Դﵽ�������������
	// ���ر���ͼƬ
	loadimage(&chooseBG, "picture/choose.png", 960, 540, true);
	putimage(0, 0, &chooseBG);
	// �����������ʽ
	settextstyle(16, 0, "Consolas");
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	outtextxy(195, 173, endplace.c_str());// �������
	outtextxy(350, 170, date.c_str());// �������
	// ��ȡ���ݲ�ƥ��
	match(date, endplace);
	// ԭ�����ݱ������������ڶ�ƥ������������������
	bubble_sort(searchResult, UpDown, sortChoose);// ʹ��ð������
	settextstyle(16, 0, "Consolas");
	// ����ƥ�����Եõ�����Ʊ����
	for (auto const& obj : searchResult) {// ����ƥ����
		int voteNum = obj["voteNum"].asInt();// ��ȡƱ��
		int id = obj["id"].asInt();// ��ȡƱ��id������
		string state = "��Ʊ";
		if (voteNum == 0) {
			state = "��Ʊ";
		}
		settextcolor(BLACK);
		// ����ҳ�������Ӧ����������Ʊ����
		if (id / 6 == page) {
			textCenter(obj["trainNum"].asString().c_str(), 30, 252 + (id % 6) * 48, 50, 109);// json�������ʼʱ��
			textCenter("��ݸ", 139, 252 + (id % 6) * 48, 50, 105);
			textCenter(obj["endPlaceCname"].asString().c_str(), 244, 252 + (id % 6) * 48, 50, 86);// Ŀ�ĵ�
			textCenter(obj["startTime"].asString().c_str(), 330, 252 + (id % 6) * 48, 50, 100);
			textCenter(obj["endTime"].asString().c_str(), 429, 252 + (id % 6) * 48, 50, 118);// ����ʱ��
			textCenter(obj["price"].asString().c_str(), 547, 252 + (id % 6) * 48, 50, 70);// �۸�
			textCenter(std::to_string(voteNum).c_str(), 617, 252 + (id % 6) * 48, 50, 109);
			textCenter(state.c_str(), 727, 252 + (id % 6) * 48, 50, 124);
		}
	}
	string showPage = std::to_string(page + 1) + "/" + std::to_string((searchResult.size() / 6) + 1);// ��ʾ��ǰҳ������ҳ��
	textCenter(showPage.c_str(), 861, 137, 20, 93);
	FlushBatchDraw();// ��δ��ɵĻ�ͼ�������
	EndBatchDraw();// ������ͼ����
}

// �ַ�����ת��
char chTurn(ExMessage mmsg) {
	// �����������Ϣ���ص��ַ�����ת��
	BYTE keyboardState[256];
	WCHAR unicodeChar;
	UINT scanCode = (mmsg.lParam >> 16) & 0xFF;
	// ��ȡ����״̬
	GetKeyboardState(keyboardState);
	// �� WCHAR ת��Ϊ char
	char narrowChar = '1';
	// �����ֽ��ַ�����ת��Ϊ Unicode
	if (ToUnicode(mmsg.wParam, scanCode, keyboardState, &unicodeChar, 1, 0) > 0)
	{
		WideCharToMultiByte(CP_UTF8, 0, &unicodeChar, 1, &narrowChar, sizeof(narrowChar), nullptr, nullptr);
	}
	return narrowChar;
}

// �ж���������
void GetIMEString(HWND hWnd, string& str)
{
	HIMC hIMC = ImmGetContext(hWnd);//��ȡHIMC 
	if (hIMC)
	{
		//������˵��һ�£������롰�й���Ϊ�� 
		//�л����������뷨�����롰zhongguo��������ַ�������IME����ַ���
		//�������뷨�б���ѡ����ַ������й��������IME����ַ��� 
		static bool flag = false;//������ɱ�ǣ���������ʱ��IME����ַ�����Ϊ�գ���true��������ɺ�IME����ַ���Ϊ�գ���false 
		DWORD dwSize = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, NULL, 0); //��ȡIME���������ַ����ĳ��� 
		if (dwSize > 0)//���IME����ַ�����Ϊ�գ���û�д��󣨴�ʱdwSizeΪ��ֵ��������������ɱ��Ϊtrue 
		{
			if (flag == false)
			{
				flag = true;
			}
		}
		else if (dwSize == 0 && flag) //���IME����ַ���Ϊ�գ����ұ��Ϊtrue�����ȡIME����ַ��� 
		{
			int iSize; //IME����ַ����Ĵ�С 
			LPSTR pszMultiByte = NULL;//IME����ַ���ָ�� 
			int ChineseSimpleAcp = 936;//���ֽ�ת��ʱ���ĵı��� 
			WCHAR* lpWideStr = NULL;//���ֽ��ַ����� 
			dwSize = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);//��ȡIME����ַ����Ĵ�С 
			if (dwSize > 0) //���IME����ַ�����Ϊ�գ���û�д��� 
			{
				dwSize += sizeof(WCHAR);//��СҪ����NULL������ 
				//Ϊ��ȡIME����ַ�������ռ� 
				if (lpWideStr)
				{
					delete[]lpWideStr;
					lpWideStr = NULL;
				}
				lpWideStr = new WCHAR[dwSize];
				memset(lpWideStr, 0, dwSize); //��ս���ռ� 
				ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, lpWideStr, dwSize);//��ȡIME����ַ����������ȡ���ǿ��ֽ� 
				iSize = WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, NULL, 0, NULL, NULL);//���㽫IME����ַ���ת��ΪASCII��׼�ֽں�Ĵ�С 
				//Ϊת������ռ� 
				if (pszMultiByte)
				{
					delete[] pszMultiByte;
					pszMultiByte = NULL;
				}
				pszMultiByte = new char[iSize + 1];
				WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, pszMultiByte, iSize, NULL, NULL);//���ֽ�ת�� 
				pszMultiByte[iSize] = '\0';
				str += pszMultiByte;//��ӵ�string�� 
				//�ͷſռ� 
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
		ImmReleaseContext(hWnd, hIMC);//�ͷ�HIMC 
	}
}

// ������ʵ�֣�����̽���
void inputOutput(string& toWrite, void (*drawWindow)(), int x1, int y1, int x2, int y2) {
	static bool CorE = true;		// �ж���Ӣ������
	while (1)
	{
		SHORT my_CAPITAL = GetKeyState(VK_CAPITAL);	// ��Сд
		bool bigSmall = my_CAPITAL;
		ExMessage mmsg1;	// ��Ϣ�ṹ��
		mmsg1 = getmessage(EX_KEY | EX_MOUSE);	// ��ȡ������Ϣ�������������
		BeginBatchDraw();	// ��ͣ��ͼ
		if (CorE) {	// ��������
			HWND hWnd = GetHWnd();
			HDC hdc = GetDC(hWnd);
			GetIMEString(hWnd, toWrite);
			if (mmsg1.message == WM_KEYUP) {
				bool isBreak = false;
				switch (mmsg1.vkcode)
				{
				case 0x10:		// ����SHIFT��
					CorE = false;
					break;
				case 0x08:		// ����Backspace��
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
				case 0x0D:		// ����Enter��
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
				// ���������Ϣ���ַ�����
				if (mmsg1.vkcode == 0x11) {// ���ctrl��
					continue;
				}
				else if (mmsg1.vkcode == 0x10) {// ���SHIFT��
					bool isUp = true;
					while (1)
					{
						ExMessage mmsg2 = getmessage(EX_KEY);	// ��ȡ������Ϣ
						if (mmsg2.message == WM_KEYUP) {	// ����Ǽ��̵�����Ϣ
							if (mmsg2.vkcode == 0x10) {	// �����Ϣ��shift��������
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
							drawWindow();// ��ͼ����
							FlushBatchDraw();// ��δ��ɵĻ�ͼ�������
							EndBatchDraw();// ������ͼ����
						}
					}
					continue;
				}
				else if (mmsg1.vkcode == 0x08) {	// ���Backspace��
					//toWrite = toWrite.substr(0, toWrite.length() - 1);
					if (toWrite.length() > 0)
					{
						if (toWrite.at(toWrite.length() - 1) & 0x8000)
							toWrite.erase(toWrite.end() - 1);
						toWrite.erase(toWrite.end() - 1);
					}
				}
				else if (mmsg1.vkcode == 0x0D) {	// ���Enter��
					break;
				}
				else if (mmsg1.vkcode == 0x14 || mmsg1.vkcode == 0x12) {	// ���caps������alt��
					continue;
				}
				else if (mmsg1.vkcode == 0x09) {
					toWrite = toWrite + "  ";
				}
				else if (narrowChar > 60 && narrowChar <= 122 && bigSmall) {	// ���caps���ڴ�д״̬
					toWrite = toWrite + (char)toupper(narrowChar);
				}
				else {
					toWrite = toWrite + narrowChar;
				}
			}
		}
		drawWindow();// ��ͼ����
		FlushBatchDraw();// ��δ��ɵĻ�ͼ�������
		EndBatchDraw();// ������ͼ����
		// ������������������ѭ��
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

// ��ʱ����ÿ5��Ʊ���������0-3�ţ���ģ�������˿͹�Ʊ�������
void timerFunction(int seconds) {
	randomVoteNum();
	while (isRun) {
		std::unique_lock<std::mutex> lk(m); // ��ȡ������
		cv.wait(lk, []() {return ready; }); // �ȴ����̵߳��ź�
		if (!isRun) {// �������رգ���ֱ�ӽ������߳�
			break;
		}
		randomVoteNum();	// Ʊ��������٣���ģ��
		drawSearchWindow();		// ���´���
		std::this_thread::sleep_for(std::chrono::seconds(seconds));	// ��ʱ����ͣ5��
	}
}

// ����Ʊ��ѯҳ��
void SearchWindow() {
	settextcolor(BLACK);
	drawSearchWindow();// ��ʼ������

	// �������߳�
	{
		std::lock_guard<std::mutex> lk(m); // ��ȡ������
		ready = true; // ����־λ��Ϊ true����ʾ�����������߳�
		cv.notify_one(); // �����źţ��������߳�
	}

	ExMessage msg;// ����һ����Ϣ�ṹ��
	//while����ѭ���������ͼ����¼��Ե��ú���
	while (1)
	{
		msg = getmessage(EX_MOUSE);// ��ȡ�����Ϣ���������
		int x = msg.x;
		int y = msg.y;
		if(msg.lbutton == 1){// �ж��������Ƿ���
			clickAnimate(x, y, drawSearchWindow);
			if (x >= 118 && x <= 244 && y >= 158 && y <= 200) { // ���Ŀ�ĵ������
				page = 0;
				inputOutput(endplace, drawSearchWindow, 118, 158, 244, 200);
			}
			else if (x >= 250 && x <= 452 && y >= 158 && y <= 200) {// ������������
				page = 0;
				inputOutput(date, drawSearchWindow, 250, 158, 452, 200);
			}
			else if (x >= 559 && x <= 584 && y >= 160 && y <= 178) {// �������ʱ������������
				sortChoose = 0;
				UpDown = true;
				drawSearchWindow();
			}
			else if (x >= 559 && x <= 584 && y >= 180 && y <= 199) {// �������ʱ������Ľ����
				sortChoose = 0;
				UpDown = false;
				drawSearchWindow();
			}
			else if (x >= 692 && x <= 718 && y >= 160 && y <= 178) {// �������ʱ������������
				sortChoose = 1;
				UpDown = true;
				drawSearchWindow();
			}
			else if (x >= 628 && x <= 718 && y >= 180 && y <= 199) {// �������ʱ������Ľ����
				sortChoose = 1;
				UpDown = false;
				drawSearchWindow();
			}
			else if (x >= 824 && x <= 850 && y >= 160 && y <= 178) {// ����۸�����������
				sortChoose = 2;
				UpDown = true;
				drawSearchWindow();
			}
			else if (x >= 824 && x <= 850 && y >= 180 && y <= 199) {// �����Ʊ�۸�����Ľ����
				sortChoose = 2;
				UpDown = false;
				drawSearchWindow();
			}
			else if (x >= 862 && x <= 909 && y >= 161 && y <= 196) {// ���ǰһҳ
				if (page-- >= 1)
					drawSearchWindow();
				else
					page = 1;
			}
			else if (x > 909 && x <= 956 && y >= 161 && y <= 196) {// ���ǰһҳ
				if (++page < ((searchResult.size() / 6) + 1))
					drawSearchWindow();
				else
					page--;
			}
			else if (x > 861 && x <= 940 && y >= 258 && y <= 298) {// �����һ����Ʊ��ť
				Choose = 0;// ѡ���˵�ǰ��һ�Ÿ���Ʊ
				if (searchResult[Choose + page * 6]["voteNum"].asInt() != 0) {
					jumpInterfact = 6;// ѡƱ��������
					break;
				}
			}
			else if (x > 861 && x <= 940 && y >= 305 && y <= 345) {// ����ڶ�����Ʊ��ť
				Choose = 1;// ѡ���˵�ǰ�ڶ��Ÿ���Ʊ
				if (searchResult[Choose + page * 6]["voteNum"].asInt() != 0) {
					jumpInterfact = 6;// ѡƱ��������
					break;
				}
			}
			else if (x > 861 && x <= 940 && y >= 353 && y <= 393) {// �����������Ʊ��ť
				Choose = 2;// ѡ���˵�ǰ�����Ÿ���Ʊ
				if (searchResult[Choose + page * 6]["voteNum"].asInt() != 0) {
					jumpInterfact = 6;// ѡƱ��������
					break;
				}
			}
			else if (x > 861 && x <= 940 && y >= 400 && y <= 440) {// ������ĸ���Ʊ��ť
				Choose = 3;// ѡ���˵�ǰ�����Ÿ���Ʊ
				if (searchResult[Choose + page * 6]["voteNum"].asInt() != 0) {
					jumpInterfact = 6;// ѡƱ��������
					break;
				}
			}
			else if (x > 861 && x <= 940 && y >= 448 && y <= 488) {// ����������Ʊ��ť
				Choose = 4;// ѡ���˵�ǰ�����Ÿ���Ʊ
				if (searchResult[Choose + page * 6]["voteNum"].asInt() != 0) {
					jumpInterfact = 6;// ѡƱ��������
					break;
				}
			}
			else if (x > 861 && x <= 940 && y >= 500 && y <= 540) {// �����������Ʊ��ť
				Choose = 5;// ѡ���˵�ǰ�����Ÿ���Ʊ
				if (searchResult[Choose + page * 6]["voteNum"].asInt() != 0) {
					jumpInterfact = 6;// ѡƱ��������
					break;
				}
			}
			else if (x > 0 && x <= 155 && y >= 126 && y <= 156) {// �������Ŀ¼��ť
				jumpInterfact = 2;
				break;
			}
		}
	}

	result = searchResult[Choose + page * 6];
	ready = false; // ����־λ��Ϊ false����ʾ���߳���Ҫ�ٴεȴ����̵߳��ź�
}
