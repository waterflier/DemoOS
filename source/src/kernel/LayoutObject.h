#ifndef _NGOS_LAYOUT_OBJECT_H_
#define _NGOS_LAYOUT_OBJECT_H_

class LayoutObject
{
	void GetOwner();
	//���ӹ�ϵ
	void GetParent();

	void GetChildCount();
	void GetChildByIndex();
	
	void StartLookupChild();
	void LookupNextChild();
	void EndLookupChild();
	
	//------------��������----------
	//λ������
	void GetCoordinateType();
	
	void GetObjRect();
	void GetObjAbsRect();
	//�ɼ�����
	void 
};


#endif //_NGOS_LAYOUT_OBJECT_H_