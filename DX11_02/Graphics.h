#pragma once
#include <windows.h>
#include <cstdlib>
#include "D3D.h"
#include "CameraEx.h"
#include "Model.h"
#include "ColorShader.h"
#include "CubeModel.h"
#include "AxisModel.h"
#include "LightShader.h"
#include "PlaneModel.h"
/////////////
// GLOBALS //
/////////////

const bool FULL_SCREEN = false; //是否全屏
const bool VSYNC_ENABLED = true; //是否垂直同步
const float SCREEN_DEPTH = 1000.0f; //深度，远点
const float SCREEN_NEAR = 0.1f; //深度，近点


class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool	Initialize(int ,int,HWND);
	void	Shutdown();
	bool	Frame();

public:
	CameraEx*			m_pCameraEx;
private:
	bool	Render();

	//定义一个D3D类成员变量
	D3D*			m_pD3D;

	Model*			m_pModel;
	AxisModel*		m_pAxisModel;
	CubeModel*		m_pCubeModel;
	ColorShader*	m_pColorShader;
	PlaneModel*		m_pPlaneModel;
	LightShader*	m_pLightShader;
};

