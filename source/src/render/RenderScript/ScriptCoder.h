#ifndef _NGOS_RENDER_SCRIPT_CODER_H_
#define _NGOS_RENDER_SCRIPT_CODER_H_
#include "RenderScript.h"
////�����ṩscript encode �� decode������ һЩ�����Ľű�ԭ��,  demo�׶��ǲ�������lua�ˣ� ̫����
#define NGRE_SCRIPT_CODE_HANDLE void*

typedef struct NGREInitScriptCoderParam{
}NGREInitScriptCoderParam, *LPNGREInitScriptCoderParam;

NGRE_RESULT	NGREInitScriptCoder(LPNGREInitScriptCoderParam param);
void		NGREUninitScriptCoder();				
NGRE_RESULT	NGREDecodeScript(NGRE_SCRIPT_HANDLE hScript, NGRE_SCRIPT_CODE_HANDLE*phCode);
NGRE_RESULT	NGRERunScriptCode(NGRE_SCRIPT_CODE_HANDLE hCode);
void		NGREReleaseScriptCode(NGRE_SCRIPT_CODE_HANDLE hCode);

#endif