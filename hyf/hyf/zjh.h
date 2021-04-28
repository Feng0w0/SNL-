#ifndef DEFINED
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;
/*******************ö������************************/
enum NodeKindEnum {	//�﷨���ڵ����ͷ�ö��
	ProK,				//����־�ڵ�
	PheadK,			//����ͷ��־�ڵ�
	TypeK,				//����������־�ڵ�
	VarK,				//	����������־�ڵ�
	ProcDecK,		//	����������־�ڵ�
	StmLK,				//������б�־�ڵ�
	DecK,				//�����ڵ�
	StmtK,				//���ڵ�
	ExpK					//���ʽ�ڵ�
};
enum DeckEnum {	//�����ڵ�����ö��
	ArrayK,				//��������
	CharK,				//�ַ�����
	IntegerK,			//��������
	RecordK,			//��¼����
	IdK					//�����ͱ�־����Ϊ����
};
enum StmtKEnum {	//���ڵ�����ö��
	IfK,					//�ж��������
	WhileK,				//ѭ���������
	AssignK,			//��ֵ�������
	ReadK,				//���������
	WriteK,				//д�������
	CallK,				//���������������
	ReturnK			//�����������
};
enum ExpKEnum {	//	���ʽ�ڵ�����ö��
	OpK,					//	����������
	ConstK,			//����������
	IdEK					//��ʶ������
};
enum ParamtEnum {	//������ֵ�λ��Ǳ��
	Valparamtype,
	Varparamtype
};
enum VarKindEnum {	//���������,�ֱ��ʾ�����Ǳ�־�������������Ա�����������Ա����
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
/*******************����������************************/
union Kind {
	DeckEnum dec;		//��¼�﷨���ڵ����������,��nodekind=DecKʱ��Ч
	StmtKEnum stmt;		//��¼�﷨���ڵ���������,��nodekind=StmtKʱ��Ч
	ExpKEnum exp;		//��¼�﷨���ڵ�ı��ʽ����,��nodekind=ExpKʱ��Ч
};
/*******************�ṹ������************************/
struct ArrayAttr {	//��¼�������͵����ԡ�
	int low;
	int up;
	DeckEnum childType;
};
struct ProcAttr {	//��¼���̵����ԡ�
	ParamtEnum paramt;
};
struct ExpAttr {	//��¼���ʽ�����ԡ�
	LexType op;	//��¼�﷨���ڵ����������ʣ�Ϊ��������
	int val;		//��¼�﷨���ڵ����ֵ,
	VarKindEnum varkind;	//��¼���������
	ExpType type;	//��¼�﷨���ڵ�ļ�����ͣ�ȡֵVoid, Integer,Boolean,Ϊ���ͼ��ExpType����
};
struct Attr {		//��¼�﷨���ڵ���������
	ArrayAttr arrayAttr;
	ProcAttr procAttr;
	ExpAttr expAttr;
};
struct TreeNode
{
	TreeNode* child[3];	//ָ�����﷨���ڵ�ָ��,0�ǳ���ͷ,1��������2�ǳ�����
	TreeNode* sibling;	//ָ���ֵ��﷨���ڵ�ָ��
	int lineno;					//��¼Դ�����к�
	NodeKindEnum nodekind;//	��¼�﷨���ڵ�����
	Kind kind;					//��¼�﷨���ڵ�ľ�������
	int idnum = 0;				//��¼һ���ڵ��б�־������
	vector<string> name;	//������Ա�ǽڵ��еı�־��������
	vector<int*> table;	//	������Ա�ǽڵ��еĸ�����־���ڷ��ű��е����
	string type_name;	//��¼�����������ڵ�Ϊ���������������������ͱ�־����ʾ����Ч
	Attr attr;				//��¼�﷨���ڵ���������
};
#define DEFINED
#endif

//���ű����

enum IdKind { typeKind, varKind, procKind };

enum AccessKind { dir, indir };
//�β���Ϣ��
struct symbtable;
struct ParamTable {
	symbtable* entry;                 //�βα�ʶ���ڷ��ű��е�λ��
	ParamTable* next=nullptr;
};
//��ʶ����Ϣ��
typedef struct
{
	struct typeIR* idtype; /*ָ���ʶ���������ڲ���ʾ*/
	IdKind kind; /*��ʶ��������*/  //typeKind, varKind, procKind
	union
	{
		struct
		{
			//Access��ʾ��ֱ�ӱ������Ǽ�ӱ�����access = ��dir,indir��indir��ʾ�Ǽ�ӱ�����������ڼ�ӱ�������dir��ʾֱ�ӱ���
			AccessKind access;
			int level;
			int off;
		}VarAttr; /*������ʶ��������*/
		struct
		{
			int level;
			ParamTable* param; /*������*/ //�β���Ϣ���ַ
			int code;
			int size;
		}ProcAttr; /*��������ʶ��������*/
	}More; /*��ʶ���Ĳ�ͬ�����в�ͬ������*/
}AttributeIR;

enum TypeKind { intTy, charTy, arrayTy, recordTy, boolTy };//intTy, charTy, arrayTy, recordTy, boolTy


//��¼�����
struct fieldChain
{
	string idname; //��¼���еı�ʶ����
	typeIR *unitType; //ָ�������͵��ڲ���ʾ��                                                               //�����д���
	int offset; //��ʾ��ǰ��ʶ���ڼ�¼�е�ƫ�ƣ�
	fieldChain* next; //ָ����һ����
};
//��ʶ���ڲ���ʾ
struct typeIR
{
	int size; /*������ռ�ռ��С*/
	TypeKind kind;  //intTy, charTy, arrayTy, recordTy, boolTy
	union
	{
		struct
		{
			struct typeIR* indexTy;
			struct typeIR* elemTy;
		}ArrayAttr;
		fieldChain* body; /*��¼�����е�����*/
	} More;
};

//���ű���
struct symbtable
{
	string idname;
	AttributeIR attrIR;
	struct symbtable* next=nullptr;
};
/*******************��������************************/
typeIR* TypeProcess(TreeNode* t);
void statement(TreeNode* t);
void bianli(TreeNode* t);
void Body(TreeNode* t);
void Analyze(TreeNode* currentP);