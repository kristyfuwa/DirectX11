#include "Graphics.h"



Graphics::Graphics()
{
	m_pD3D = nullptr;
	m_pCameraEx = nullptr;
	m_pColorShader = nullptr;
	m_pModel = nullptr;
	m_pAxisModel = nullptr;
	m_pLightShader = nullptr;
	m_pCube = nullptr;
	m_pPlane = nullptr;
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

	m_pCameraEx = new Camera();
	if (!m_pCameraEx)
		return false;
	
	//Set Camera Position

	m_pCameraEx->setPosition(&D3DXVECTOR3(8.0f, 3.0f, -20.0f));

	m_pModel = new Model();
	if (!m_pModel)
		return false;
	result = m_pModel->initialize(m_pD3D->GetDevice());
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
	m_pPlane = new Plane();
	if (!m_pPlane)
		return false;
	//初始化水平模型对象
	result = m_pPlane->initialize(m_pD3D->GetDevice());
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the water model", L"Error", MB_OK);
		return false;
	}
	
	//初始化Cube模型
	m_pCube = new Cube();
	if (!m_pCube)
		return false;
	result = m_pCube->Initialize(m_pD3D->GetDevice(),"cube.txt");
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the cube model", L"Error", MB_OK);
		return false;
	}

	m_pColorShader = new ColorShader();
	if (!m_pColorShader)
		return false;

	result = m_pColorShader->initialize(m_pD3D->GetDevice(), hWnd);
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

	return true;
}

void Graphics::Shutdown()
{
	if (m_pColorShader)
	{
		m_pColorShader->shutDown();
		delete m_pColorShader;
		m_pColorShader = 0;
	}

	if (m_pModel)
	{
		m_pModel->shutDown();
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

	if (m_pCube)
	{
		m_pCube->Shutdown();
		delete m_pCube;
		m_pCube = 0;
	}


	if (m_pLightShader)
	{
		m_pLightShader->Shutdown();
		delete m_pLightShader;
		m_pLightShader = 0;
	}

	if (m_pPlane)
	{
		m_pPlane->shutDown();
		delete m_pPlane;
		m_pPlane = 0;
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

	m_pCameraEx->getViewMatrix(&viewMatrix);
	m_pD3D->GetWorldMatrix(worldMatrix);
	m_pD3D->GetProjectionMatrix(projectionMatrix);

	m_pAxisModel->Render(m_pD3D->GetDeviceContext());
	//用shader渲染
	result = m_pColorShader->render(m_pD3D->GetDeviceContext(), m_pAxisModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;

	//把模型顶点和索引顶点放入管线，准备渲染
	m_pModel->render(m_pD3D->GetDeviceContext());
	result = m_pColorShader->render(m_pD3D->GetDeviceContext(), m_pModel->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;


	D3DXVECTOR3 cameraPos;
	m_pCameraEx->getPosition(&cameraPos);

	LightBuffer light;
	MaterialBuffer material;
	CameraBuffer camera;
	Light mLight[NUM_LIGHTS];
	static float angle = 0;
	angle += dt;
	if (angle > 6.183)
		angle = 0;
	float x, z;
	x = 6 * cos(angle);
	z = 6 * sin(angle);
	camera.cameraPosition = D3DXVECTOR4(cameraPos.x, cameraPos.y, cameraPos.z, 1.0);
	material.Ka = D3DXVECTOR4(0.0, 0.0, 0.0, 1.0);
	material.Ke = D3DXVECTOR4(0.0, 0.0, 0.0, 1.0);

	mLight[0].SetLightPosition(x, 5.0, z, 1.0);
	mLight[1].SetLightPosition(-3.0, 4.0, -5.0, 1.0);
	mLight[1].SetLightSpotLight(0.3, 0.2, 0.0, 0.0);
	mLight[1].SetLightColor(0.5, 0.3, 0.2, 1.0);


	light.lightPosition[0] = mLight[0].GetPosition();
	light.lightColor[0] = mLight[0].GetLightColor();
	light.globalAmbient[0] = mLight[0].GetGlobalAmbient();
	light.attenuation[0] = mLight[0].GetLightAttenuation();
	light.lightDirection[0] = mLight[0].GetDirection();
	light.shininess[0] = mLight[0].GetShininess();
	light.spotlight[0] = mLight[0].GetSpotLight();

	light.lightPosition[1] = mLight[1].GetPosition();
	light.lightColor[1] = mLight[1].GetLightColor();
	light.globalAmbient[1] = mLight[1].GetGlobalAmbient();
	light.attenuation[1] = mLight[1].GetLightAttenuation();
	light.lightDirection[1] = mLight[1].GetDirection();
	light.shininess[1] = mLight[1].GetShininess();
	light.spotlight[1] = mLight[1].GetSpotLight();


	m_pCube->Render(m_pD3D->GetDeviceContext());
	result = m_pLightShader->Render(m_pD3D->GetDeviceContext(), m_pCube->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,light,material,camera);
	if (!result)
		return false;

	m_pPlane->render(m_pD3D->GetDeviceContext());
	result = m_pLightShader->Render(m_pD3D->GetDeviceContext(), m_pPlane->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, light, material, camera);
	if (!result)
		return false;

	m_pD3D->EndScene();

	return true;
}
