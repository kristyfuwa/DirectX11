#pragma once
#include <windows.h>
#include <cstdlib>
#include "D3D.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"
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
	bool	Frame(float);

public:
	
private:
	bool	Render(float);

	//����һ��D3D���Ա����
	D3D*				m_pD3D;
	Camera*			m_pCamera;
	Model*				m_pModel;
	ColorShader*	m_pColorShader;
};

