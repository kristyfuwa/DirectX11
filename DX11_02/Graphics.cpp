#include "Graphics.h"



Graphics::Graphics()
{
	m_pD3D = nullptr;
	m_pCameraEx = nullptr;
	m_pColorShader = nullptr;
	m_pModel = nullptr;
	m_pAxisModel = nullptr;
	m_pLight = nullptr;
	m_pLightShader = nullptr;
	m_pCubeModel = nullptr;
	m_pPlaneModel = nullptr;
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

	m_pCameraEx = new CameraEx();
	if (!m_pCameraEx)
		return false;
	
	//Set Camera Position

	m_pCameraEx->setPosition(&D3DXVECTOR3(8.0f, 3.0f, -20.0f));

	m_pModel = new Model();
	if (!m_pModel)
		return false;
	result = m_pModel->Initialize(m_pD3D->GetDevice());
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	m_pAxisModel = new AxisModel();
	if (!m_pAxisModel)
		return false;
	//初始化坐标轴模型对象
	result = m_pAxisModel->Initialize(m_pD3D->GetDevice());
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the Axis object", L"Error", MB_OK);
		return false;

	}

	//创建平面模型
	m_pPlaneModel = new PlaneModel();
	if (!m_pPlaneModel)
		return false;
	//初始化水平模型对象
	result = m_pPlaneModel->Initialize(m_pD3D->GetDevice());
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the water model", L"Error", MB_OK);
		return false;
	}
	
	//初始化Cube模型
	m_pCubeModel = new CubeModel();
	if (!m_pCubeModel)
		return false;
	result = m_pCubeModel->Initialize(m_pD3D->GetDevice());
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the cube model", L"Error", MB_OK);
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

	//创建光照shader类
	m_pLightShader = new LightShader();
	if (!m_pLightShader)
		return false;
	result = m_pLightShader->Initialize(m_pD3D->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the light shader", L"Error", MB_OK);
		return false;
	}

	//创建光源对象
	m_pLight = new Light();
	if (!m_pLight)
		return false;

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

	if (m_pCameraEx)
	{
		delete m_pCameraEx;
		m_pCameraEx = 0;
	}

	if (m_pAxisModel)
	{
		m_pAxisModel->Shutdown();
		delete m_pAxisModel;
		m_pAxisModel = 0;
	}

	if (m_pCubeModel)
	{
		m_pCubeModel->Shutdown();
		delete m_pCubeModel;
		m_pCubeModel = 0;
	}

	if (m_pLight)
	{
		delete m_pLight;
		m_pLight = 0;
	}

	if (m_pLightShader)
	{
		m_pLightShader->Shutdown();
		delete m_pLightShader;
		m_pLightShader = 0;
	}

	if (m_pPlaneModel)
	{
		m_pPlaneModel->Shutdown();
		delete m_pPlaneModel;
		m_pPlaneModel = 0;
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
	bool result;
	// 调用Render函数，渲染3D场景
	// Render是GraphicsClass的私有函数.

	result = Render();
	if (!result)
		return false;
	return true;
}

bool Graphics::Render()
{

	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	//设置framebuffer为浅蓝色
	m_pD3D->BeginScene(0.0f, 0.0f, 0.5f, 1.0f);

	m_pCameraEx->getViewMatrix(&viewMatrix);
	m_pD3D->GetWorldMatrix(worldMatrix);
	m_pD3D->GetProjectionMatrix(projectionMatrix);

	m_pAxisModel->Render(m_pD3D->GetDeviceContext());
	//用shader渲染
	result = m_pColorShader->Render(m_pD3D->GetDeviceContext(), m_pAxisModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;

	//把模型顶点和索引顶点放入管线，准备渲染
	m_pModel->Render(m_pD3D->GetDeviceContext());
	result = m_pColorShader->Render(m_pD3D->GetDeviceContext(), m_pModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;

	D3DXVECTOR3 cameraPos;
	D3DXVECTOR4 realCameraPos;
	D3DXVECTOR4 Ke = D3DXVECTOR4(0.8, 0.0,0.2, 1.0);
	D3DXVECTOR4 Ka = D3DXVECTOR4(0.2, 0.2, 0.2, 1.0);
	D3DXVECTOR4 Kd = D3DXVECTOR4(0.7, 0.5, 0.6,1.0);
	D3DXVECTOR4 Ks = D3DXVECTOR4(1.0, 1.0,1.0, 1.0);
	m_pCameraEx->getPosition(&cameraPos);
	realCameraPos = D3DXVECTOR4(cameraPos.x, cameraPos.y, cameraPos.z, 1.0);

	m_pCubeModel->Render(m_pD3D->GetDeviceContext());
	result = m_pLightShader->Render(m_pD3D->GetDeviceContext(), m_pCubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_pLight->GetPosition(), m_pLight->GetLightColor(), m_pLight->GetGlobalAmbient(),
		realCameraPos , Ke, Ka, Kd, Ks, m_pLight->GetDirection(), m_pLight->GetShininess());
	if (!result)
		return false;

	Ke = D3DXVECTOR4(0.2, 0.8, 0.0, 1.0);
	Ka = D3DXVECTOR4(0.3, 0.3, 0.3, 1.0);
	Kd = D3DXVECTOR4(1.0, 1.0, 1.0, 1.0);
	Ks = D3DXVECTOR4(1.0, 1.0, 1.0, 1.0);

	m_pPlaneModel->Render(m_pD3D->GetDeviceContext());
	result = m_pLightShader->Render(m_pD3D->GetDeviceContext(), m_pPlaneModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_pLight->GetPosition(), m_pLight->GetLightColor(), m_pLight->GetGlobalAmbient(),
		realCameraPos, Ke, Ka, Kd, Ks, m_pLight->GetDirection(), m_pLight->GetShininess());
	if (!result)
		return false;

	m_pD3D->EndScene();

	return true;
}
