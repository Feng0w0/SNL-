#ifndef DEFINED
#include <iostream>
#include<string>
#include<fstream>
using namespace std;

/*******************枚举声明************************/
enum LexType {
	ID, IF, BEGIN, INTC, END, PLUS, MINUS, TIMES, OVER, EQ, LT, LMIDPAREN, RMIDPAREN, DOT
	, TYPE, VAR, PROCEDURE, PROGRAM, SEMI, INTEGER, CHAR, ARRAY, RECORD, UNDERANGE
	, OF, COMMA, LPAREN, RPAREN, ENDWH, WHILE, RETURN, READ, WRITE, ASSIGN, THEN, FI
	, ELSE, DO
};
struct Token
{
	int linsShow;		//记录单词行号
	LexType lex;		//记录该单词的词法信息，其中LexType为单词的类型枚举
	string sem;		//记录该单词的语义信息
};
#define DEFINED
#endif
enum state {
	START, INASSIGN, INCOMMENT, INNUM, INID, INCHAR, INRANGE, DONE, FINISH
};


/*******************函数声明************************/
void GetNextChar();
void InputError(string errorInfo, string path);
