#ifndef _NGOS_RENDER_SCRIPT_H_
#define _NGOS_RENDER_SCRIPT_H_
#include "RenderDefine.h"
#include <stdlib.h>
/*���⣺  ����������⣬һ�� Ϊ����Ҫ��Ⱦ�ű���1. ������Ⱦʵ�� 2. Ϊipc�ṩmarshalʵ��  3.���㶨�ư��������� 4....
						���� ����������һ�� ��Ⱦ�ű���Ҫ��ǿ���������������ʽ�� �������ã� �������ݣ� �ṹ�����ƣ�
			1. �ű��ں��費��Ҫ�ɶ��Ժ��ı����루�����ں�û�пɶ��ԣ�Ҳ����ͨ���ɶ��ű����Եİ�װ���ṩ�ɶ��ԣ� 
			2. �ű���render object��ʵ���Ƿ�Ҫ͸��,����render objectʵ�ֻ��ƵĴ����д��ʲô���ӣ���ͳ����Ⱦʽapi�� 
				���ɽű���api�� �ṩԤ���ɵĽű�����ֻ��������������ű��Ŀɶ�����~
			3. ��λ��Դ��ԭ�(�����ı��� �زģ� �������ã�gl�ű��� ����һ�п������õĴ���ڴ涫����  
			�����ڴ棬 Զ���ڴ棬 �����Կ����棬 Զ���Կ����棬 �����豸)�� ��λ��Դ��ʵ�ֲַ㵽����ʵ��

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