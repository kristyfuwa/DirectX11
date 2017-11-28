#pragma once
#include <windows.h>

/////////////
// GLOBALS //
/////////////

const bool FULL_SCREEN = false; //�Ƿ�ȫ��
const bool VSYNC_ENABLED = true; //�Ƿ�ֱͬ��
const float SCREEN_DEPTH = 1000.0f; //��ȣ�Զ��
const float SCREEN_NEAR = 0.1f; //��ȣ�����


class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool	Initialize(int ,int,HWND);
	void	Shutdown();
	bool	Frame();

private:
	bool	Render();
};

