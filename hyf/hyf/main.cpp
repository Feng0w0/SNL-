#include"hyf.h"
#include"zyg.h"
#include"zjh.h"
int main() {
	//�ʷ�����
	GetNextChar();		
	//�﷨����
	TreeNode* root= DesParse();
	//�������                                                                      
	Analyze(root);
	system("pause");
	return 0;
}
