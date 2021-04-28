#include"zjh.h"
//符号表的实现

symbtable Scope[10000];//符号表
symbtable DestroyScope[10000];//撤销符号表
int destroylevel = 0; 
int level = 0;
bool flagtree=false;//用于判断自定义类型是类型声明还是变量声明，变量声明为false
symbtable* table = nullptr;
//创建一个符号表
int off=0,off2=0;
void CreatTable() {
	off2 = off;
	off = 0;
	level++;
}

//撤销一个符号表
void DestroyTable() {
	DestroyScope[destroylevel].next = Scope[level].next;
	destroylevel++;
	Scope[level].next = nullptr;
	off = off2;
	level = level - 1;
}

//从表头开始，依次将节点中的标识符名字和id比较是否相同，直
//到找到此标识符或到达表尾，若找到，返回真值，Entry为标识符
//在符号表中的位置，否则，返回值为假。
bool SearchoneTable(string id, int currentLevel, symbtable** Entry) {
		symbtable* t = Scope[currentLevel].next;
		while (t != nullptr) {                    //判断是否相同
			if (t->idname == id) {
				(*Entry) = t;
				return true;
			}
			else t = t->next;
		}
	return false;
}
//符号表中查找标识符：
bool FindEntry(string id,bool flag, symbtable** Entry) {
	if (flag) {
		for (int i = level; i > 0; i--)
			if (SearchoneTable(id, i, Entry))
				return true;
	}
	else return SearchoneTable(id, level, Entry);
	return false;
}
//登记标识符和属性到符号表
bool Enter(string Id, AttributeIR* AttribP, symbtable** Entry) {
	bool present = FindEntry(Id,false, Entry);
	if (present == true) {
		cout <<"标识符重复声明"<<endl;
	}
	else {                                                //插入符号表
		
		symbtable* newsymbtable = new symbtable;
		newsymbtable->idname = Id;
		newsymbtable->attrIR = *AttribP;          //标识符信息项
		/*cout << "入表" + Id ;
		cout <<"层数"<< AttribP->More.VarAttr.level<<" " << AttribP->More.ProcAttr.level<<endl;*/
		if (Scope[level].next == nullptr)
			Scope[level].next = newsymbtable;
		else {
			symbtable* t = Scope[level].next;
			while (t->next != nullptr)
			{
				t = t->next;
			}
			t->next = newsymbtable;
		}
		(*Entry) = newsymbtable;
	}
	return present;
}
//在域表中查找域名
bool FindField(string Id, fieldChain* head, fieldChain** Entry) {
	fieldChain* r = head;
	while (r != nullptr) {
		if (Id == r->idname)
		{
			(*Entry) = r;
			return true;
		}
		r = r->next;
	}
	return false;
}
//打印符号表
//void PrintSymbTable() {
//	/*for (int i = 1; i < level+1; i++) {
//		symbtable* t = Scope[i].next;
//		while (t != nullptr) {
//			if (t->attrIR.kind == typeKind)
//				cout << t->idname << " " << t->attrIR.kind << " "; 
//			cout << t->attrIR;
//			cout << " " << endl;
//			else if (t->attrIR.kind == varKind)
//			{
//				cout << t->idname <<" "<< t->attrIR.kind <<" "<< t->attrIR <<" "<< t->attrIR.More.VarAttr.level <<" "<<t->attrIR.idtype->kind<<" "<< t->attrIR.idtype->size << endl;
//			}
//			else 
//			t = t->next;
//		}
//	}*/
//}
//输出符号表
void Inputsymb(string path) {
	ofstream ouput;
	ouput.open(path);
	for (int i = 1; i < level+1; i++) {
		symbtable* t = Scope[i].next;
		while (t != nullptr) {
			//有选择地输出一些符号表信息项
			if (t->attrIR.kind == typeKind)
				ouput << t->idname << " " << t->attrIR.kind << " "<<&t->attrIR<<" " << t->attrIR.idtype << " " << t->attrIR.More.VarAttr.level<< endl;
			else if (t->attrIR.kind == varKind)
			{
				if (t->attrIR.idtype != nullptr)
					ouput << t->idname << " " << t->attrIR.kind << " " << &t->attrIR << " " << t->attrIR.idtype << " " << t->attrIR.More.VarAttr.level << " " << t->attrIR.idtype->kind << " " << t->attrIR.idtype->size << endl;
				else ouput << t->idname << " " << t->attrIR.kind << " " << &t->attrIR << " " << endl;
			}
			else ouput << t->idname <<" "<< t->attrIR.kind <<" "<<&t->attrIR<<" "<< t->attrIR.idtype << " " << t->attrIR.More.ProcAttr.level << " "<<t->attrIR.More.ProcAttr.param<< endl;

			t = t->next;
		}
	}
	ouput.close();
}
//输出已撤销地符号表
void Inputdestroysymb(string path) {
	ofstream ouput;
	ouput.open(path,ios::app);
	ouput << endl << endl << endl << endl << endl;
	ouput <<"******************************已删除符号表*************************************"<<endl;
	for (int i = 0; i < destroylevel ; i++) {
		symbtable* t = DestroyScope[i].next;
		while (t != nullptr) {
			if (t->attrIR.kind == typeKind)
				ouput << t->idname << " " << t->attrIR.kind << " " << &t->attrIR << " " << t->attrIR.idtype << " " << t->attrIR.More.VarAttr.level << endl;
			else if (t->attrIR.kind == varKind)
			{
				ouput << t->idname << " " << t->attrIR.kind << " " << &t->attrIR << " " << t->attrIR.idtype << " " << t->attrIR.More.VarAttr.level << " " << t->attrIR.idtype->kind << " " << t->attrIR.idtype->size << endl;
			}
			else ouput << t->idname << " " << t->attrIR.kind << " " << &t->attrIR << " " << t->attrIR.idtype << " " << t->attrIR.More.ProcAttr.level << " " << t->attrIR.More.ProcAttr.param << endl;

			t = t->next;
		}
	}
	ouput.close();
}
typeIR* intPtr, * charPtr, * boolPtr;
//初始化基本类型内部表示函数：初始化整数类型，字符类型，布尔类型的内部表示说明由于这三种类型均为基本类型，内部表示固定。
void initialize() {
	intPtr = new typeIR;
	intPtr->size = 1;
	intPtr->kind = intTy;
	charPtr = new typeIR;
	charPtr->size = 1;
	charPtr->kind = charTy;
	boolPtr = new typeIR;
	boolPtr->size = 1;
	boolPtr->kind = boolTy;
}
//自定义类型内部结构分析函数
typeIR* nameType(TreeNode* t) {
	symbtable** Entry = new symbtable*;
	int i = 0;
	if (flagtree)i++;
	bool flag = FindEntry(t->name[i],false, Entry);
	if (!flag) cout << t->lineno<<"自定义类型" + t->name[i] + "未声明"<<endl;
	else return (*Entry)->attrIR.idtype;

	return nullptr;
}
//数组类型内部表示处理函数
typeIR* arrayType(TreeNode* t) {
	if (t->attr.arrayAttr.low > t->attr.arrayAttr.up) {
		cout << "数组下表越界"<<endl;
	}
	else {
		typeIR* newarrayType = new typeIR;
		newarrayType->kind = arrayTy;
		newarrayType->size = (t->attr.arrayAttr.up) - (t->attr.arrayAttr.low);
		newarrayType->More.ArrayAttr.indexTy = intPtr;
		if (t->attr.arrayAttr.childType == CharK)
			newarrayType->More.ArrayAttr.elemTy = charPtr;
		else newarrayType->More.ArrayAttr.elemTy = intPtr;
		return newarrayType;
	}
	return nullptr;
}
void outputbody(fieldChain* bodyt) {
	if (bodyt == nullptr)
		return;
	else {
		cout <<"  "<< bodyt->idname;
		outputbody(bodyt->next);
		return;
	}
}
//处理记录类型的内部表示函数
typeIR* recordType(TreeNode* t) {
	typeIR* newrecordType = new typeIR;
	newrecordType->kind = recordTy;
	fieldChain* newbody = nullptr,*dt = nullptr;
	TreeNode* nt = t->child[0];
	int recordoffset = 0;
	while (nt != nullptr) {
		for (int i = 0; i < nt->name.size(); i++) {
		if (newbody != nullptr) {
				fieldChain** Entry=new fieldChain*;
				bool flag = FindField(nt->name[i], newbody, Entry);
				if (flag)
					cout << t->lineno << "record重命名"<<endl;
			}
			fieldChain* newt = new fieldChain;;
			if (newbody == nullptr)
				newbody = newt;
			else {
				dt->next=newt;
			}
			newt->unitType = TypeProcess(nt);
			newt->idname = nt->name[i];
			newt->offset = recordoffset + newt->unitType->size;
			newt->next=nullptr;
			dt = newt;
		}
		nt = nt->sibling;
	}
	dt->next = nullptr;
	newrecordType->More.body = newbody;
	//cout << newbody->next;
	//outputbody(newbody);cout<<endl;
	return newrecordType;
}

//类型分析处理函数：类型分析处理。处理语法树的当前节点类型，构造当前类型的内部
//表示，并将其地址返回给Ptr类型内部表示的地址。
typeIR* TypeProcess(TreeNode* t) {
	if (t->kind.dec == ArrayK)
		return arrayType(t);
	else if (t->kind.dec == CharK)
		return charPtr;
	else if (t->kind.dec == IntegerK)
		return intPtr;
	else if (t->kind.dec == RecordK)
		return recordType(t);
	else if (t->kind.dec == IdK)
		return nameType(t);
	return nullptr;
}

//类型声明部分分析处理函数
void TypeDecPart(TreeNode* t) {
	if (t == nullptr)
		return;
	
	//下面写处理过程
	symbtable** Entry = new symbtable*;
	int i = 0;
	if (t->kind.dec == IdK) i++;
	for (; i < t->name.size(); i++) {
		bool flag = FindEntry(t->name[0],false, Entry);
		if (flag)
			cout << t->lineno<<"类型标识符" + t->name[0] + "重复声明"<<endl;
		else {
			AttributeIR* a = new AttributeIR;
			a->kind = typeKind;
			flagtree = true;
			a->idtype = TypeProcess(t);
			flagtree = false;
			Enter(t->name[0], a, Entry);
		}
	}TypeDecPart(t->sibling);
	return;
}
//变量声明部分分析处理函数
void VarDecList(TreeNode* t) {
	if (t == nullptr)
		return;
	//下面写处理过程
	symbtable** Entry = new symbtable*;
	int i = 0;
	if (t->kind.dec == IdK) i++;
	for (; i < t->name.size(); i++) {
		
		bool flag = FindEntry(t->name[i],false, Entry);
		if (flag)
			cout << t->lineno<<"变量标识符" + t->name[i] + "重复声明"<<endl;
		else {
			AttributeIR* a = new AttributeIR;
			a->kind = varKind;
			a->idtype = TypeProcess(t);
			a->More.VarAttr.access = dir;
			a->More.VarAttr.level = level;
			if(a->idtype!=nullptr)
			a->More.VarAttr.off = off + a->idtype->size;
			Enter(t->name[i], a, Entry);
		}
	}
	VarDecList(t->sibling);
	return;
}
//形参分析处理函数
ParamTable* ParaDecList(TreeNode* t) {
	if (t == nullptr)
		return nullptr;
	ParamTable* parat = nullptr;
	ParamTable* paralist = nullptr,*dt=nullptr;
	for (int i = 0; i < t->name.size(); i++) {
		symbtable** Entry = new symbtable*;
		bool flag = FindEntry(t->name[i],false, Entry);
		if (flag)
			cout << t->lineno<<"形参标识符" + t->name[i] + "重复声明"<<endl;
		else {
			AttributeIR* a = new AttributeIR;
			a->kind = varKind;
			a->idtype = TypeProcess(t);
			a->More.VarAttr.access = dir;
			a->More.VarAttr.level = level;
			a->More.VarAttr.off = off + a->idtype->size;
			Enter(t->name[i], a, Entry);
			parat = new ParamTable;
			if (paralist == nullptr)
				paralist = parat;
			else dt->next = parat;
			parat->entry = (*Entry);
			//parat->next =new ParamTable;
			parat->next = nullptr;
			dt = parat;
		}
	}
	dt->next=ParaDecList(t->sibling);
	return paralist;
}
//过程声明部分分析处理函数
void ProcDecPart(TreeNode* t) {
	if (t == nullptr)
		return;
	//将函数添加进符号表
	AttributeIR* procir = new AttributeIR;
	procir->kind = procKind;
	procir->More.ProcAttr.level = level;
	ParamTable* paramt=new ParamTable;
	procir->More.ProcAttr.param = paramt;
	symbtable** Entry = new symbtable*;
	Enter(t->name[0], procir, Entry);
	//下面写处理过程
	CreatTable();
	//处理形参，并将形参表给函数的符号表内的param项
	(*Entry)->attrIR.More.ProcAttr.param=ParaDecList(t->child[0]);   
	
	bianli(t->child[1]);                       //声明                
	Body(t->child[2]->child[0]);                                  //语句
	DestroyTable();
	return;
}
//过程声明头分析函数
//symbtable* HeadProcess(TreeNode* t) {
//
//	return nullptr;
//}
//表达式分析处理函数
typeIR* Expr(TreeNode* t, AccessKind* Ekind) {
	if (t->kind.exp == IdEK) {
		symbtable** Entry = new symbtable*;
		bool flag = FindEntry(t->name[0], true, Entry);
		if (!flag)
			cout << t->lineno << "表达式语句中" + t->name[0] + "未声明"<<endl;
		else {
			if (t->child[0] != nullptr) {
				if ((*Entry)->attrIR.idtype->kind == arrayTy)                //数组类型返回元素的内部表示的kind
				{
					Expr(t->child[0], Ekind);
					if (Expr(t->child[0], Ekind) != nullptr) {
						if (Expr(t->child[0], Ekind)->kind == arrayTy)
							return Expr(t->child[0], Ekind);
					}
					return (*Entry)->attrIR.idtype->More.ArrayAttr.elemTy;
				}
				else if ((*Entry)->attrIR.idtype->kind == recordTy) {
					fieldChain*aa= (*Entry)->attrIR.idtype->More.body;
					while (aa != nullptr)
					{
						if (aa->idname == t->child[0]->name[0])
							return aa->unitType;
					}
					cout << t->lineno << "record中没有此元素"<<endl;
				}
			}
			else return (*Entry)->attrIR.idtype;
		}
	}
	else if (t->kind.exp == ConstK) {
		(*Ekind) = dir;
		return intPtr;
	}
	else {
		AccessKind* Ekindt=new AccessKind;
		typeIR* a = Expr(t->child[0], Ekindt);
		typeIR* b = Expr(t->child[1], Ekindt);
		if (a == b) {
			(*Ekind) = dir;
			return a;
		}

		else cout << t->lineno << "表达式语句类型不匹配"<<endl;
	}
	return nullptr;
}
//TypeKind assignstatementpart(TreeNode* t) {
//	if (t->kind.exp == IdEK) {
//		symbtable** Entry = new symbtable*;
//		bool flag = FindEntry(t->name[0], true,Entry);
//		if (!flag)
//			cout << "赋值语句中" + t->name[0] + "未声明";
//		else {
//			if ((*Entry)->attrIR.idtype->kind == arrayTy)                //数组类型返回元素的内部表示的kind
//			{
//				return (*Entry)->attrIR.idtype->More.ArrayAttr.elemTy->kind;
//			}
//			else return (*Entry)->attrIR.idtype->kind;
//		}
//	}
//	else if (t->kind.exp == ConstK) {
//		return intTy;
//	}
//	else {
//		TypeKind a=assignstatementpart(t->child[0]);
//		TypeKind b = assignstatementpart(t->child[1]);
//		if (a == b)
//			return a;
//		else cout << "赋值语句等式右端类型不匹配";
//	}
//	return intTy;
//}
//赋值语句分析函数
void assignstatement(TreeNode* t) {
	if (t->child[0]->kind.exp != IdEK)
		cout << t->lineno << "赋值语句左端不是标识符"<<endl;
	else {
		symbtable** Entry =new symbtable*;
		bool flag = FindEntry(t->child[0]->name[0],true, Entry);
		if (!flag)
			cout << t->lineno<< "标识符" + t->child[0]->name[0] + "未声明"<<endl;
		else {
			TreeNode* nowt = t->child[0];
			symbtable* findt = *Entry;
			typeIR* kindleft = findt->attrIR.idtype;
			if (nowt->child[0] != nullptr) {
				if (findt->attrIR.idtype->kind == arrayTy) {
					AccessKind* Ekind=new AccessKind;
					kindleft = Expr(nowt->child[0],Ekind);
				}
				else if ((*Entry)->attrIR.idtype->kind == recordTy) {
					fieldChain* aa = (*Entry)->attrIR.idtype->More.body;
					bool flagg = true;
					while (aa != nullptr)
					{
						if (aa->idname == nowt->child[0]->name[0]) {
							kindleft = aa->unitType;
							flagg = false;
							break;
						}	
						aa = aa->next;
					}
					if(flagg) cout << t->lineno << "record中没有此元素"<<endl;
				}
			}
			AccessKind* Ekind=new AccessKind;
			typeIR *kindright = Expr(t->child[1],Ekind);
			if (kindleft != kindright)
				cout << t->lineno<<"赋值语句左右类型不匹配"<<endl;
		}
	}
}
//过程调用语句分析处理函数
void callstatement(TreeNode* t) {
	symbtable** Entry = new symbtable*;
	bool flag = FindEntry(t->child[0]->name[0],true, Entry);
	if (!flag)
		cout << t->lineno<< "函数" + t->child[0]->name[0] + "未声明"<<endl;
	else {
		if ((*Entry)->attrIR.kind != procKind)
			cout << t->lineno<< "标识符不是函数类型，无法调用"<<endl;
		else {
			if (t->child[1] == nullptr) {
				if ((*Entry)->attrIR.More.ProcAttr.param == nullptr)
					return;
				else cout << t->lineno << "参数不匹配"<<endl;
			}
			ParamTable* a = (*Entry)->attrIR.More.ProcAttr.param;
			TreeNode* b = t->child[1];
			while(a != nullptr&&b!=nullptr) {
				AccessKind* Ekind = new AccessKind;
				typeIR*bt=Expr(b,Ekind);
				if (a->entry->attrIR.idtype != bt) {
					if (bt->kind == arrayTy && a->entry->attrIR.idtype->kind == arrayTy) {
						if (bt->size == a->entry->attrIR.idtype->size && bt->More.ArrayAttr.elemTy == a->entry->attrIR.idtype->More.ArrayAttr.elemTy)
							;
						else cout << t->lineno << "参数不匹配"<<endl;
					}
					else cout << t->lineno << "参数不匹配"<<endl;
				}
					
				b = b->sibling;
				a = a->next;
			}
			if (a != nullptr || b != nullptr) {
				cout << t->lineno << "参数不匹配"<<endl;
			}
				
			return;
		}
	}
}
//void ifstatmentpart(TreeNode* t) {                                                                  //if语句分程序，还没有判断语句两边是否同一类型
//
//	if (t->kind.exp == IdEK) {
//		symbtable** Entry = new symbtable*;
//		bool flag = FindEntry(t->name[0],true, Entry);
//		if (!flag) {
//			cout << "if语句中标识符" + t->name[0] + "未声明";
//		}
//		return;
//	}
//	if (t->kind.exp == OpK) {
//		
//		ifstatmentpart(t->child[0]);
//		ifstatmentpart(t->child[1]);
//		return;
//	}
//	return;
//
//}
//条件语句分析处理函数                                                                         
void ifstatment(TreeNode* t) {
	//if语法树child[0]为if表达式,且为操作符
	TreeNode* ift = t->child[0];
	AccessKind* Ekind = new AccessKind;
	Expr(ift, Ekind);
	TreeNode* elset = t->child[1];
	while (elset != nullptr) {
		statement(elset);
		elset = elset->sibling;
	}TreeNode* thent = t->child[2];
	while (thent != nullptr) {
		statement(thent);
		thent = thent->sibling;
	}
	return;

}
//void whilestatmentpart(TreeNode* t) {
//	if (t->kind.exp == IdEK) {
//		symbtable** Entry = new symbtable*;
//		bool flag = FindEntry(t->name[0],true, Entry);
//		if (!flag) {
//			cout << "while语句中标识符" + t->name[0] + "未声明";
//		}
//		return;
//	}
//	if (t->kind.exp == OpK) {
//		whilestatmentpart(t->child[0]);
//		whilestatmentpart(t->child[1]);
//		return;
//	}
//	return;
//
//}
//循环语句分析处理函数
void whilestatement(TreeNode* t) {
	//while语法树child[0]为if表达式,同if
	TreeNode* whilet = t->child[0];
	AccessKind *Ekind=new AccessKind;
	Expr(whilet, Ekind);
	TreeNode* dot = t->child[1];
	while (dot != nullptr) {
		statement(dot);
		dot = dot->sibling;
	}
}
//读语句分析处理函数
void readstatement(TreeNode* t) {
	symbtable** Entry = new symbtable*;
	//for (int i = 0; i < t->name.size(); i++) {}
	bool flag = FindEntry(t->name[0],true, Entry);
	if (!flag)
		cout <<t->lineno<< "读语句标识符" + t->name[0] + "未声明"<<endl;
	return;
}
//TypeKind writestatementpart(TreeNode* t) {
//	if (t->kind.exp == IdEK) {
//		symbtable** Entry = new symbtable*;
//		bool flag = FindEntry(t->name[0],true, Entry);
//		if (!flag)
//			cout <<t->lineno<< "写语句中" + t->name[0] + "未声明";
//		else {
//			if ((*Entry)->attrIR.idtype->kind == arrayTy)                //数组类型返回元素的内部表示的kind
//			{
//				return (*Entry)->attrIR.idtype->More.ArrayAttr.elemTy->kind;
//			}
//			else return (*Entry)->attrIR.idtype->kind;
//		}
//	}
//	else if (t->kind.exp == ConstK) {
//		return intTy;
//	}
//	else {
//		if (assignstatementpart(t->child[0]) == assignstatementpart(t->child[1]))
//			return assignstatementpart(t->child[0]);
//		else cout <<t->lineno<<"写语句表达式类型不匹配";
//	}
//	return intTy;
//}
//写语句分析处理函数
void writestatement(TreeNode* t) {
	AccessKind* Ekind = new AccessKind;
	Expr(t->child[0],Ekind);
	return;
}
//返回语句分析处理程序
void returnstatment(TreeNode* t) {
	return;
}
//语句序列分析处理函数
void statement(TreeNode* t) {
	if (t->kind.stmt == IfK) {
		ifstatment(t);
	}
	else if (t->kind.stmt == WhileK) {
		whilestatement(t);
	}
	else if (t->kind.stmt == AssignK) {
		assignstatement(t);
	}
	else if (t->kind.stmt == ReadK) {
		readstatement(t);
	}
	else if (t->kind.stmt == WriteK) {
		writestatement(t);
	}
	else if (t->kind.stmt == CallK) {
		callstatement(t);
	}
	else if (t->kind.stmt == ReturnK) {
		returnstatment(t);
	}
	return;
}
//执行体部分分析处理函数：
void Body(TreeNode* t) {
	if (t == nullptr) return;
	while (t != nullptr) {
		statement(t);
		t = t->sibling;
	}
}

//数组变量的处理分析函数
typeIR* arrayVar(TreeNode* t) {

	return nullptr;
}
//记录变量中域变量的分析处理函数
typeIR* recordVar(TreeNode* t) {
	return nullptr;
}

//遍历语法树，调用相应的函数处理语法树结点
void bianli(TreeNode* t) {
	if (t == nullptr)
		return;
	if (t->nodekind == ProK) {
		bianli(t->child[0]);
		bianli(t->child[1]);
		bianli(t->child[2]);
		return;
	}if (t->nodekind == PheadK)
		return;
	if (t->nodekind == TypeK) {
		TypeDecPart(t->child[0]);
		bianli(t->sibling);
		return;
	}if (t->nodekind == VarK) {
		VarDecList(t->child[0]);
		bianli(t->sibling);
		return;
	}if (t->nodekind == ProcDecK) {
		ProcDecPart(t);
		return;
	}if (t->nodekind == StmLK) {
		Body(t->child[0]);
		return;
	}if (t->nodekind == StmtK) {
		Body(t->child[0]);
		return;
	}
}
//语义分析主程序
void Analyze(TreeNode* currentP) {
	CreatTable();
	initialize();
	bianli(currentP);
	Inputsymb("symbtable.txt");
	Inputdestroysymb("symbtable.txt");
}


