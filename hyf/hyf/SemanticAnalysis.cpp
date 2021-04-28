#include"zjh.h"
//���ű��ʵ��

symbtable Scope[10000];//���ű�
symbtable DestroyScope[10000];//�������ű�
int destroylevel = 0; 
int level = 0;
bool flagtree=false;//�����ж��Զ��������������������Ǳ�����������������Ϊfalse
symbtable* table = nullptr;
//����һ�����ű�
int off=0,off2=0;
void CreatTable() {
	off2 = off;
	off = 0;
	level++;
}

//����һ�����ű�
void DestroyTable() {
	DestroyScope[destroylevel].next = Scope[level].next;
	destroylevel++;
	Scope[level].next = nullptr;
	off = off2;
	level = level - 1;
}

//�ӱ�ͷ��ʼ�����ν��ڵ��еı�ʶ�����ֺ�id�Ƚ��Ƿ���ͬ��ֱ
//���ҵ��˱�ʶ���򵽴��β�����ҵ���������ֵ��EntryΪ��ʶ��
//�ڷ��ű��е�λ�ã����򣬷���ֵΪ�١�
bool SearchoneTable(string id, int currentLevel, symbtable** Entry) {
		symbtable* t = Scope[currentLevel].next;
		while (t != nullptr) {                    //�ж��Ƿ���ͬ
			if (t->idname == id) {
				(*Entry) = t;
				return true;
			}
			else t = t->next;
		}
	return false;
}
//���ű��в��ұ�ʶ����
bool FindEntry(string id,bool flag, symbtable** Entry) {
	if (flag) {
		for (int i = level; i > 0; i--)
			if (SearchoneTable(id, i, Entry))
				return true;
	}
	else return SearchoneTable(id, level, Entry);
	return false;
}
//�ǼǱ�ʶ�������Ե����ű�
bool Enter(string Id, AttributeIR* AttribP, symbtable** Entry) {
	bool present = FindEntry(Id,false, Entry);
	if (present == true) {
		cout <<"��ʶ���ظ�����"<<endl;
	}
	else {                                                //������ű�
		
		symbtable* newsymbtable = new symbtable;
		newsymbtable->idname = Id;
		newsymbtable->attrIR = *AttribP;          //��ʶ����Ϣ��
		/*cout << "���" + Id ;
		cout <<"����"<< AttribP->More.VarAttr.level<<" " << AttribP->More.ProcAttr.level<<endl;*/
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
//������в�������
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
//��ӡ���ű�
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
//������ű�
void Inputsymb(string path) {
	ofstream ouput;
	ouput.open(path);
	for (int i = 1; i < level+1; i++) {
		symbtable* t = Scope[i].next;
		while (t != nullptr) {
			//��ѡ������һЩ���ű���Ϣ��
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
//����ѳ����ط��ű�
void Inputdestroysymb(string path) {
	ofstream ouput;
	ouput.open(path,ios::app);
	ouput << endl << endl << endl << endl << endl;
	ouput <<"******************************��ɾ�����ű�*************************************"<<endl;
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
//��ʼ�����������ڲ���ʾ��������ʼ���������ͣ��ַ����ͣ��������͵��ڲ���ʾ˵���������������;�Ϊ�������ͣ��ڲ���ʾ�̶���
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
//�Զ��������ڲ��ṹ��������
typeIR* nameType(TreeNode* t) {
	symbtable** Entry = new symbtable*;
	int i = 0;
	if (flagtree)i++;
	bool flag = FindEntry(t->name[i],false, Entry);
	if (!flag) cout << t->lineno<<"�Զ�������" + t->name[i] + "δ����"<<endl;
	else return (*Entry)->attrIR.idtype;

	return nullptr;
}
//���������ڲ���ʾ������
typeIR* arrayType(TreeNode* t) {
	if (t->attr.arrayAttr.low > t->attr.arrayAttr.up) {
		cout << "�����±�Խ��"<<endl;
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
//�����¼���͵��ڲ���ʾ����
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
					cout << t->lineno << "record������"<<endl;
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

//���ͷ��������������ͷ������������﷨���ĵ�ǰ�ڵ����ͣ����쵱ǰ���͵��ڲ�
//��ʾ���������ַ���ظ�Ptr�����ڲ���ʾ�ĵ�ַ��
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

//�����������ַ���������
void TypeDecPart(TreeNode* t) {
	if (t == nullptr)
		return;
	
	//����д�������
	symbtable** Entry = new symbtable*;
	int i = 0;
	if (t->kind.dec == IdK) i++;
	for (; i < t->name.size(); i++) {
		bool flag = FindEntry(t->name[0],false, Entry);
		if (flag)
			cout << t->lineno<<"���ͱ�ʶ��" + t->name[0] + "�ظ�����"<<endl;
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
//�����������ַ���������
void VarDecList(TreeNode* t) {
	if (t == nullptr)
		return;
	//����д�������
	symbtable** Entry = new symbtable*;
	int i = 0;
	if (t->kind.dec == IdK) i++;
	for (; i < t->name.size(); i++) {
		
		bool flag = FindEntry(t->name[i],false, Entry);
		if (flag)
			cout << t->lineno<<"������ʶ��" + t->name[i] + "�ظ�����"<<endl;
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
//�βη���������
ParamTable* ParaDecList(TreeNode* t) {
	if (t == nullptr)
		return nullptr;
	ParamTable* parat = nullptr;
	ParamTable* paralist = nullptr,*dt=nullptr;
	for (int i = 0; i < t->name.size(); i++) {
		symbtable** Entry = new symbtable*;
		bool flag = FindEntry(t->name[i],false, Entry);
		if (flag)
			cout << t->lineno<<"�βα�ʶ��" + t->name[i] + "�ظ�����"<<endl;
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
//�����������ַ���������
void ProcDecPart(TreeNode* t) {
	if (t == nullptr)
		return;
	//��������ӽ����ű�
	AttributeIR* procir = new AttributeIR;
	procir->kind = procKind;
	procir->More.ProcAttr.level = level;
	ParamTable* paramt=new ParamTable;
	procir->More.ProcAttr.param = paramt;
	symbtable** Entry = new symbtable*;
	Enter(t->name[0], procir, Entry);
	//����д�������
	CreatTable();
	//�����βΣ������βα�������ķ��ű��ڵ�param��
	(*Entry)->attrIR.More.ProcAttr.param=ParaDecList(t->child[0]);   
	
	bianli(t->child[1]);                       //����                
	Body(t->child[2]->child[0]);                                  //���
	DestroyTable();
	return;
}
//��������ͷ��������
//symbtable* HeadProcess(TreeNode* t) {
//
//	return nullptr;
//}
//���ʽ����������
typeIR* Expr(TreeNode* t, AccessKind* Ekind) {
	if (t->kind.exp == IdEK) {
		symbtable** Entry = new symbtable*;
		bool flag = FindEntry(t->name[0], true, Entry);
		if (!flag)
			cout << t->lineno << "���ʽ�����" + t->name[0] + "δ����"<<endl;
		else {
			if (t->child[0] != nullptr) {
				if ((*Entry)->attrIR.idtype->kind == arrayTy)                //�������ͷ���Ԫ�ص��ڲ���ʾ��kind
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
					cout << t->lineno << "record��û�д�Ԫ��"<<endl;
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

		else cout << t->lineno << "���ʽ������Ͳ�ƥ��"<<endl;
	}
	return nullptr;
}
//TypeKind assignstatementpart(TreeNode* t) {
//	if (t->kind.exp == IdEK) {
//		symbtable** Entry = new symbtable*;
//		bool flag = FindEntry(t->name[0], true,Entry);
//		if (!flag)
//			cout << "��ֵ�����" + t->name[0] + "δ����";
//		else {
//			if ((*Entry)->attrIR.idtype->kind == arrayTy)                //�������ͷ���Ԫ�ص��ڲ���ʾ��kind
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
//		else cout << "��ֵ����ʽ�Ҷ����Ͳ�ƥ��";
//	}
//	return intTy;
//}
//��ֵ����������
void assignstatement(TreeNode* t) {
	if (t->child[0]->kind.exp != IdEK)
		cout << t->lineno << "��ֵ�����˲��Ǳ�ʶ��"<<endl;
	else {
		symbtable** Entry =new symbtable*;
		bool flag = FindEntry(t->child[0]->name[0],true, Entry);
		if (!flag)
			cout << t->lineno<< "��ʶ��" + t->child[0]->name[0] + "δ����"<<endl;
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
					if(flagg) cout << t->lineno << "record��û�д�Ԫ��"<<endl;
				}
			}
			AccessKind* Ekind=new AccessKind;
			typeIR *kindright = Expr(t->child[1],Ekind);
			if (kindleft != kindright)
				cout << t->lineno<<"��ֵ����������Ͳ�ƥ��"<<endl;
		}
	}
}
//���̵���������������
void callstatement(TreeNode* t) {
	symbtable** Entry = new symbtable*;
	bool flag = FindEntry(t->child[0]->name[0],true, Entry);
	if (!flag)
		cout << t->lineno<< "����" + t->child[0]->name[0] + "δ����"<<endl;
	else {
		if ((*Entry)->attrIR.kind != procKind)
			cout << t->lineno<< "��ʶ�����Ǻ������ͣ��޷�����"<<endl;
		else {
			if (t->child[1] == nullptr) {
				if ((*Entry)->attrIR.More.ProcAttr.param == nullptr)
					return;
				else cout << t->lineno << "������ƥ��"<<endl;
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
						else cout << t->lineno << "������ƥ��"<<endl;
					}
					else cout << t->lineno << "������ƥ��"<<endl;
				}
					
				b = b->sibling;
				a = a->next;
			}
			if (a != nullptr || b != nullptr) {
				cout << t->lineno << "������ƥ��"<<endl;
			}
				
			return;
		}
	}
}
//void ifstatmentpart(TreeNode* t) {                                                                  //if���ֳ��򣬻�û���ж���������Ƿ�ͬһ����
//
//	if (t->kind.exp == IdEK) {
//		symbtable** Entry = new symbtable*;
//		bool flag = FindEntry(t->name[0],true, Entry);
//		if (!flag) {
//			cout << "if����б�ʶ��" + t->name[0] + "δ����";
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
//����������������                                                                         
void ifstatment(TreeNode* t) {
	//if�﷨��child[0]Ϊif���ʽ,��Ϊ������
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
//			cout << "while����б�ʶ��" + t->name[0] + "δ����";
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
//ѭ��������������
void whilestatement(TreeNode* t) {
	//while�﷨��child[0]Ϊif���ʽ,ͬif
	TreeNode* whilet = t->child[0];
	AccessKind *Ekind=new AccessKind;
	Expr(whilet, Ekind);
	TreeNode* dot = t->child[1];
	while (dot != nullptr) {
		statement(dot);
		dot = dot->sibling;
	}
}
//��������������
void readstatement(TreeNode* t) {
	symbtable** Entry = new symbtable*;
	//for (int i = 0; i < t->name.size(); i++) {}
	bool flag = FindEntry(t->name[0],true, Entry);
	if (!flag)
		cout <<t->lineno<< "������ʶ��" + t->name[0] + "δ����"<<endl;
	return;
}
//TypeKind writestatementpart(TreeNode* t) {
//	if (t->kind.exp == IdEK) {
//		symbtable** Entry = new symbtable*;
//		bool flag = FindEntry(t->name[0],true, Entry);
//		if (!flag)
//			cout <<t->lineno<< "д�����" + t->name[0] + "δ����";
//		else {
//			if ((*Entry)->attrIR.idtype->kind == arrayTy)                //�������ͷ���Ԫ�ص��ڲ���ʾ��kind
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
//		else cout <<t->lineno<<"д�����ʽ���Ͳ�ƥ��";
//	}
//	return intTy;
//}
//д������������
void writestatement(TreeNode* t) {
	AccessKind* Ekind = new AccessKind;
	Expr(t->child[0],Ekind);
	return;
}
//�����������������
void returnstatment(TreeNode* t) {
	return;
}
//������з���������
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
//ִ���岿�ַ�����������
void Body(TreeNode* t) {
	if (t == nullptr) return;
	while (t != nullptr) {
		statement(t);
		t = t->sibling;
	}
}

//��������Ĵ����������
typeIR* arrayVar(TreeNode* t) {

	return nullptr;
}
//��¼������������ķ���������
typeIR* recordVar(TreeNode* t) {
	return nullptr;
}

//�����﷨����������Ӧ�ĺ��������﷨�����
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
//�������������
void Analyze(TreeNode* currentP) {
	CreatTable();
	initialize();
	bianli(currentP);
	Inputsymb("symbtable.txt");
	Inputdestroysymb("symbtable.txt");
}


