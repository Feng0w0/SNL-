// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#include"zyg.h"

string arrayLexType_zyg[] = { "id","if","begin","intc","end","+","-","*","/","=","<","[","]" ,"."
	,"type","var", "procedure","program",";","integer","char","array","record",".."
	,"of",",","(",")","endwh","while","return","read","write",":=","then","fi"
	,"else","do"

};

//id, if, begin, intc, end, plus, minus, times, over, eq, lt, lmidparen, rmidparen, dot
//, type, var, procedure, program, semi, integer, char, array, record, underange
//, of, comma, lparen, rparen, endwh, while, return, read, write, assign, then, fi
//, else, do


void GetNextChar() {
	//string imbuffer;
	string query;
	int linenum = 0;
	state sta = START;
	int longquery;
	int i = 0;
	Token tok;
	ifstream fin("snls.txt");
	ofstream fout("token.txt");
	if (!fin&&!fout) {
		string a = "文件不能打开";
		InputError(a, "error.txt");
		exit(0);
		//cout << "文件不能打开" << endl;
	}
	else
	{
		while ((getline(fin, query))) {
			linenum++;
			i = 0;
			string imbuffer;
			//cout << query<<endl;
			longquery = size(query);
			//cout << longquery << endl;
			if (sta == FINISH) {
				break;
			}
			while (i < longquery) {
				if (sta == FINISH) {
					tok.linsShow = linenum;
					tok.lex = DOT;
					tok.sem = "Finish";
					//cout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
					fout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
					imbuffer.clear();
					break;
				}
				while ((sta == INCOMMENT || sta == START) && i < longquery) {
					if (query[i] == ' ') {
						while (query[i] == ' ') {
							i++;
						}
					}
					//cout << i << endl;
					if (query[i] >= 'a' && query[i] <= 'z') {
						if (sta == START) {
							sta = INID;
						}
						//cout << i << endl;
						while ((query[i] >= 'a' && query[i] <= 'z') || (query[i] >= '0' && query[i] <= '9')) {
							imbuffer = imbuffer + query[i];
							i++;
							if (i == longquery)
								break;
						}
						//cout << imbuffer << endl;
						if (sta == INID) {
							tok.linsShow = linenum;
							tok.lex = ID;
							tok.sem = imbuffer;
							for (int a = 0; a <= 37; a++) {
								if (arrayLexType_zyg[a] == imbuffer) {
									tok.lex = LexType(a);
									tok.sem = "ReseverWord";
									break;
								}
							}
							//cout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							fout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							imbuffer.clear();
							sta = START;
							break;
						}
					}
					if (query[i] >= '0' && query[i] <= '9') {
						if (sta == START) {
							sta = INNUM;
						}
						while (query[i] >= '0' && query[i] <= '9') {
							imbuffer = imbuffer + query[i];
							i++;
							if (i == longquery)
								break;
						}
						while (query[i] >= 'a' && query[i] <= 'z') {
							sta = START;
							i++;
							if (i == longquery)
								break;
						}
						//cout << imbuffer << endl;
						if (sta == INNUM) {
							tok.linsShow = linenum;
							tok.lex = INTC;
							tok.sem = imbuffer;
							//cout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							fout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							imbuffer.clear();
							sta = START;
							break;
						}
						if (sta == START) {
							//fout << linenum << " " << "error" << endl;
							string a = to_string(linenum) + "行有错误";
							InputError(a, "error.txt");
							exit(0);
							break;
						}
					}
					if (query[i] == '+' || query[i] == '-' || query[i] == '*' || query[i] == '/' || query[i] == '(' || query[i] == ')' || query[i] == ';' || query[i] == '[' || query[i] == ']' || query[i] == '=' || query[i] == '<') {
						if (sta == START)
							sta = DONE;
						if (sta == DONE) {
							tok.linsShow = linenum;
							//tok.lex = ID;
							//tok.sem = imbuffer;
							imbuffer = query[i];
							for (int a = 0; a <= 37; a++) {
								if (arrayLexType_zyg[a] == imbuffer) {
									tok.lex = LexType(a);
									tok.sem = "SingleSeparator";
									break;
								}
							}
							i++;
							//cout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							fout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							imbuffer.clear();
							sta = START;
							break;
						}
					}
					if (query[i] == ':') {
						if (i < longquery - 1 && query[i + 1] == '=') {
							if (sta == START)
								sta = INASSIGN;
						}
						/*else {
							cout << linenum << ' ' << "error" << endl;
						}*/
						if (sta == INASSIGN) {
							tok.linsShow = linenum;
							tok.lex = ASSIGN;
							tok.sem = "DobleSeparator";
							i = i + 2;
							//cout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							fout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							imbuffer.clear();
							sta = START;
							break;
						}

					}
					if (query[i] == ',') {
						if (sta == START)
							sta = INCHAR;
						if (sta == INCHAR) {
							tok.linsShow = linenum;
							tok.lex = COMMA;
							tok.sem = "SingleSeparator";
							i = i + 1;
							//cout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							fout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							imbuffer.clear();
							sta = START;
							while (query[i] == ' ') {
								i++;
							}
							if (i < longquery && ((query[i] >= 'a' && query[i] <= 'z') || (query[i] >= '0' && query[i] <= '9')));
							else {
								//cout << linenum << " " << "error" << endl;
								//fout << linenum << " " << "error" << endl
								string a = to_string(linenum) + "行有错误";
								InputError(a, "error.txt");
								exit(0);
							}
								
							break;
						}
					}
					if (query[i] == '.') {
						if (i < longquery - 1 && query[i + 1] == '.') {
							if (sta == START)
								sta = INRANGE;
						}
						else {
							if (sta == START && tok.lex == END)
								sta = FINISH;
							else
								if (sta == START) {
									i++;
									tok.linsShow = linenum;
									tok.lex = DOT;
									tok.sem = "Dot";
									//cout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
									fout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
									imbuffer.clear();
									break;
								}
						}
						if (sta == INRANGE) {
							tok.linsShow = linenum;
							tok.lex = UNDERANGE;
							tok.sem = "ArrayIndex";
							i = i + 2;
							//cout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							fout << tok.linsShow << ' ' << tok.lex << ' ' << tok.sem << endl;
							imbuffer.clear();
							sta = START;
							break;
						}
					}
					if (query[i] == '{')
						if (sta == START)
							sta = INCOMMENT;
					if (sta == INCOMMENT) {
						//cout << "注释开始" << endl;
						while (i < longquery && query[i] != '}')
							i++;
						if (query[i] == '}') {
							i++;
							sta = START;
							//cout << "注释结束" << endl;
							break;
						}
					}
					if (sta != INCOMMENT && sta != FINISH&&i<longquery) {
						//cout << linenum << " " << "error" << endl;
						//fout << linenum << " " << "error" << endl;
						string a = to_string(linenum) + "行有错误";
						InputError(a, "error.txt");
						exit(0);
						i++;
					}

				}
			}
		}
	}
	fin.close();
	fout.close();
}
