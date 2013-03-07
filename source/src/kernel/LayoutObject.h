#ifndef _NGOS_LAYOUT_OBJECT_H_
#define _NGOS_LAYOUT_OBJECT_H_

class LayoutObject
{
	void GetOwner();
	//父子关系
	void GetParent();

	void GetChildCount();
	void GetChildByIndex();
	
	void StartLookupChild();
	void LookupNextChild();
	void EndLookupChild();
	
	//------------基础属性----------
	//位置属性
	void GetCoordinateType();
	
	void GetObjRect();
	void GetObjAbsRect();
	//可见属性
	void 
};


#endif //_NGOS_LAYOUT_OBJECT_H_