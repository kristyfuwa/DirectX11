#pragma once
//定义一些常用颜色
#include <d3d11.h>
#include <D3DX10math.h>

#if defined(DEBUG) || defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO 
#endif
#endif


/*
VS中自带了内存泄露检测工具，若要启用内存泄露检测，则在程序中包括以下语句：
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
它们的先后顺序不能改变。通过包括 crtdbg.h，
将malloc和free函数映射到其”Debug”版本_malloc_dbg和_free_dbg，
这些函数将跟踪内存分配和释放。此映射只在调试版本(在其中定义了_DEBUG)中发生。
#define语句将CRT堆函数的基版本映射到对应的”Debug”版本。
发布版本使用普通的malloc 和 free 函数。
*/

#if defined(DEBUG) || defined(_DEBUG)
#define  _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

#include <dxerr.h>
#include <cassert>

//一个简单的错误检测宏
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


#define NUM_LIGHTS 2


const D3DXVECTOR4 WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXVECTOR4 BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXVECTOR4 RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXVECTOR4 GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXVECTOR4 BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const D3DXVECTOR4 YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const D3DXVECTOR4 CYAN(0.0f, 1.0f, 1.0f, 1.0f); //蓝绿色
const D3DXVECTOR4 MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);//洋红色

const D3DXVECTOR4 BEACH_SAND(1.0f, 0.96f, 0.62f, 1.0f);
const D3DXVECTOR4 LIGHT_YELLOW_GREEN(0.48f, 0.77f, 0.46f, 1.0f);
const D3DXVECTOR4 DARK_YELLOW_GREEN(0.1f, 0.48f, 0.19f, 1.0f);
const D3DXVECTOR4 DARKBROWN(0.45f, 0.39f, 0.34f, 1.0f);


struct LightBuffer
{
	D3DXVECTOR4 lightPosition[NUM_LIGHTS];
	D3DXVECTOR4 lightColor[NUM_LIGHTS];
	D3DXVECTOR4 globalAmbient[NUM_LIGHTS];//光源的环境光反射系数
	D3DXVECTOR4 attenuation[NUM_LIGHTS];//衰减系数，x,y,z对应常量，线性和二次系数
	D3DXVECTOR4 spotlight[NUM_LIGHTS]; //对于spotlight，x，y分别存储内和外角cos值
	D3DXVECTOR3 lightDirection[NUM_LIGHTS];//平行光方向
	float		shininess[NUM_LIGHTS];//高光指数
};

struct MaterialBuffer
{
	D3DXVECTOR4 Ke;//材质自发光
	D3DXVECTOR4 Ka;//材质环境光系数；
};

struct CameraBuffer
{
	D3DXVECTOR4 cameraPosition;
};