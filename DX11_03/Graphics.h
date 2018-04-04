#pragma once
#include <windows.h>
#include <cstdlib>
#include "D3D.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"
#include "Cube.h"
#include "AxisModel.h"
#include "LightShader.h"
#include "Plane.h"
#include "Light.h"
#include "common.h"
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
	bool	Frame(float dt);

public:
	Camera*			m_pCameraEx;
private:
	bool	Render(float dt);

	//����һ��D3D���Ա����
	D3D*			m_pD3D;

	Model*			m_pModel;
	AxisModel*		m_pAxisModel;
	Cube*			m_pCube;
	ColorShader*	m_pColorShader;
	Plane*			m_pPlane;
	LightShader*	m_pLightShader;
};

