#ifndef _NGOS_RESOURCE_MANAGER_H_
#define _NGOS_RESOURCE_MANAGER_H_


////resource mananger 提供id和路径加载到render能访问到内存的实现， 不提供对内存的解释
///render engine 的 ri讲内存解释为可供render使用的实例类型
#define NGRM_RESULT		long
#define NGRM_SUCCESS			0
#define NGRM_INVALID_TYPE		1
#define NGRM_INAVLID_ID			2
#define NGRM_INVALID_PATH		3
typedef char*		NGRM_ResId;
typedef char*		NGRM_ResPath;


typedef struct NGRM_InitParam{
}NGRM_InitParam;

NGRM_RESULT NGRM_Init(NGRM_InitParam* pParam);
void		NGRM_Uninit();

typedef enum NGRM_ResType{
	NGRM_ResType_Bitmap = 0,
	NGRM_ResType_Color = 1,
	NGRM_ResType_Count,
}NGRM_ResType;

NGRM_RESULT NGRM_AddResource(NGRM_ResId idRes, NGRM_ResPath pathRes, NGRM_ResType typeRes);
NGRM_RESULT NGRM_GetResource(NGRM_ResType typeRes, NGRM_ResId idRes, void** ppRes);


#endif