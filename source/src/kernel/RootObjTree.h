//
// RootObjTree.h 
//
// Create:liuzhicong,2013-3-9
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_ROOTOBJ_TREE_H_
#define _NGOS_ROOTOBJ_TREE_H_
/*
namespace NGOS
{

//һ������ӵ�У��ɱ�������̹���Ķ�����
//��һ������������UI�߳�

//��һ��ģ���� �������ĺ��ӳ���uiobject,����������һ��uiobjtree, uiobjtree�������ͨ��uiobject,����������ǳ���һЩ�������͹�����������������Դ
//�����ּ��������ĺÿ�����Ч��������ܺ��ȶ��ԣ������ƺ��������� (�е�����Ŀǰ�Ĳ����)
//��������������� ������������ ������ȷһ��������Ķ��󲻻ᳬ��������ʱ������Ϊ��������һ����������������������Ҫhittest������ʱ��������������������Ĳ��Һ���
//�ṩ���ַּ���������ʹ���߶�һ��ѡ�񣬸��õ��Ż�ϵͳ������

//���ʹ�� Ӳ���ṩ�� ָ��ԭ���������������������������

//Ϊ��������ܣ����úܶ�ؼ������ڹ����ڴ��ϡ��������Щ�����ڴ治����Ϊʹ���߶Գ���API�Ĳ���ȷ�ĵ����ͷŲ���������ĳЩ��ֵĽ��̱�������
//������ ��RootObjTreeֻ��һ�����̷���ʱ���ܷǳ���(disable process share)
//root obj tree�����н��̶��������̵Ķ�����ʺܼ�

//��ô���ӵĹ����ڴ���ƣ������Cд��(������C++ �ṩsim_class ,��ò��û̫���Ҫ��)��İ��Լ�Ū������
//�޽��̼�ͨ�ţ�c-s��ģʽ��� �����������̶ȵı���������ģ������������root obj tree owner process����,����һ������coding�ļ��侫�ɵģ�֧�ֲ������ʵ����ݽṹ(�᲻��̫������?�е����ں˿���)
//�������ǵ��Ժ�app framework������û�ʹ����һ�ָ������׳�������ԣ�������C/C++�� kernel�ĸ���C�ӿں���ƻ���������Ϊ��
//�������ٶ�UIObj��decode_handle������ 
//y
class RootObjTree
{
public:
	//���������������ڵĶ����ϵĶ������HitTest��Select
	//����ӣ�ɾ�����󣬸��¶����λ�����ԺͿɼ�����ʱ����Ҫ����������(���������ڽ��̽�������Чʱ����δ���������ɾ������򵥵ķ�������������Ҳ�����Ǹ����̣��������ں�����ӽ��̽�����֪ͨ��������ȷ��ɾ��)
	//���ÿ�����̶�������������ô�������в�ѯʱ����Ҫ��������Щ�������ﶼ����һ��
	ObjectIndex _index;

	//����ι������������ж��´λ���ʱӦ������Щ����
	//�ڸı��������֮ǰ��Ҫ�õ�һ����Ӧ����
	DirtyRectManager _dirtyRectManager;

	//����һ���ü�����õ�һ�ο������еĻ��ƽű�
	//������������ڼ䣬����סUIObjTree(����? ) ���Ա����Ƿǳ������ڲ�������
	char* GetRenderScript(RECT clipRect)
	{
		//�Ѿ�������zorder˳��?
		vecotr<OBJ_HANDLE> vh = _index.SelectVisibleObject(clipRect);
		for(int i=0;i<vh.size();++i)
		{
			AppendRenderScript(GetObjRenderScript(vh[i],clipRect));
		}
	}

	char* GetObjRenderScript(OBJ_HANDLE hObj,RECT clipRect)
	{

	}

	//���ʶ������ϵĶ���Ӧ���������ģ����ɾ��������������
	//��η�ֹһ�����̲�������̿�ס���������root obj tree����Ӧ

	void AddChild(OBJTREE_HANDLE hTree,OBJ_HANDLE hObj,OBJ_HANDLE hFather)
	{
		pObj = GetObjData(hObj);
		if(pObj == NULL)
		{
			return;
		}
		pObj->m_father = hFather;
		pObj->m_owner = hTree;

		pFather = GetObjData(hFather); 
		if(GetOwnerProcess(pFather) == getpid())
		{
			pFather->m_child.push_back(pObj);
			GetIndex(pFather)->updateIndex(pObj);
		}

		
		ReleaseObjData(hObj);
	}

	void RemoveChild(OBJ_HANDLE hObj)
	{

	}

	void MoveChild(OBJ_HANDLE hObj,OBJ_HANDLE newFather)
	{

	}
	
};

}
*/

#endif