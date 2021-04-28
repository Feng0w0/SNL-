#ifndef DEFINED
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<set>
#include<cstdlib>
#include<stack>
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
	ID,IF,BEGIN,INTC,END,PLUS,MINUS,TIMES,OVER,EQ,LT,LMIDPAREN,RMIDPAREN,DOT
	,TYPE,VAR, PROCEDURE,PROGRAM,SEMI,INTEGER,CHAR,ARRAY,RECORD,UNDERANGE
	,OF,COMMA,LPAREN,RPAREN,ENDWH,WHILE,RETURN,READ,WRITE,ASSIGN,THEN,FI
	,ELSE,DO
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
	int idnum=0;				//��¼һ���ڵ��б�־������
	vector<string> name;	//������Ա�ǽڵ��еı�־��������
	vector<int*> table;	//	������Ա�ǽڵ��еĸ�����־���ڷ��ű��е����
	string type_name;	//��¼�����������ڵ�Ϊ���������������������ͱ�־����ʾ����Ч
	Attr attr;				//��¼�﷨���ڵ���������
};
struct Token
{
	int linsShow;		//��¼�����к�
	LexType lex;		//��¼�õ��ʵĴʷ���Ϣ������LexTypeΪ���ʵ�����ö��
	string sem;		//��¼�õ��ʵ�������Ϣ
};
#define DEFINED
#endif
/*******************��������************************/

//LL1Process�ļ���ʹ�õı���
/*TreeNode* currentP;	//�洢��ǰ�ڵ�
TreeNode* saveP;		//���浱ǰָ���¼���������ڵ��ָ��
DeckEnum* temp;
bool getExpResult;
bool getExpResult2;	//��������
int expflag;	//���ڼ�������*/
//DesProcess�ļ���ʹ�õı���
//string temp_name;
/*******************��������************************/
int Priosoty(LexType op);	//����Ϊ������������Ϊ���ʵĴʷ�����
void Predict(int pnum);			//���ݲ���ʽ���ѡ��һ��Ҫִ�еĺ���
void CreatLL1Table();	//����LL(1)������
TreeNode* Parse();	//LL(1)�﷨����������
int GetIndex(set<string>& A, string s);	//����VN���ϻ�VT������ָ���ַ������±�
void Input(TreeNode *t,string path);	//��������������ļ���
void InputError(string errorInfo, string path);	//��������Ϣ���뵽�ļ���
//ÿһ������ʽ�Ĵ�����
void process1();
void process2();
void process3();
void process4();
void process5();
void process6();
void process7();
void process8();
void process9();
void process10();
void process11();
void process12();
void process12();
void process13();
void process14();
void process15();
void process16();
void process17();
void process18();
void process19();
void process20();
void process21();
void process22();
void process23();
void process24();
void process25();
void process26();
void process27();
void process28();
void process29();
void process30();
void process31();
void process32();
void process33();
void process34();
void process35();
void process36();
void process37();
void process38();
void process39();
void process40();
void process41();
void process42();
void process43();
void process44();
void process45();
void process46();
void process47();
void process48();
void process49();
void process50();
void process51();
void process52();
void process53();
void process54();
void process55();
void process56();
void process57();
void process58();
void process59();
void process60();
void process61();
void process62();
void process63();
void process64();
void process65();
void process66();
void process67();
void process68();
void process69();
void process70();
void process71();
void process72();
void process73();
void process74();
void process75();
void process76();
void process77();
void process78();
void process79();
void process80();
void process81();
void process82();
void process83();
void process84();
void process85();
void process86();
void process87();
void process88();
void process89();
void process90();
void process91();
void process92();
void process93();
void process94();
void process95();
void process96();
void process97();
void process98();
void process99();
void process100();
void process101();
void process102();
void process103(); 
void process104();
//�ݹ��½���ʹ�õĺ���
TreeNode* DesParse();
TreeNode* Program();
TreeNode* ProgramHead();
TreeNode* DeclarePart();
TreeNode* TypeDec();
TreeNode* TypeDeclaration();
TreeNode* TypeDecList();
TreeNode* TypeDecMore();
void TypeId(TreeNode* t);
void TypeDef(TreeNode* t);
void BaseType(TreeNode* t);
void StructureType(TreeNode* t);
void ArrayType(TreeNode* t);
void RecType(TreeNode* t);
TreeNode* FieldDecList();
TreeNode* FieldDecMore();
void IdList(TreeNode* t);
void IdMore(TreeNode* t);
TreeNode* VarDec();
TreeNode* VarDeclaration();
TreeNode* VarDecList();
TreeNode* VarDecMore();
void VarIdList(TreeNode* t);
void VarIdMore(TreeNode* t);
TreeNode* ProcDec();
TreeNode* ProcDeclaration();
TreeNode* ProcDecMore();
void ParamList(TreeNode* t);
TreeNode* ParamDecList();
TreeNode* ParamMore();
TreeNode* Param();
void FormList(TreeNode* t);
void FidMore(TreeNode* t);
TreeNode* ProcDecPart();
TreeNode* ProcBody();
TreeNode* ProgramBody();
TreeNode* StmList();
TreeNode* StmMore();
TreeNode* Stm();
void AssCall(TreeNode* t);
void AssignmentRest(TreeNode* t);
TreeNode* ConditionalStm();
TreeNode* LoopStm();
TreeNode* InputStm();
TreeNode* OutputStm();
TreeNode* ReturnStm();
void CallStmRest(TreeNode* t);
TreeNode* ActParamList();
TreeNode* ActParamMore();
TreeNode* Exp();
TreeNode* Simple_exp();
TreeNode* Term();
TreeNode* Factor();
TreeNode* Variable();
void VariMore(TreeNode* t);
TreeNode* FieldVar();
void FieldVarMore(TreeNode* t);
void Match(LexType expexted);
void ReadToken();

