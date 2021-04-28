#ifndef DEFINED
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;
/*******************枚举声明************************/
enum NodeKindEnum {	//语法树节点类型法枚举
	ProK,				//根标志节点
	PheadK,			//程序头标志节点
	TypeK,				//类型声明标志节点
	VarK,				//	变量声明标志节点
	ProcDecK,		//	函数声明标志节点
	StmLK,				//语句序列标志节点
	DecK,				//声明节点
	StmtK,				//语句节点
	ExpK					//表达式节点
};
enum DeckEnum {	//声明节点类型枚举
	ArrayK,				//数组类型
	CharK,				//字符类型
	IntegerK,			//整数类型
	RecordK,			//记录类型
	IdK					//类类型标志符作为类型
};
enum StmtKEnum {	//语句节点类型枚举
	IfK,					//判断语句类型
	WhileK,				//循环语句类型
	AssignK,			//赋值语句类型
	ReadK,				//读语句类型
	WriteK,				//写语句类型
	CallK,				//函数调用语句类型
	ReturnK			//返回语句类型
};
enum ExpKEnum {	//	表达式节点类型枚举
	OpK,					//	操作符类型
	ConstK,			//常整数类型
	IdEK					//标识符类型
};
enum ParamtEnum {	//参数是值参还是变参
	Valparamtype,
	Varparamtype
};
enum VarKindEnum {	//变量的类别,分别表示变量是标志符变量，数组成员变量还是域成员变量
	IdV,
	ArrayMembV,
	FieldMembV
};
enum LexType {
	ID, IF, BEGIN, INTC, END, PLUS, MINUS, TIMES, OVER, EQ, LT, LMIDPAREN, RMIDPAREN, DOT
	, TYPE, VAR, PROCEDURE, PROGRAM, SEMI, INTEGER, CHAR, ARRAY, RECORD, UNDERANGE
	, OF, COMMA, LPAREN, RPAREN, ENDWH, WHILE, RETURN, READ, WRITE, ASSIGN, THEN, FI
	, ELSE, DO
};
enum ExpType {
	Void, Integer, Boolean
};
/*******************共用体声明************************/
union Kind {
	DeckEnum dec;		//记录语法树节点的声明类型,当nodekind=DecK时有效
	StmtKEnum stmt;		//记录语法树节点的语句类型,当nodekind=StmtK时有效
	ExpKEnum exp;		//记录语法树节点的表达式类型,当nodekind=ExpK时有效
};
/*******************结构体声明************************/
struct ArrayAttr {	//记录数组类型的属性。
	int low;
	int up;
	DeckEnum childType;
};
struct ProcAttr {	//记录过程的属性。
	ParamtEnum paramt;
};
struct ExpAttr {	//记录表达式的属性。
	LexType op;	//记录语法树节点的运算符单词，为单词类型
	int val;		//记录语法树节点的数值,
	VarKindEnum varkind;	//记录变量的类别
	ExpType type;	//记录语法树节点的检查类型，取值Void, Integer,Boolean,为类型检查ExpType类型
};
struct Attr {		//记录语法树节点其他属性
	ArrayAttr arrayAttr;
	ProcAttr procAttr;
	ExpAttr expAttr;
};
struct TreeNode
{
	TreeNode* child[3];	//指向子语法树节点指针,0是程序头,1是声明，2是程序体
	TreeNode* sibling;	//指向兄弟语法树节点指针
	int lineno;					//记录源程序行号
	NodeKindEnum nodekind;//	记录语法树节点类型
	Kind kind;					//记录语法树节点的具体类型
	int idnum = 0;				//记录一个节点中标志符个数
	vector<string> name;	//容器成员是节点中的标志符的名字
	vector<int*> table;	//	容器成员是节点中的各个标志符在符号表中的入口
	string type_name;	//记录类型名，当节点为声明类型且类型是由类型标志符表示是有效
	Attr attr;				//记录语法树节点其他属性
};
#define DEFINED
#endif

//符号表管理

enum IdKind { typeKind, varKind, procKind };

enum AccessKind { dir, indir };
//形参信息表
struct symbtable;
struct ParamTable {
	symbtable* entry;                 //形参标识符在符号表中的位置
	ParamTable* next=nullptr;
};
//标识符信息项
typedef struct
{
	struct typeIR* idtype; /*指向标识符的类型内部表示*/
	IdKind kind; /*标识符的类型*/  //typeKind, varKind, procKind
	union
	{
		struct
		{
			//Access表示是直接变量还是间接变量：access = （dir,indir）indir表示是间接变量（变参属于间接变量），dir表示直接变量
			AccessKind access;
			int level;
			int off;
		}VarAttr; /*变量标识符的属性*/
		struct
		{
			int level;
			ParamTable* param; /*参数表*/ //形参信息表地址
			int code;
			int size;
		}ProcAttr; /*过程名标识符的属性*/
	}More; /*标识符的不同类型有不同的属性*/
}AttributeIR;

enum TypeKind { intTy, charTy, arrayTy, recordTy, boolTy };//intTy, charTy, arrayTy, recordTy, boolTy


//记录的域表
struct fieldChain
{
	string idname; //记录域中的标识符；
	typeIR *unitType; //指向域类型的内部表示；                                                               //这里有错误
	int offset; //表示当前标识符在记录中的偏移；
	fieldChain* next; //指向下一个域。
};
//标识符内部表示
struct typeIR
{
	int size; /*类型所占空间大小*/
	TypeKind kind;  //intTy, charTy, arrayTy, recordTy, boolTy
	union
	{
		struct
		{
			struct typeIR* indexTy;
			struct typeIR* elemTy;
		}ArrayAttr;
		fieldChain* body; /*记录类型中的域链*/
	} More;
};

//符号表定义
struct symbtable
{
	string idname;
	AttributeIR attrIR;
	struct symbtable* next=nullptr;
};
/*******************函数声明************************/
typeIR* TypeProcess(TreeNode* t);
void statement(TreeNode* t);
void bianli(TreeNode* t);
void Body(TreeNode* t);
void Analyze(TreeNode* currentP);