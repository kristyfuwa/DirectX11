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
		// ѭ��������Ϣ����������յ�WM_QUIT.
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

	//�õ�System class����(Ӧ�ó�����)
	ApplicationHandle = this;

	// �õ�Ӧ�ó���ʵ�����
	m_hInstance = GetModuleHandle(NULL);

	// Ӧ�ó�������
	m_applicationName = L"Engine";

	// ���ô��������.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc; //ָ���ص�����
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //Ĭ�Ϻ�ɫ���ں�ɫ����
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// ע�ᴰ����
	RegisterClassEx(&wc);

	// �õ�windows����ֱ���
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// �����Ƿ�ȫ�����ò�ͬ�ķֱ���.
	if (FULL_SCREEN)
	{
		//ȫ��ģʽ�£����ô��ڴ�СΪwindows����ֱ���.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// ��ʱ������ʾ�豸Ϊȫ��ģʽ��ע�⣺Ӧ�ó����˳�ʱ�򣬽��ָ�ϵͳĬ�����á�
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// ���ô��ڵ�Ĭ��λ��Ϊ(0,0).
		posX = posY = 0;
	}
	else
	{
		// ����ģʽ��800*600.
		screenWidth = 800;
		screenHeight = 600;

		// ����λ��,posX, posY�������Ͻ�����
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// ȫ���ʹ���ʹ�ò�ͬ�Ĳ���.
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

	// ��ʾ���ڲ�������Ϊ����.
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// �������.
	//ShowCursor(false);
}

void EngineManager::ShutdownWindows()
{
	//��ʾ���.
	//ShowCursor(true);

	// �ָ�Ĭ����ʾ����.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	// ����Ӧ�ó���ʵ��.
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;
	ApplicationHandle = NULL;

	return;
}

LRESULT	CALLBACK EngineManager::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// ��ⰴ����Ϣ.
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

		//���ڴ�С�����ı�ʱ�����³�ʼ��ͼ�ζ���
		if (m_pGraphics)
		{
			bool result = m_pGraphics->Initialize(screenWidth, screenHeight, hwnd);
			if (!result)
				return false;
		}
		return 0;
	}
		//�κ�������Ϣ���͵�windowsȱʡ����.
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

		// ����������Ϣ.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// ���ڹر���Ϣ.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	//MessageHandle���̴�������������Ϣ.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
