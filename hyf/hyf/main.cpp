#include"hyf.h"
#include"zyg.h"
#include"zjh.h"
int main() {
	//词法分析
	GetNextChar();		
	//语法分析
	TreeNode* root= DesParse();
	//语义分析                                                                      
	Analyze(root);
	system("pause");
	return 0;
}
