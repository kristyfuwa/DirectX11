#include "Graphics.h"



Graphics::Graphics()
{
	m_pD3D = nullptr;
	m_pCameraEx = nullptr;
	m_pColorShader = nullptr;
	m_pModel = nullptr;
	m_pAxisModel = nullptr;
	m_pWaterModel = nullptr;
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

	m_pCameraEx->setPosition(&D3DXVECTOR3(0.0f, 0.0f, -20.0f));

	m_pModel = new Model();
	if (!m_pModel)
		return false;
	result = m_pModel->Initialize(m_pD3D->GetDevice(),300,300,1.0f);
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

	//创建水模型
	m_pWaterModel = new WaterModel();
	if (!m_pWaterModel)
		return false;
	//初始化水模型对象
	result = m_pWaterModel->Initialize(m_pD3D->GetDevice(), 257, 257, 0.5f, 0.03f, 3.25f, 0.4f);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the water model", L"Error", MB_OK);
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

	if (m_pWaterModel)
	{
		m_pWaterModel->Shutdown();
		delete m_pWaterModel;
		m_pWaterModel = 0;
	}

	if (m_pD3D)
	{
		m_pD3D->Shutdown();
		delete m_pD3D;
		m_pD3D = 0;
	}

	return;
}

bool Graphics::Frame(float dt)
{
	bool result;
	// 调用Render函数，渲染3D场景
	// Render是GraphicsClass的私有函数.

	result = Render(dt);
	if (!result)
		return false;
	return true;
}

bool Graphics::Render(float dt)
{

	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	//设置framebuffer为浅蓝色
	m_pD3D->BeginScene(0.0f, 0.0f, 0.5f, 1.0f);

	//得到view矩阵
	//m_pCamera->Render();

	m_pCameraEx->getViewMatrix(&viewMatrix);
	m_pD3D->GetWorldMatrix(worldMatrix);
	m_pD3D->GetProjectionMatrix(projectionMatrix);

	//设置为实体填充模式
	m_pD3D->SetFillMode(D3D11_FILL_SOLID);

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

	static float t_base = 0.0f;
	static float total_time = 0.0f;
	total_time += dt;
	if (total_time - t_base >= 0.25f)
	{
		t_base += 0.25f;
		int i = 5 + rand() % 250;
		int j = 5 + rand() % 250;

		//得到1到2之间的一个浮点数
		float r = 1.0f + (float)(rand()) / (float)RAND_MAX*(2.0f - 1.0f);
		m_pWaterModel->Disturb(i, j, r);
	}

	m_pWaterModel->Update(m_pD3D->GetDeviceContext(), dt);
	//设置线框模式
	m_pD3D->SetFillMode(D3D11_FILL_WIREFRAME);
	//把模型顶点和索引缓冲放入管线，准备渲染
	m_pWaterModel->Render(m_pD3D->GetDeviceContext());
	
	//用shader渲染
	result = m_pColorShader->Render(m_pD3D->GetDeviceContext(),m_pWaterModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;
	m_pD3D->EndScene();

	return true;
}
