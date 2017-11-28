#include "GraphicsClass.h"



GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass&)
{

}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hWnd)
{
	return true;
}

void GraphicsClass::Shutdown()
{
	return;
}

bool GraphicsClass::Frame()
{
	return true;
}

bool GraphicsClass::Render()
{
	return true;
}
