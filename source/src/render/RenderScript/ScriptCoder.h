#ifndef _NGOS_RENDER_SCRIPT_CODER_H_
#define _NGOS_RENDER_SCRIPT_CODER_H_
#include "RenderScript.h"
////这里提供script encode 和 decode操作， 一些基本的脚本原语,  demo阶段是不是先用lua了， 太重型
#define NGRE_SCRIPT_CODE_HANDLE void*

typedef struct NGREInitScriptCoderParam{
}NGREInitScriptCoderParam, *LPNGREInitScriptCoderParam;

NGRE_RESULT	NGREInitScriptCoder(LPNGREInitScriptCoderParam param);
void		NGREUninitScriptCoder();				
NGRE_RESULT	NGREDecodeScript(NGRE_SCRIPT_HANDLE hScript, NGRE_SCRIPT_CODE_HANDLE*phCode);
NGRE_RESULT	NGRERunScriptCode(NGRE_SCRIPT_CODE_HANDLE hCode);
void		NGREReleaseScriptCode(NGRE_SCRIPT_CODE_HANDLE hCode);

#endif