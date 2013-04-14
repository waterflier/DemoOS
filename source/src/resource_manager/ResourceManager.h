#ifndef _NGOS_RESOURCE_MANAGER_H_
#define _NGOS_RESOURCE_MANAGER_H_


////resource mananger �ṩid��·�����ص�render�ܷ��ʵ��ڴ��ʵ�֣� ���ṩ���ڴ�Ľ���
///render engine �� ri���ڴ����Ϊ�ɹ�renderʹ�õ�ʵ������
#define NGRM_RESULT		long
#define NGRM_SUCCESS	0
typedef NGRM_ResId		const char*;
typedef NGRM_ResPath	const char*;


typedef struct NGRM_InitParam{
}NGRM_InitParam;

NGRM_RESULT NGRM_Init(NGRM_InitParam* pParam);
void		NGRM_Uninit();

typedef NGRM_ResType		long;
#define NGRM_ResType_Bitmap	1
NGRM_RESULT NGRM_AddResource(NGRM_ResId idRes, NGRM_ResPath pathRes, NGRM_ResType typeRes);
NGRM_RESULT NGRM_GetResurce(NGRM_ResId idRes, void** ppRes);


#endif