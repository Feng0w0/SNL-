#include"hyf.h"
/*******************��������************************/
stack<string> signStack;	//����ջ
stack<TreeNode*> opSignStack;	//������ջ
stack<TreeNode*> opNumStack;	//������ջ
 stack<TreeNode**>	syntaxTreeStack;		//�﷨��ջ
vector<vector<string>> productSet;	//�洢���еĲ���ʽ
vector<vector<string>> predictSet;	//�洢���в���ʽ��predict��
vector<vector<int>> LL1Table;	//LL1����
set<string> VT;	//�洢�����ռ���
set<string> VN;	//�洢���з��ռ���
string S;	//��ʼ��
TreeNode* currentP;	//�洢��ǰ�ڵ�
TreeNode* saveP;		//���浱ǰָ���¼���������ڵ��ָ��
TreeNode* saveFuncP;	//����ָ��ĳһ�ڵ��ָ��
DeckEnum* temp;
bool getExpResult = true;
bool getExpResult2;	//��������
int expflag = 0;	//���ڼ�������
extern Token token;
string arrayLexType_hyf[] = { "ID","IF","BEGIN","INTC","END","PLUS","MINUS","TIMES","OVER","EQ","LT","LMIDPAREN","RMIDPAREN" ,"DOT"
	,"TYPE","VAR", "PROCEDURE","PROGRAM","SEMI","INTEGER","CHAR","ARRAY","RECORD","UNDERANGE"
	,"OF","COMMA","LPAREN","RPAREN","ENDWH","WHILE","RETURN","READ","WRITE","ASSIGN","THEN","FI"
	,"ELSE","DO"
};


/*******************��������************************/
TreeNode *Parse() {
	
	CreatLL1Table();
	S = productSet[0][0];	//��ʼ����ֵ
	signStack.push(S);
	TreeNode* proK;	//���ڵ�
	proK = new TreeNode();
	(*proK).nodekind = ProK;
	(*proK).lineno = 1;
	syntaxTreeStack.push(&(*proK).child[2]);
	syntaxTreeStack.push(&(*proK).child[1]);
	syntaxTreeStack.push(&(*proK).child[0]);	//�����ڵ�Ķ���ָ��ѹ���﷨��ջ

	//����һ��Token����lineno��¼�����к�
	ifstream in("token.txt");
	string line;
	getline(in, line);
	istringstream inword(line);
	inword >> token.linsShow;
	int lex;
	inword >> lex;
	token.lex = (LexType)lex;
	inword >> token.sem;
	int lineno = token.linsShow;
	while (!signStack.empty()) {
		string top = signStack.top();
		int num = GetIndex(VN, top);	//��top���ռ����᷵��-1;
		if (num != -1) {	//�����ռ���
			int x = GetIndex(VN, top);
			int y = GetIndex(VT, arrayLexType_hyf[(int)token.lex]);
			int pnum = LL1Table[x][y];		//ջ�����ռ�����Ϊ�кţ�Token�Ĵʷ���ϢΪ�кţ�ȡLL(1)���������ݣ��õ�����ʽ���
			if (pnum == 0) {
				string a = to_string(token.linsShow) + ":" + arrayLexType_hyf[(int)token.lex] + "������" + top + "��predict��";
				InputError(a, "error.txt");
				//cout << token.linsShow << ":" << arrayLexType_hyf[(int)token.lex] << "������" << top << "��predict��";
				exit(0);
			}
			signStack.pop();
			Predict(pnum);
		}
		else {					//���ռ���
			if (top == arrayLexType_hyf[(int)token.lex]) {	//ջ����tokenƥ��
				signStack.pop();
				//����һ��Token������lineon��¼�к�
				getline(in, line);
				istringstream inword(line);
				inword >> token.linsShow;
				int lex;
				inword >> lex;
				token.lex = (LexType)lex;
				inword >> token.sem;
				lineno = token.linsShow;
			}
			else {	
				string a = to_string(token.linsShow) + ":ջ��(" + top + ")���뵱ǰtoken(" + arrayLexType_hyf[(int)token.lex] + ")ƥ��";
				InputError(a, "error.txt");
				exit(0);
			}
		}
	}
	if (in.eof()) {
		Input(proK,"treeLL1.txt");
	}
	else {
		string a= "�ļ���ǰ����";
		InputError(a, "error.txt");
		//cout << "�ļ���ǰ����";
		exit(0);
	}
	in.close();
	return proK;
}

void Input(TreeNode* root,string path) {
	ofstream ouput;
	ouput.open(path);
	stack<TreeNode*> inputSort;
	inputSort.push(root);
	while (!inputSort.empty()) {
		TreeNode* t = inputSort.top();
		inputSort.pop();
		ouput << t->child[0] << " " << t->child[1] << " " << t->child[2] << " ";
		ouput << t->sibling << " ";
		ouput << t->lineno << " ";
		ouput << t->nodekind << " ";
		ouput << t->kind.dec << " ";
		ouput << t->idnum << " ";
		for (int i = 0; i < t->idnum; i++) {
			ouput << t->name[i] << " ";
		}
		ouput << t->type_name << " ";
		ouput << t->attr.arrayAttr.childType << " " << t->attr.arrayAttr.low << " " << t->attr.arrayAttr.up << " ";
		ouput << t->attr.expAttr.op << " " << t->attr.expAttr.type << " " << t->attr.expAttr.val << " " << t->attr.expAttr.varkind << " ";
		ouput << t->attr.procAttr.paramt << endl;
		if (t->sibling != NULL) {
			inputSort.push(t->sibling);
		}
		for (int i = 2; i >= 0; i--) {
			if (t->child[i] != NULL) {
				inputSort.push(t->child[i]);
			}
		}
	}
	ouput.close();
}
void InputError(string errorInfo,string path) {
	ofstream ouput;
	ouput.open(path);
	ouput << errorInfo;
	ouput.close();
}
void Predict(int pnum) {
	if (pnum == 1)process1();
	else if (pnum == 2)process2();
	else if (pnum == 3)process3();
	else if (pnum == 4)process4();
	else if (pnum == 5)process5();
	else if (pnum == 6)process6();
	else if (pnum == 7)process7();
	else if (pnum == 8)process8();
	else if (pnum == 9)process9();
	else if (pnum == 10)process10();
	else if (pnum == 11)process11();
	else if (pnum == 12)process12();
	else if (pnum == 13)process13();
	else if (pnum == 14)process14();
	else if (pnum == 15)process15();
	else if (pnum == 16)process16();
	else if (pnum == 17)process17();
	else if (pnum == 18)process18();
	else if (pnum == 19)process19();
	else if (pnum == 20)process20();
	else if (pnum == 21)process21();
	else if (pnum == 22)process22();
	else if (pnum == 23)process23();
	else if (pnum == 24)process24();
	else if (pnum == 25)process25();
	else if (pnum == 26)process26();
	else if (pnum == 27)process27();
	else if (pnum == 28)process28();
	else if (pnum == 29)process29();
	else if (pnum == 30)process30();
	else if (pnum == 31)process31();
	else if (pnum == 32)process32();
	else if (pnum == 33)process33();
	else if (pnum == 34)process34();
	else if (pnum == 35)process35();
	else if (pnum == 36)process36();
	else if (pnum == 37)process37();
	else if (pnum == 38)process38();
	else if (pnum == 39)process39();
	else if (pnum == 40)process40();
	else if (pnum == 41)process41();
	else if (pnum == 42)process42();
	else if (pnum == 43)process43();
	else if (pnum == 44)process44();
	else if (pnum == 45)process45();
	else if (pnum == 46)process46();
	else if (pnum == 47)process47();
	else if (pnum == 48)process48();
	else if (pnum == 49)process49();
	else if (pnum == 50)process50();
	else if (pnum == 51)process51();
	else if (pnum == 52)process52();
	else if (pnum == 53)process53();
	else if (pnum == 54)process54();
	else if (pnum == 55)process55();
	else if (pnum == 56)process56();
	else if (pnum == 57)process57();
	else if (pnum == 58)process58();
	else if (pnum == 59)process59();
	else if (pnum == 60)process60();
	else if (pnum == 61)process61();
	else if (pnum == 62)process62();
	else if (pnum == 63)process63();
	else if (pnum == 64)process64();
	else if (pnum == 65)process65();
	else if (pnum == 66)process66();
	else if (pnum == 67)process67();
	else if (pnum == 68)process68();
	else if (pnum == 69)process69();
	else if (pnum == 70)process70();
	else if (pnum == 71)process71();
	else if (pnum == 72)process72();
	else if (pnum == 73)process73();
	else if (pnum == 74)process74();
	else if (pnum == 75)process75();
	else if (pnum == 76)process76();
	else if (pnum == 77)process77();
	else if (pnum == 78)process78();
	else if (pnum == 79)process79();
	else if (pnum == 80)process80();
	else if (pnum == 81)process81();
	else if (pnum == 82)process82();
	else if (pnum == 83)process83();
	else if (pnum == 84)process84();
	else if (pnum == 85)process85();
	else if (pnum == 86)process86();
	else if (pnum == 87)process87();
	else if (pnum == 88)process88();
	else if (pnum == 89)process89();
	else if (pnum == 90)process90();
	else if (pnum == 91)process91();
	else if (pnum == 92)process92();
	else if (pnum == 93)process93();
	else if (pnum == 94)process94();
	else if (pnum == 95)process95();
	else if (pnum == 96)process96();
	else if (pnum == 97)process97();
	else if (pnum == 98)process98();
	else if (pnum == 99)process99();
	else if (pnum == 100)process100();
	else if (pnum == 101)process101();
	else if (pnum == 102)process102();
	else if (pnum == 103)process103();
	else if (pnum == 104)process104();
	else;
}

void CreatLL1Table() {
	string line;	//��¼ÿһ���ַ���
	vector<string> words;	//�洢ÿһ���ַ���������е���
	//��product.txt�ļ����뵽productSet�����ļ��ڴ洢���еĲ���ʽ,ͨ�����ļ����Ի�����еķ��ռ���
	ifstream in("product.txt");
	if (in) {
		while (getline(in, line)) {
			//cout << line << endl;
			istringstream inword(line);
			string word;	//��¼����
			int i = 0;		//��¼������������Եڶ�������������������һ�����ʼ��뼯��VN
			while (inword >> word) {
				i++;
				if (i == 1) {
					VN.insert(word);
					words.push_back(word);
				}
				else if (i > 2) {
					words.push_back(word);
				}
			}
			productSet.push_back(words);
			words.clear();
		}
	}
	else {
		cerr << "���ļ�������";
	}
	in.close();
	//��predict.txt�ļ����뵽predictSet�����ļ��ڴ洢���еĲ���ʽ��predict��,ͨ�����ļ����Ի�����е��ռ���
	in.open("predict.txt");
	if (in) {
		while (getline(in, line)) {
			//cout << line << endl;
			istringstream inword(line);
			string word;	//��¼����
			int i = 0;		//��¼������������һ�����ʼ��뼯��VT
			while (inword >> word) {
				i++;
				words.push_back(word);
				if (i != 1) {
					VT.insert(word);
				}
			}
			predictSet.push_back(words);
			words.clear();
		}
	}
	else {
		cerr << "���ļ�������";
	}
	in.close();
	//���������ȡ�������з��ռ������ռ�������LL1����Ȼ��ͨ��predictSet���Ծ�����и�ֵ
	int m = VN.size(), n = VT.size();
	LL1Table = vector<vector<int>>(m, vector<int>(n, 0));
	for (int i = 0; i < predictSet.size(); i++) {
		int num = stoi(predictSet[i][0]);
		string s = productSet[num-1][0];
		int x = GetIndex(VN, s);
		for (int j = 1; j < predictSet[i].size(); j++) {
			int y = GetIndex(VT, predictSet[i][j]);
			LL1Table[x][y] = num;
		}
	}
}

int Priosoty(LexType op) {
	if (op == END) {
		return 0;
	}
	else if (op == LT || op == EQ) {
		return 1;
	}
	else if (op == PLUS || op == MINUS) {
		return 2;
	}
	else if (op == TIMES || op == OVER) {
		return 3;
	}
	else {
		string a = to_string(token.linsShow)+":û��" + arrayLexType_hyf[(int)token.lex] + "�����";
		InputError(a, "error.txt");
		//cout << "û��" << arrayLexType_hyf[(int)token.lex] << "�����";
		exit(0);
	}
}

int GetIndex(set<string> &A, string s) {
	int i = 0;
	for (set<string>::iterator begin = A.begin(); begin != A.end(); begin++, i++) {
		if (*begin == s) {
			return i;
		}
	}
	return -1;	//���ַ���s���ڼ�����ʱ����-1
}
void process1() {
	for (int i = productSet[0].size()-1; i >= 1; i--) {
		signStack.push(productSet[0][i]);
	}
}
void process2() {
	for (int i = productSet[1].size() - 1; i >= 1; i--) {
		signStack.push(productSet[1][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = PheadK;
	(*currentP).lineno = token.linsShow;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
}
void process3() {
	for (int i = productSet[2].size() - 1; i >= 1; i--) {
		signStack.push(productSet[2][i]);
	}
	(*currentP).idnum++;
	(*currentP).name.push_back(token.sem);
}
void process4() {
	for (int i = productSet[3].size() - 1; i >= 1; i--) {
		signStack.push(productSet[3][i]);
	}
}
void process5(){}
void process6() {
	for (int i = productSet[5].size() - 1; i >= 1; i--) {
		signStack.push(productSet[5][i]);
	}
}
void process7() {
	for (int i = productSet[6].size() - 1; i >= 1; i--) {
		signStack.push(productSet[6][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = TypeK;
	(*currentP).lineno = token.linsShow;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).sibling = new TreeNode();
	//(*currentP).child[0] = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
	syntaxTreeStack.push(&(*currentP).child[0]);
	
}
void process8() {
	for (int i = productSet[7].size() - 1; i >= 1; i--) {
		signStack.push(productSet[7][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = DecK;
	(*currentP).lineno = token.linsShow;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).sibling = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
}
void process9() {
	syntaxTreeStack.pop();
}
void process10() {
	for (int i = productSet[9].size() - 1; i >= 1; i--) {
		signStack.push(productSet[9][i]);
	}
}
void process11() {
	for (int i = productSet[10].size() - 1; i >= 1; i--) {
		signStack.push(productSet[10][i]);
	}
	(*currentP).idnum++;
	(*currentP).name.push_back(token.sem);
}
void process12() {
	for (int i = productSet[11].size() - 1; i >= 1; i--) {
		signStack.push(productSet[11][i]);
	}
	temp = &((*currentP).kind.dec);
}
void process13() {
	for (int i = productSet[12].size() - 1; i >= 1; i--) {
		signStack.push(productSet[12][i]);
	}
	temp = &((*currentP).kind.dec);
}
void process14() {
	for (int i = productSet[13].size() - 1; i >= 1; i--) {
		signStack.push(productSet[13][i]);
	}
	(*currentP).kind.dec = IdK;
	(*currentP).type_name = token.sem;
	(*currentP).idnum++;
	(*currentP).name.push_back(token.sem);
}
void process15() {
	for (int i = productSet[14].size() - 1; i >= 1; i--) {
		signStack.push(productSet[14][i]);
	}
	(*temp) = IntegerK;
}
void process16() {
	for (int i = productSet[15].size() - 1; i >= 1; i--) {
		signStack.push(productSet[15][i]);
	}
	(*temp) = CharK;
}
void process17() {
	for (int i = productSet[16].size() - 1; i >= 1; i--) {
		signStack.push(productSet[16][i]);
	}
}
void process18() {
	for (int i = productSet[17].size() - 1; i >= 1; i--) {
		signStack.push(productSet[17][i]);
	}
}
void process19() {
	for (int i = productSet[18].size() - 1; i >= 1; i--) {
		signStack.push(productSet[18][i]);
	}
	(*temp) = ArrayK;
	temp = &((*currentP).attr.arrayAttr.childType);
}
void process20() {
	for (int i = productSet[19].size() - 1; i >= 1; i--) {
		signStack.push(productSet[19][i]);
	}
	(*currentP).attr.arrayAttr.low = stoi(token.sem);
}
void process21() {
	for (int i = productSet[20].size() - 1; i >= 1; i--) {
		signStack.push(productSet[20][i]);
	}
	(*currentP).attr.arrayAttr.up = stoi(token.sem);
}
void process22() {
	for (int i = productSet[21].size() - 1; i >= 1; i--) {
		signStack.push(productSet[21][i]);
	}
	(*temp) = RecordK;
	saveP = currentP;
	//(*currentP).child[0] = new TreeNode();
	syntaxTreeStack.push(&(*currentP).child[0]);
}
void process23() {
	for (int i = productSet[22].size() - 1; i >= 1; i--) {
		signStack.push(productSet[22][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = DecK;
	(*currentP).lineno = token.linsShow;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	temp = &((*currentP).kind.dec);
	//(*currentP).sibling = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
}
void process24() {
	for (int i = productSet[23].size() - 1; i >= 1; i--) {
		signStack.push(productSet[23][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = DecK;
	(*currentP).lineno = token.linsShow;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).sibling = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
}
void process25() {
	syntaxTreeStack.pop();
	currentP = saveP;
}
void process26() {
	for (int i = productSet[25].size() - 1; i >= 1; i--) {
		signStack.push(productSet[25][i]);
	}
}
void process27() {
	for (int i = productSet[26].size() - 1; i >= 1; i--) {
		signStack.push(productSet[26][i]);
	}
	(*currentP).idnum++;
	(*currentP).name.push_back(token.sem);
}
void process28() {}
void process29() {
	for (int i = productSet[28].size() - 1; i >= 1; i--) {
		signStack.push(productSet[28][i]);
	}
}
void process30(){}
void process31() {
	for (int i = productSet[30].size() - 1; i >= 1; i--) {
		signStack.push(productSet[30][i]);
	}
}
void process32() {
	for (int i = productSet[31].size() - 1; i >= 1; i--) {
		signStack.push(productSet[31][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = VarK;
	(*currentP).lineno = token.linsShow;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).sibling = new TreeNode();
	//(*currentP).child[0] = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
	syntaxTreeStack.push(&(*currentP).child[0]);
}
void process33() {
	for (int i = productSet[32].size() - 1; i >= 1; i--) {
		signStack.push(productSet[32][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = DecK;
	(*currentP).lineno = token.linsShow;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).sibling = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
}
void process34() {
	syntaxTreeStack.pop();
}
void process35() {
	for (int i = productSet[34].size() - 1; i >= 1; i--) {
		signStack.push(productSet[34][i]);
	}
}
void process36() {
	for (int i = productSet[35].size() - 1; i >= 1; i--) {
		signStack.push(productSet[35][i]);
	}
	(*currentP).idnum++;
	(*currentP).name.push_back(token.sem);
}
void process37() {}
void process38() {
	for (int i = productSet[37].size() - 1; i >= 1; i--) {
		signStack.push(productSet[37][i]);
	}
}
void process39(){}
void process40() {
	for (int i = productSet[39].size() - 1; i >= 1; i--) {
		signStack.push(productSet[39][i]);
	}
}
void process41() {
	for (int i = productSet[40].size() - 1; i >= 1; i--) {
		signStack.push(productSet[40][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = ProcDecK;
	(*currentP).lineno = token.linsShow;
	TreeNode** t = syntaxTreeStack.top();
	saveFuncP = currentP;
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).sibling = new TreeNode();
	//(*currentP).child[2] = new TreeNode();
	//(*currentP).child[1] = new TreeNode();
	//(*currentP).child[0] = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
	syntaxTreeStack.push(&(*currentP).child[2]);
	syntaxTreeStack.push(&(*currentP).child[1]);
	syntaxTreeStack.push(&(*currentP).child[0]);
}
void process42() {}
void process43() {
	for (int i = productSet[42].size() - 1; i >= 1; i--) {
		signStack.push(productSet[42][i]);
	}
}
void process44() {
	for (int i = productSet[43].size() - 1; i >= 1; i--) {
		signStack.push(productSet[43][i]);
	}
	(*currentP).idnum++;
	(*currentP).name.push_back(token.sem);
}
void process45() {
	syntaxTreeStack.pop();
}
void process46() {
	for (int i = productSet[45].size() - 1; i >= 1; i--) {
		signStack.push(productSet[45][i]);
	}
}
void process47() {
	for (int i = productSet[46].size() - 1; i >= 1; i--) {
		signStack.push(productSet[46][i]);
	}
}
void process48() {
	syntaxTreeStack.pop();
	currentP=saveFuncP;
	temp = &((*currentP).kind.dec);
}
void process49() {
	for (int i = productSet[48].size() - 1; i >= 1; i--) {
		signStack.push(productSet[48][i]);
	}
}
void process50() {
	for (int i = productSet[49].size() - 1; i >= 1; i--) {
		signStack.push(productSet[49][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = DecK;
	(*currentP).lineno = token.linsShow;
	(*currentP).attr.procAttr.paramt = Valparamtype;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).sibling = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
}
void process51() {
	for (int i = productSet[50].size() - 1; i >= 1; i--) {
		signStack.push(productSet[50][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = DecK;
	(*currentP).lineno = token.linsShow;
	(*currentP).attr.procAttr.paramt = Varparamtype;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).sibling = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
}
void process52() {
	for (int i = productSet[51].size() - 1; i >= 1; i--) {
		signStack.push(productSet[51][i]);
	}
	(*currentP).idnum++;
	(*currentP).name.push_back(token.sem);
}
void process53() {}
void process54() {
	for (int i = productSet[53].size() - 1; i >= 1; i--) {
		signStack.push(productSet[53][i]);
	}
}
void process55() {
	for (int i = productSet[54].size() - 1; i >= 1; i--) {
		signStack.push(productSet[54][i]);
	}
}
void process56() {
	for (int i = productSet[55].size() - 1; i >= 1; i--) {
		signStack.push(productSet[55][i]);
	}
}
void process57() {
	for (int i = productSet[56].size() - 1; i >= 1; i--) {
		signStack.push(productSet[56][i]);
	}
	/*TreeNode* p = proK->child[1], * q = proK;
	while (p->sibling != NULL) {
		q = p;
		p = p->sibling;
	}
	delete p;
	if (q == proK)q->child[1] = NULL;
	else q->sibling = NULL;*/
	
	syntaxTreeStack.pop();
	currentP = new TreeNode();
	(*currentP).nodekind = StmLK;
	(*currentP).lineno = token.linsShow;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).child[0] = new TreeNode();
	syntaxTreeStack.push(&(*currentP).child[0]);
}
void process58() {
	for (int i = productSet[57].size() - 1; i >= 1; i--) {
		signStack.push(productSet[57][i]);
	}
}
void process59() {
	syntaxTreeStack.pop();
}
void process60() {
	for (int i = productSet[59].size() - 1; i >= 1; i--) {
		signStack.push(productSet[59][i]);
	}
}
void process61() {
	for (int i = productSet[60].size() - 1; i >= 1; i--) {
		signStack.push(productSet[60][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = StmtK;
	(*currentP).lineno = token.linsShow;
	(*currentP).kind.stmt = IfK;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).sibling = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
}
void process62() {
	for (int i = productSet[61].size() - 1; i >= 1; i--) {
		signStack.push(productSet[61][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = StmtK;
	(*currentP).lineno = token.linsShow;
	(*currentP).kind.stmt = WhileK;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).sibling = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
}
void process63() {
	for (int i = productSet[62].size() - 1; i >= 1; i--) {
		signStack.push(productSet[62][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = StmtK;
	(*currentP).lineno = token.linsShow;
	(*currentP).kind.stmt = ReadK;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).sibling = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
}
void process64() {
	for (int i = productSet[63].size() - 1; i >= 1; i--) {
		signStack.push(productSet[63][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = StmtK;
	(*currentP).lineno = token.linsShow;
	(*currentP).kind.stmt = WriteK;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).sibling = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
}
void process65() {
	for (int i = productSet[64].size() - 1; i >= 1; i--) {
		signStack.push(productSet[64][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = StmtK;
	(*currentP).lineno = token.linsShow;
	(*currentP).kind.stmt = ReturnK;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
	//currentP = t;
	//(*currentP).sibling = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
}
//��ȷ��
void process66() {
	for (int i = productSet[65].size() - 1; i >= 1; i--) {
		signStack.push(productSet[65][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = StmtK;
	(*currentP).lineno = token.linsShow;
	TreeNode* t = new TreeNode();
	(*t).nodekind = ExpK;
	(*t).kind.exp = IdEK;
	(*t).attr.expAttr.varkind = IdV;
	(*t).lineno = token.linsShow;
	(*t).idnum++;
	(*t).name.push_back(token.sem);
	(*currentP).child[0] = t;
	TreeNode** t1 = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t1 = currentP;
	//currentP = t1;
	//(*currentP).sibling = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
}
void process67() {
	for (int i = productSet[66].size() - 1; i >= 1; i--) {
		signStack.push(productSet[66][i]);
	}
	(*currentP).kind.stmt = AssignK;
}
void process68() {
	for (int i = productSet[67].size() - 1; i >= 1; i--) {
		signStack.push(productSet[67][i]);
	}
	(*((*currentP).child[0])).attr.expAttr.varkind = IdV;
	(*currentP).kind.stmt = CallK;
}
//�����
void process69() {
	for (int i = productSet[68].size() - 1; i >= 1; i--) {
		signStack.push(productSet[68][i]);
	}
	//(*currentP).child[1] = new TreeNode();
	syntaxTreeStack.push(&(*currentP).child[1]);
	currentP = (*currentP).child[0];
	TreeNode* t = new TreeNode();
	(*t).nodekind = ExpK;
	(*t).lineno = token.linsShow;
	(*t).kind.exp = OpK;
	(*t).attr.expAttr.op = END;
	opSignStack.push(t);
}
void process70() {
	for (int i = productSet[69].size() - 1; i >= 1; i--) {
		signStack.push(productSet[69][i]);
	}
	//(*currentP).child[0] = new TreeNode();
	//(*currentP).child[1] = new TreeNode();
	//(*currentP).child[2] = new TreeNode();
	syntaxTreeStack.push(&(*currentP).child[2]);
	syntaxTreeStack.push(&(*currentP).child[1]);
	syntaxTreeStack.push(&(*currentP).child[0]);
}
void process71() {
	for (int i = productSet[70].size() - 1; i >= 1; i--) {
		signStack.push(productSet[70][i]);
	}
	//(*currentP).child[1] = new TreeNode();
	//(*currentP).child[0] = new TreeNode();
	syntaxTreeStack.push(&(*currentP).child[1]);
	syntaxTreeStack.push(&(*currentP).child[0]);
}
void process72() {
	for (int i = productSet[71].size() - 1; i >= 1; i--) {
		signStack.push(productSet[71][i]);
	}
}
void process73() {
	for (int i = productSet[72].size() - 1; i >= 1; i--) {
		signStack.push(productSet[72][i]);
	}
	(*currentP).idnum++;
	(*currentP).name.push_back(token.sem);
}
void process74() {
	for (int i = productSet[73].size() - 1; i >= 1; i--) {
		signStack.push(productSet[73][i]);
	}
	//(*currentP).child[0] = new TreeNode();
	syntaxTreeStack.push(&(*currentP).child[0]);
	TreeNode* t = new TreeNode;
	(*t).nodekind = ExpK;
	(*t).lineno = token.linsShow;
	(*t).kind.exp = OpK;
	(*t).attr.expAttr.op = END;
	opSignStack.push(t);
}
void process75() {
	for (int i = productSet[74].size() - 1; i >= 1; i--) {
		signStack.push(productSet[74][i]);
	}
}
void process76() {
	//(*currentP).child[0] = new TreeNode();
	for (int i = productSet[75].size() - 1; i >= 1; i--) {
		signStack.push(productSet[75][i]);
	}
	syntaxTreeStack.push(&(*currentP).child[1]);
}
void process77() {
	syntaxTreeStack.pop();
}
void process78() {
	for (int i = productSet[77].size() - 1; i >= 1; i--) {
		signStack.push(productSet[77][i]);
	}
	TreeNode* t = new TreeNode;
	(*t).nodekind = ExpK;
	(*t).lineno = token.linsShow;
	(*t).kind.exp = OpK;
	(*t).attr.expAttr.op = END;
	opSignStack.push(t);
}
void process79(){}
void process80() {
	for (int i = productSet[79].size() - 1; i >= 1; i--) {
		signStack.push(productSet[79][i]);
	}
	//(*currentP).sibling = new TreeNode();
	syntaxTreeStack.push(&(*currentP).sibling);
}
void process81() {
	for (int i = productSet[80].size() - 1; i >= 1; i--) {
		signStack.push(productSet[80][i]);
	}
	TreeNode* t = new TreeNode;
	(*t).nodekind = ExpK;
	(*t).lineno = token.linsShow;
	(*t).kind.exp = OpK;
	(*t).attr.expAttr.op = END;
	opSignStack.push(t);
	getExpResult = false;
}
void process82() {
	for (int i = productSet[81].size() - 1; i >= 1; i--) {
		signStack.push(productSet[81][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = ExpK;
	(*currentP).lineno = token.linsShow;
	(*currentP).kind.exp = OpK;
	(*currentP).attr.expAttr.op = token.lex;
	LexType top = (*opSignStack.top()).attr.expAttr.op;
	while (Priosoty(top) >= Priosoty(token.lex)) {
		TreeNode* t = opSignStack.top();
		opSignStack.pop();
		TreeNode* Rnum = opNumStack.top();
		opNumStack.pop();
		TreeNode* Lnum = opNumStack.top();
		opNumStack.pop();
		(*t).child[0] = Lnum;
		(*t).child[1] = Rnum;
		opNumStack.push(t);
		top = (*opSignStack.top()).attr.expAttr.op;
	}
	opSignStack.push(currentP);
	getExpResult = true;
}
void process83() {
	for (int i = productSet[82].size() - 1; i >= 1; i--) {
		signStack.push(productSet[82][i]);
	}
}
void process84() {
	if (token.lex== RPAREN &&expflag > 0) {
		LexType top = (*opSignStack.top()).attr.expAttr.op;
		while (top != LPAREN) {
			TreeNode* t = opSignStack.top();
			opSignStack.pop();
			TreeNode* Rnum = opNumStack.top();
			opNumStack.pop();
			TreeNode* Lnum = opNumStack.top();
			opNumStack.pop();
			(*t).child[0] = Lnum;
			(*t).child[1] = Rnum;
			opNumStack.push(t);
			top = (*opSignStack.top()).attr.expAttr.op;
		}
		opSignStack.pop();
		expflag--;
	}
	else {
		if (getExpResult || getExpResult2) {
			LexType top = (*opSignStack.top()).attr.expAttr.op;
			while (top != END) {
				TreeNode* t = opSignStack.top();
				opSignStack.pop();
				TreeNode* Rnum = opNumStack.top();
				opNumStack.pop();
				TreeNode* Lnum = opNumStack.top();
				opNumStack.pop();
				(*t).child[0] = Lnum;
				(*t).child[1] = Rnum;
				opNumStack.push(t);
				top = (*opSignStack.top()).attr.expAttr.op;
			}
			opSignStack.pop();
			currentP = opNumStack.top();
			opNumStack.pop();
			TreeNode** t = syntaxTreeStack.top();
			syntaxTreeStack.pop();
			*t = currentP;
			getExpResult2 = false;
		}
	}
}
void process85() {
	for (int i = productSet[84].size() - 1; i >= 1; i--) {
		signStack.push(productSet[84][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = ExpK;
	(*currentP).lineno = token.linsShow;
	(*currentP).kind.exp = OpK;
	(*currentP).attr.expAttr.op = token.lex;
	LexType top = (*opSignStack.top()).attr.expAttr.op;
	while (top!=LPAREN&&Priosoty(top) >= Priosoty(token.lex)) {
		TreeNode* t = opSignStack.top();
		opSignStack.pop();
		TreeNode* Rnum = opNumStack.top();
		opNumStack.pop();
		TreeNode* Lnum = opNumStack.top();
		opNumStack.pop();
		(*t).child[0] = Lnum;
		(*t).child[1] = Rnum;
		opNumStack.push(t);
		top = (*opSignStack.top()).attr.expAttr.op;
	}
	opSignStack.push(currentP);
}
void process86() {
	for (int i = productSet[85].size() - 1; i >= 1; i--) {
		signStack.push(productSet[85][i]);
	}
}
void process87(){}
void process88() {
	for (int i = productSet[87].size() - 1; i >= 1; i--) {
		signStack.push(productSet[87][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = ExpK;
	(*currentP).lineno = token.linsShow;
	(*currentP).kind.exp = OpK;
	(*currentP).attr.expAttr.op = token.lex;
	LexType top = (*opSignStack.top()).attr.expAttr.op;
	while (top != LPAREN &&Priosoty(top) >= Priosoty(token.lex) ) {
		TreeNode* t = opSignStack.top();
		opSignStack.pop();
		TreeNode* Rnum = opNumStack.top();
		opNumStack.pop();
		TreeNode* Lnum = opNumStack.top();
		opNumStack.pop();
		(*t).child[0] = Lnum;
		(*t).child[1] = Rnum;
		opNumStack.push(t);
		top = (*opSignStack.top()).attr.expAttr.op;
		opSignStack.push(currentP);
	}
}
void process89() {
	for (int i = productSet[88].size() - 1; i >= 1; i--) {
		signStack.push(productSet[88][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = ExpK;
	(*currentP).lineno = token.linsShow;
	(*currentP).kind.exp = OpK;
	(*currentP).attr.expAttr.op = LPAREN;
	opSignStack.push(currentP);
	expflag++;
}
void process90() {
	for (int i = productSet[89].size() - 1; i >= 1; i--) {
		signStack.push(productSet[89][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = ExpK;
	(*currentP).lineno = token.linsShow;
	(*currentP).kind.exp = ConstK;
	(*currentP).attr.expAttr.val = stoi(token.sem);
	opNumStack.push(currentP);
}
void process91() {
	for (int i = productSet[90].size() - 1; i >= 1; i--) {
		signStack.push(productSet[90][i]);
	}
}
void process92() {
	for (int i = productSet[91].size() - 1; i >= 1; i--) {
		signStack.push(productSet[91][i]);
	}
	currentP = new TreeNode();
	(*currentP).nodekind = ExpK;
	(*currentP).lineno = token.linsShow;
	(*currentP).kind.exp = IdEK;
	(*currentP).idnum++;
	(*currentP).name.push_back(token.sem);
	opNumStack.push(currentP);
}
void process93() {
	(*currentP).attr.expAttr.varkind = IdV;
}
void process94() {
	for (int i = productSet[93].size() - 1; i >= 1; i--) {
		signStack.push(productSet[93][i]);
	}
	(*currentP).attr.expAttr.varkind = ArrayMembV;
	//(*currentP).child[0] = new TreeNode();
	syntaxTreeStack.push(&(*currentP).child[0]);
	TreeNode* t = new TreeNode();
	(*t).nodekind = ExpK;
	(*t).lineno = token.linsShow;
	(*t).kind.exp = OpK;
	(*t).attr.expAttr.op = END;
	opSignStack.push(t);
	getExpResult2 = true;
}
void process95() {
	for (int i = productSet[94].size() - 1; i >= 1; i--) {
		signStack.push(productSet[94][i]);
	}
	(*currentP).attr.expAttr.varkind = FieldMembV;
	//(*currentP).child[0] = new TreeNode();
	syntaxTreeStack.push(&(*currentP).child[0]);
}
void process96() {
	for (int i = productSet[95].size() - 1; i >= 1; i--) {
		signStack.push(productSet[95][i]);
	}
	currentP = new TreeNode();
	(*currentP).idnum++;
	(*currentP).name.push_back(token.sem);
	(*currentP).nodekind = ExpK;
	(*currentP).lineno = token.linsShow;
	(*currentP).kind.exp = IdEK;
	TreeNode** t = syntaxTreeStack.top();
	syntaxTreeStack.pop();
	*t = currentP;
}
void process97() {
	(*currentP).attr.expAttr.varkind = IdV;
}
void process98() {
	for (int i = productSet[97].size() - 1; i >= 1; i--) {
		signStack.push(productSet[97][i]);
	}
	(*currentP).attr.expAttr.varkind = ArrayMembV;
	//(*currentP).child[0] = new TreeNode();
	syntaxTreeStack.push(&(*currentP).child[0]);
	TreeNode* t = new TreeNode();
	(*t).nodekind = ExpK;
	(*t).lineno = token.linsShow;
	(*t).kind.exp = OpK;
	(*t).attr.expAttr.op = END;
	opSignStack.push(t);
	getExpResult2 = true;
}
void process99() {
	for (int i = productSet[98].size() - 1; i >= 1; i--) {
		signStack.push(productSet[98][i]);
	}
}
void process100() {
	for (int i = productSet[99].size() - 1; i >= 1; i--) {
		signStack.push(productSet[99][i]);
	}
}
void process101() {
	for (int i = productSet[100].size() - 1; i >= 1; i--) {
		signStack.push(productSet[100][i]);
	}
}
void process102() {
	for (int i = productSet[101].size() - 1; i >= 1; i--) {
		signStack.push(productSet[101][i]);
	}
}
void process103() {
	for (int i = productSet[102].size() - 1; i >= 1; i--) {
		signStack.push(productSet[102][i]);
	}
}
void process104() {
	for (int i = productSet[103].size() - 1; i >= 1; i--) {
		signStack.push(productSet[103][i]);
	}
}