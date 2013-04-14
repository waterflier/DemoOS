#ifndef _NGOS_RENDER_SCRIPT_H_
#define _NGOS_RENDER_SCRIPT_H_
#include "RenderDefine.h"
#include <stdlib.h>
/*问题：  最基本的问题，一， 为何需要渲染脚本：1. 隔离渲染实现 2. 为ipc提供marshal实现  3.方便定制半主动对象 4....
						二， 依赖于问题一， 渲染脚本需要多强的语言能力（表达式， 迭代调用， 参数传递， 结构化控制）
			1. 脚本内核需不需要可读性和文本编码（即便内核没有可读性，也可以通过可读脚本语言的包装来提供可读性） 
			2. 脚本对render object的实现是否要透明,就是render object实现绘制的代码会写成什么样子（传统的渲染式api， 
				生成脚本的api， 提供预生成的脚本代码只传入参数（依赖脚本的可读））~
			3. 定位资源的原语？(包括文本， 素材， 甚至配置，gl脚本， 其他一切可以重用的大块内存东西，  
			本地内存， 远程内存， 本地显卡缓存， 远程显卡缓存， 网络设备)， 定位资源的实现分层到哪里实现

*/
#define NGRE_SCRIPT_HANDLE	void*

#define NGRE_SCRIPT_DECODE_FAIL		-1
#define NGRE_SCRIPT_RUN_FAIL		-100
typedef struct NGREScpritInitParam{
}*LPNGREScriptInitParam;

NGRE_RESULT NGREScriptInit(LPNGREScriptInitParam pParam);
NGRE_RESULT	NGRERunScriptCode(NGRE_SCRIPT_HANDLE hScript);
void		NGREScriptUninit();

//drawbitmap
//drawtext
//fillrect
//
//
//
//0
//
//1
//
//2
//args
//res
//
//3
//
//
//4 framebuffer
//
//
//5
//
//vdisk=>memeory=>hardware
//`
//fillrect 0,0,width height
//drawbitmap bkg,9,0,100,100,clip,mask,trans,mesh
//drawbitmap bkg2,200,200,100,100
//
//bitmp = drawtext str,font
//drawbitmap bitmap,trans,mesh
//
//lua
//
//byte code
//
//
//sub drawtasklisk 
//
//
//end


#endif