//
// RenderScript.h 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_RENDER_SCRIPT_H_
#define _NGOS_RENDER_SCRIPT_H_





NGOS_RENDER_SCRIPT_BUFFER_HANDLE CreateRenderScript();
int ReleaseRenderScript(NGOS_RENDER_SCRIPT_BUFFER_HANDLE hRenderScript);
int AppendRenderScript(NGOS_RENDER_SCRIPT_BUFFER_HANDLE hDest,NGOS_RENDER_SCRIPT_BUFFER_HANDLE hSrc);

#endif //_NGOS_RENDER_SCRIPT_H_