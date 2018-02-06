#pragma once
//����һЩ������ɫ
#include <d3d11.h>
#include <D3DX10math.h>

#if defined(DEBUG) || defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO 
#endif
#endif


/*
VS���Դ����ڴ�й¶��⹤�ߣ���Ҫ�����ڴ�й¶��⣬���ڳ����а���������䣺
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
���ǵ��Ⱥ�˳���ܸı䡣ͨ������ crtdbg.h��
��malloc��free����ӳ�䵽�䡱Debug���汾_malloc_dbg��_free_dbg��
��Щ�����������ڴ������ͷš���ӳ��ֻ�ڵ��԰汾(�����ж�����_DEBUG)�з�����
#define��佫CRT�Ѻ����Ļ��汾ӳ�䵽��Ӧ�ġ�Debug���汾��
�����汾ʹ����ͨ��malloc �� free ������
*/

#if defined(DEBUG) || defined(_DEBUG)
#define  _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

#include <dxerr.h>
#include <cassert>

//һ���򵥵Ĵ������
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
	#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif




const D3DXVECTOR4 WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXVECTOR4 BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXVECTOR4 RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXVECTOR4 GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXVECTOR4 BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const D3DXVECTOR4 YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const D3DXVECTOR4 CYAN(0.0f, 1.0f, 1.0f, 1.0f); //����ɫ
const D3DXVECTOR4 MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);//���ɫ

const D3DXVECTOR4 BEACH_SAND(1.0f, 0.96f, 0.62f, 1.0f);
const D3DXVECTOR4 LIGHT_YELLOW_GREEN(0.48f, 0.77f, 0.46f, 1.0f);
const D3DXVECTOR4 DARK_YELLOW_GREEN(0.1f, 0.48f, 0.19f, 1.0f);
const D3DXVECTOR4 DARKBROWN(0.45f, 0.39f, 0.34f, 1.0f);