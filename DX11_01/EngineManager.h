#pragma once
//����ú��ܹ�����windowsͷ�ļ��Ĵ�С��ʹ������������һЩ����Ҫ���ļ����ӿ����ʱ��
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Input.h"
#include "Graphics.h"
#include "Timer.h"

class EngineManager
{
public:
	EngineManager();
	EngineManager(const EngineManager&);
	~EngineManager();

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

	Input*		m_pInput;
	Graphics*	m_pGraphics;
	Timer*		m_pTimer;
};

//���徲̬�Ļص������Լ�Ӧ�ó�����
//��Ϊ���崰����ʱ�򣬱���ָ�����ڻص����������������þ�̬�Ļص�����WndProc
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static EngineManager* ApplicationHandle = 0;

