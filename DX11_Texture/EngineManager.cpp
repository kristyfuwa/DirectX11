#include "EngineManager.h"



EngineManager::EngineManager()
{
	m_pInput = 0;
	m_pGraphics = 0;
}


EngineManager::EngineManager(const EngineManager&)
{

}

EngineManager::~EngineManager()
{
}

bool EngineManager::Initialize()
{
	int screenWidth = 0, screenHeight = 0;
	int result;
	InitializeWindows(screenWidth, screenHeight);

	m_pInput = new Input;
	if (!m_pInput)
		return false;
	m_pInput->Initialize();

	m_pGraphics = new Graphics;
	if (!m_pGraphics)
		return false;
	result = m_pGraphics->Initialize(screenWidth, screenHeight, m_hWnd);
	if (!result)
		return false;


	return true;
}

void EngineManager::Shutdown()
{
	if (m_pGraphics)
	{
		m_pGraphics->Shutdown();
		delete m_pGraphics;
		m_pGraphics = 0;
	}
	if (m_pInput)
	{
		delete m_pInput;
		m_pInput = 0;
	}

	ShutdownWindows();
}

void EngineManager::Run()
{
	MSG msg;
	bool done, result = 1;
	ZeroMemory(&msg, sizeof(msg));
	done = false;

	while (!done)
	{
		// 循环进行消息处理，如果接收到WM_QUIT.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
	return;
}

bool EngineManager::Frame()
{
	if (m_pInput->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	m_pGraphics->Frame(0.0f);
	return true;
}

void EngineManager::InitializeWindows(int& screenWidth, int & screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//得到System class对象(应用程序句柄)
	ApplicationHandle = this;

	// 得到应用程序实例句柄
	m_hInstance = GetModuleHandle(NULL);

	// 应用程序名字
	m_applicationName = L"Engine";

	// 设置窗口类参数.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc; //指定回调函数
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //默认黑色窗口黑色背景
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// 注册窗口类
	RegisterClassEx(&wc);

	// 得到windows桌面分辨率
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 根据是否全屏设置不同的分辨率.
	if (FULL_SCREEN)
	{
		//全屏模式下，设置窗口大小为windows桌面分辨率.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 临时设置显示设备为全屏模式，注意：应用程序退出时候，将恢复系统默认设置。
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// 设置窗口的默认位置为(0,0).
		posX = posY = 0;
	}
	else
	{
		// 窗口模式：800*600.
		screenWidth = 800;
		screenHeight = 600;

		// 窗口位置,posX, posY窗口左上角坐标
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// 全屏和窗口使用不同的参数.
	if (FULL_SCREEN)
	{
		m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);
	}
	else
	{
		m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_OVERLAPPEDWINDOW,
			posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);
	}

	// 显示窗口并设置其为焦点.
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// 隐藏鼠标.
	//ShowCursor(false);
}

void EngineManager::ShutdownWindows()
{
	//显示光标.
	//ShowCursor(true);

	// 恢复默认显示设置.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	// 销毁应用程序实例.
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;
	ApplicationHandle = NULL;

	return;
}

LRESULT	CALLBACK EngineManager::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// 检测按键消息.
	case WM_KEYDOWN:
	{
		m_pInput->KeyDown((unsigned int)wparam);
		return 0;
	}
	case WM_KEYUP:
	{
		m_pInput->KeyUp((unsigned int)wparam);
		return 0;
	}
	case WM_SIZE:
	{
		int screenWidth = 0;
		int screenHeight = 0;

		screenWidth = LOWORD(lparam);
		screenHeight = HIWORD(lparam);

		//窗口大小发生改变时，重新初始化图形对象
		if (m_pGraphics)
		{
			bool result = m_pGraphics->Initialize(screenWidth, screenHeight, hwnd);
			if (!result)
				return false;
		}
		return 0;
	}
		//任何其它消息发送到windows缺省处理.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{

		// 窗口销毁消息.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// 窗口关闭消息.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	//MessageHandle过程处理其它所有消息.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
