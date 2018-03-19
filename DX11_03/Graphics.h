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

const bool FULL_SCREEN = false; //�Ƿ�ȫ��
const bool VSYNC_ENABLED = true; //�Ƿ�ֱͬ��
const float SCREEN_DEPTH = 1000.0f; //��ȣ�Զ��
const float SCREEN_NEAR = 0.1f; //��ȣ�����


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

	//����һ��D3D���Ա����
	D3D*			m_pD3D;

	Model*			m_pModel;
	AxisModel*		m_pAxisModel;
	CubeModel*		m_pCubeModel;
	ColorShader*	m_pColorShader;
	PlaneModel*		m_pPlaneModel;
	LightShader*	m_pLightShader;
};
