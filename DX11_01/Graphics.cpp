#include "Graphics.h"



Graphics::Graphics()
{
	m_pD3D = nullptr;
	m_pCamera = nullptr;
	m_pColorShader = nullptr;
	m_pModel = nullptr;
}


Graphics::Graphics(const Graphics&)
{

}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hWnd)
{
	bool result;

	//如果对象已经存在，先释放掉他们
	Shutdown();

	m_pD3D = new D3D;
	if (!m_pD3D)
		return false;


	result = m_pD3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_pCamera = new Camera();
	if (!m_pCamera)
		return false;
	
	//Set Camera Position

	m_pCamera->SetPosition(4.0f, 6.0f, -10.0f);

	m_pModel = new Model();
	if (!m_pModel)
		return false;
	result = m_pModel->Initialize(m_pD3D->GetDevice());
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	m_pColorShader = new ColorShader();
	if (!m_pColorShader)
		return false;

	result = m_pColorShader->Initialize(m_pD3D->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the color shader", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::Shutdown()
{
	if (m_pColorShader)
	{
		m_pColorShader->Shutdown();
		delete m_pColorShader;
		m_pColorShader = 0;
	}

	if (m_pModel)
	{
		m_pModel->Shutdown();
		delete m_pModel;
		m_pModel = 0;
	}

	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = 0;
	}

	if (m_pD3D)
	{
		m_pD3D->Shutdown();
		delete m_pD3D;
		m_pD3D = 0;
	}

	return;
}

bool Graphics::Frame()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	//设置framebuffer为浅蓝色
	m_pD3D->BeginScene(0.0f, 0.0f, 0.5f, 1.0f);

	//得到view矩阵
	m_pCamera->Render();

	m_pCamera->GetViewMatirx(viewMatrix);
	m_pD3D->GetWorldMatrix(worldMatrix);
	m_pD3D->GetProjectionMatrix(projectionMatrix);

	//把模型顶点和索引顶点放入管线，准备渲染
	m_pModel->Render(m_pD3D->GetDeviceContext());

	result = m_pColorShader->Render(m_pD3D->GetDeviceContext(), m_pModel->GetIndexCount(),worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;

	m_pD3D->EndScene();

	return true;
}

bool Graphics::Render()
{
	return true;
}
