#ifndef DEFINED
#include <iostream>
#include<string>
#include<fstream>
using namespace std;

/*******************ö������************************/
enum LexType {
	ID, IF, BEGIN, INTC, END, PLUS, MINUS, TIMES, OVER, EQ, LT, LMIDPAREN, RMIDPAREN, DOT
	, TYPE, VAR, PROCEDURE, PROGRAM, SEMI, INTEGER, CHAR, ARRAY, RECORD, UNDERANGE
	, OF, COMMA, LPAREN, RPAREN, ENDWH, WHILE, RETURN, READ, WRITE, ASSIGN, THEN, FI
	, ELSE, DO
};
struct Token
{
	int linsShow;		//��¼�����к�
	LexType lex;		//��¼�õ��ʵĴʷ���Ϣ������LexTypeΪ���ʵ�����ö��
	string sem;		//��¼�õ��ʵ�������Ϣ
};
#define DEFINED
#endif
enum state {
	START, INASSIGN, INCOMMENT, INNUM, INID, INCHAR, INRANGE, DONE, FINISH
};


/*******************��������************************/
void GetNextChar();
void InputError(string errorInfo, string path);
