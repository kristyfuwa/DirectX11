#include "D3D.h"

D3D::D3D()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
}


D3D::~D3D()
{

}

//Initialize�����������D3D���õ����д��롣
bool D3D::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
	float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory*	factory;
	IDXGIAdapter*	adapter;
	IDXGIOutput*	adapterOutput;
	unsigned int numModes, i, numerator = 0, denominator = 1;
	size_t stringLength;
	DXGI_MODE_DESC*	displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC	swapChainDesc;
	D3D_FEATURE_LEVEL	featureLevel;
	ID3D11Texture2D*	backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// ���洹ֱͬ������
	m_vsync_enabled = vsync;

	// ����һ��DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	//�ýӿڹ�������һ�����Կ�������
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// �õ��������������.
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	//�õ��ʺ� DXGI_FORMAT_R8G8B8A8_UNORM ����ʾģʽ.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		HR(result);
		return false;
	}

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	//����������ʾģʽ���õ�ˢ������������ֵnumerator �� denominato
	for (i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//// �õ��Կ�����
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// �����Դ��С.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//�����Կ�����
	//wcstombs_s, wide charת��Ϊchar
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		HR(result);
		return false;
	}

	//�����Ļ�߶Ȼ��߿���Ϊ0����ᴴ����Ȼ���ʧ�ܣ� 
	//��������С��ʱ�򣬻���ִ���Ϊ0������� 
	if (screenWidth < 1)
		screenWidth = 1;
	if (screenHeight < 1)
		screenHeight = 1;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	//֡����Ĵ�С��Ӧ�ó��򴰿ڴ�С���.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// �󻺳��surface�ĸ�ʽΪDXGI_FORMAT_R8G8B8A8_UNORM.
	// surface��ÿ��������4���޷��ŵ�8bit[ӳ�䵽0-1]����ʾ��NORM��ʾ��һ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// ���ʹ�ô�ֱͬ�������ú󻺳��ˢ���ʡ�.
	//ˢ���ʾ���һ���ӰѺ󻺳���������Ļ�ϻ����Ĵ�����
	//���������ֱͬ����������ˢ���ʣ���fps�ǹ̶���
	if (vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// ���ú󻺳����;
	// ���ǵ���ȾĿ�껺��Ϊ�󻺳塣
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// �󻺳�����Ĵ��ھ��.
	swapChainDesc.OutputWindow = hwnd;
	// ��ʹ�ö��ز���
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	// ����ȫ�����ߴ���ģʽ.
	if (fullscreen)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	// �趨ɨ����ordering�Լ�����Ϊunspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// �󻺳����ݳ��ֵ���Ļ�󣬷���������
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	
	//�����ñ�־
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// ����feature levelΪD3D11
	// ����Կ���֧��D3D11,�����ܹ�ͨ���������������ʹ��D3D10,����9.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	////��debugģʽ�£�ʹ�ô������豸֧��debug.
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//��ʽһ������alt+enterȫ��
	{
		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, &featureLevel, 1,
			D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);

		//ע�⣺D3D11CreateDeviceAndSwapChain�����豸�ͽ�����ʱ���ڲ���ʹ���Լ��Ľӿڹ�����

		if (FAILED(result))
		{
			HR(result);
			return false;
		}

		//�õ� D3D11CreateDeviceAndSwapChain�����ڲ�ʹ�ýӿڹ��� 
		//����������ṩ���Լ���ѯ��һ���ӿڣ��ÿͻ�ȥѯ������������Ƿ�֧��ĳ���ӿڣ�
		//����QueryInterface���ж�����Ƿ�֧��ĳ���ض��Ľӿ�
		IDXGIDevice* pDXGIDevice;
		result = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
		if (FAILED(result))
		{
			HR(result);
			return false;
		}

		IDXGIAdapter* pDXGIAdapter;
		result = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter);
		if (FAILED(result))
		{
			HR(result);
			return false;
		}

		IDXGIFactory* pIDXGIFactory;
		result = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pIDXGIFactory);
		if (FAILED(result))
		{
			HR(result);
			return false;
		}
		//��ֹalt+enterȫ��
		//��������Ϣ�ķ�ʽȥ��ALT+ENTER��ȫ��/����ģʽֱ�ӵ��л�;
		//DXGIӦ��������Hook������Ϣ�������̣�����ALT + ENTER������
		//��ʵ���Զ����ڴ���ģʽ��ȫ��ģʽ�л���
		//����DXGI_MWA_NO_WINDOW_CHANGES����ָ��ȡ������������Ϣ���У���"ȡ��"�˹��ܡ�
		pIDXGIFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES);

	}

	/*
	//��ʽ��������alt+enterȫ�������Ƿֿ�����device��swapchain��
	{
		//�ֿ�����device�ͽ���������������dxgi�Զ���hook alt+enter
		//ע������㣺����õ�һ��������NULL,�ڶ�������ΪD3D_DRIVER_TYPE_HARDWARE������洴��swapchain 
		//��fail�����ֱ����adapter�����ı�ڶ��������������ʾ�����豸ʧ�ܣ�adapter��Ч�� 
		result = D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, createDeviceFlags, &featureLevel, 1,
			D3D11_SDK_VERSION, &m_device, NULL, &m_deviceContext);
		if (FAILED(result))
		{
			HR(result);
			return false;
		}
		//����������
		result = factory->CreateSwapChain(m_device, &swapChainDesc, &m_swapChain);
		if (FAILED(result))
		{
			HR(result);
			return false;
		}
		//��ֹDXGI������Ϣ���У���׽ALT+ENTER����ȫ���ʹ���ģʽ֮���л� 
		factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);

	}
	*/

	// �ͷ���ʾģʽ�б�
	delete[]displayModeList;
	displayModeList = 0;

	//�ͷ����������.
	adapterOutput->Release();
	adapterOutput = 0;

	//�ͷ�������
	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;
	




	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// �ú󻺳崴����ȾĿ����ͼ.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	//�ͷź󻺳壨���ü�����1��
	backBufferPtr->Release();
	backBufferPtr = 0;

	//��ʼ����Ȼ�������
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	//������Ȼ�������
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;//������Ȼ���Ϊ1
	depthBufferDesc.ArraySize = 1;//������Ȼ���Ϊ1��������������2�������и�����;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//������Ȼ���
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// ��ʼ�����ģ��״̬����.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	
	// �������ģ��״̬����.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;//D3D11_DEPTH_WRITE_MASK_ZERO��ֹд��Ȼ���
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// ����front face ����ʹ�õ�ģ�����ģʽ.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// ����back face����ʹ�õ�ģ�����ģʽ.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �������ģ��״̬��ʹ����Ч
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}
	// �������ģ��״̬.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// ��ʼ�����ģ����ͼ.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// �������ģ����ͼ����.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// �������ģ����ͼ.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// ����ȾĿ����ͼ����Ȼ��嵽��Ⱦ����.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// ���ù�դ��������ָ���������α���Ⱦ.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// ������դ��״̬
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	//���ù�դ��״̬��ʹ����Ч
	m_deviceContext->RSSetState(m_rasterState);

	// �����ӿڣ���ʾȫ���󻺳�����
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// �����ӿ�
	m_deviceContext->RSSetViewports(1, &viewport);
	// ����͸��ͶӰ����
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// ����͸��ͶӰ����.
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);
	
	//��ʼ��world����Ϊ��λ����.
	//�þ���ʵ�־ֲ����굽���������ת��
	D3DXMatrixIdentity(&m_worldMatrix);

	// ��������ͶӰ������Ҫ����ʵʩ2D��Ⱦ.

	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void D3D::Shutdown()
{
	// �ͷŽ�������Դǰ��������Ϊ����ģʽ��������ܻ�����쳣.
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	return;
}

void D3D::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];
	// ��������󻺳���ɫ.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	//����󻺳�.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3D::EndScene()
{
	//��Ⱦ��ɺ󣬰Ѻ󻺳�����present��ǰ����
	if (m_vsync_enabled)
	{
		// ������Ļˢ����.
		m_swapChain->Present(1, 0);
	}
	else
	{
		//�����ܿ��present�󻺳�.
		m_swapChain->Present(0, 0);
	}
	return;
}

ID3D11Device* D3D::GetDevice()
{
	return m_device;
}


ID3D11DeviceContext* D3D::GetDeviceContext()
{
	return m_deviceContext;
}

void D3D::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}


void D3D::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}


void D3D::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

void D3D::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

bool D3D::SetFillMode(D3D11_FILL_MODE fillmode)
{
	
	// ���ù�դ��������ָ���������α���Ⱦ.
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = fillmode;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	bool result;
	// ������դ��״̬
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	//���ù�դ��״̬��ʹ����Ч
	m_deviceContext->RSSetState(m_rasterState);
	return true;
}