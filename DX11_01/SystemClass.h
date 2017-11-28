#pragma once
//����ú��ܹ�����windowsͷ�ļ��Ĵ�С��ʹ������������һЩ����Ҫ���ļ����ӿ����ʱ��
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "InputClass.h"
#include "GraphicsClass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool		Initialize();
	void		Shutdown();
	void		Run();

	LRESULT	CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool		Frame();
	void		InitializeWindows(int& screenWidth, int & screenHeight);
	void		ShutdownWindows();

	LPCWSTR		m_applicationName;
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;

	InputClass*		m_pInput;
	GraphicsClass*	m_pGraphics;
};

//���徲̬�Ļص������Լ�Ӧ�ó�����
//��Ϊ���崰����ʱ�򣬱���ָ�����ڻص����������������þ�̬�Ļص�����WndProc
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

