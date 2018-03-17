#include "Light.h"
#include "LightShader.h"

LightShader::LightShader(void)
{
	m_pVertexShader = 0;
	m_pPixelShader = 0;
	m_pLayout = 0;
	m_pMatrixBuffer = 0;
	m_pLightMaterialBuffer = 0;

}

LightShader::LightShader(const LightShader & others)
{

}

LightShader::~LightShader(void)
{
}

bool LightShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// 初始化vs和ps.
	result = InitializeShader(device, hwnd, L"light.hlsl", L"light.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

void LightShader::Shutdown()
{
	// 释放shader资源.
	ShutdownShader();

	return;
}

bool LightShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix, D3DXVECTOR4 cameraPosition)
{
	bool result;

	// 设置shader参数.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix,cameraPosition);
	if (!result)
	{
		HR(result);
		return false;
	}

	// 用指定shader渲染顶点缓冲.
	RenderShader(deviceContext, indexCount);

	return true;
}

bool LightShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightmaterialBufferDesc;


	// 初始化指针为空.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// 编译vs代码.
	//增加D3D10_SHADER_DEBUG，是为了在perfstuido中查看hlsl代码
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		HR(result);
		// 如果vs编译失败，输出错误消息
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// 如果没有任何错误消息，可能是shader文件丢失.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	//  编译ps代码.

	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// 如果ps编译失败，输出错误信息.
		HR(result);
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// 如果没有任何错误消息，可能是shader文件丢失.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// 从缓冲创建vs shader.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// 从缓冲中创建pvs shader.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// 设置数据布局，以便在shader中使用.
	// 定义要和ModelClass中的顶点结构一致.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// 得到layout中的元素数量
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// 创建顶点输入布局.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_pLayout);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	//释放顶点和像素缓冲.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	// 设置动态矩阵描述.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	lightmaterialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightmaterialBufferDesc.ByteWidth = sizeof(LightMaterialBufferType);
	lightmaterialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightmaterialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightmaterialBufferDesc.MiscFlags = 0;
	lightmaterialBufferDesc.StructureByteStride = 0;

	// 创建const buffer指针，以便访问shader常量.
	result = device->CreateBuffer(&lightmaterialBufferDesc, NULL, &m_pLightMaterialBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	return true;
}

void LightShader::ShutdownShader()
{
	// 释放光照材质缓冲.
	if (m_pLightMaterialBuffer)
	{
		m_pLightMaterialBuffer->Release();
		m_pLightMaterialBuffer = 0;
	}

	// 释放常量缓冲
	if (m_pMatrixBuffer)
	{
		m_pMatrixBuffer->Release();
		m_pMatrixBuffer = 0;
	}

	//释放顶点布局.
	if (m_pLayout)
	{
		m_pLayout->Release();
		m_pLayout = 0;
	}

	// 释放ps .
	if (m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = 0;
	}

	// 释放vs
	if (m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = 0;
	}
	return;
}

void LightShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// 得到错误信息buffer指针.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// 得到buffer的长度.
	bufferSize = errorMessage->GetBufferSize();

	// 打开一个文件写错误信息.
	fout.open("shader-error.txt");

	// 输出错误信心.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// 关闭文件
	fout.close();

	// 释放错误消息.
	errorMessage->Release();
	errorMessage = 0;

	//弹出错误提示.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool LightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix,D3DXVECTOR4 cameraPosition)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightMaterialBufferType* dataPtr2;

	// 传入shader前，确保矩阵转置，这是D3D11的要求.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	//  锁定常量缓冲，以便能够写入.
	result = deviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// 得到const buffer指针.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// 设置world,view以及projection矩阵.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// 解锁常量缓冲.
	deviceContext->Unmap(m_pMatrixBuffer, 0);

	// 设置常量缓冲位置.
	bufferNumber = 0;

	// 用更新后的值设置常量缓冲.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);


	// 锁定光照材质缓冲.
	result = deviceContext->Map(m_pLightMaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//得到常量缓冲数据指针
	dataPtr2 = (LightMaterialBufferType*)mappedResource.pData;

	D3DXVECTOR4 Ke[NUM_LIGHTS];
	D3DXVECTOR4 Ka[NUM_LIGHTS];
	D3DXVECTOR4 Kd[NUM_LIGHTS];
	D3DXVECTOR4 Ks[NUM_LIGHTS];

	Ke[0] = D3DXVECTOR4(0.2, 0.2, 0.0, 1.0);
	Ka[0] = D3DXVECTOR4(0.0, 0.0, 0.0, 1.0);
	Kd[0] = D3DXVECTOR4(1.0, 1.0, 1.0, 1.0);
	Ks[0] = D3DXVECTOR4(0.2, 0.2, 0.2, 1.0);

	Ke[1] = D3DXVECTOR4(0.0, 0.0, 0.0, 1.0);
	Ka[1] = D3DXVECTOR4(0.0, 0.0, 0.0, 1.0);
	Kd[1] = D3DXVECTOR4(0.5, 0.5, 0.5, 1.0);
	Ks[1] = D3DXVECTOR4(0.1, 0.1, 0.1, 1.0);

	Light light[NUM_LIGHTS];
	light[0].SetLightPosition(5.0, 5.0, -3.0, 1.0);
	light[1].SetLightPosition(-8.0, -4.0, 5.0, 1.0);
	light[0].SetLightColor(1.0, 1.0, 1.0, 1.0);
	light[1].SetLightColor(1.0, 1.0, 1.0, 1.0);

	for (int i = 0; i < NUM_LIGHTS; ++i)
	{
		// 常量缓冲赋值.
		dataPtr2->lightPosition[i] = light[i].GetPosition();
		dataPtr2->lightColor[i] = light[i].GetLightColor();
		dataPtr2->globalAmbient[i] = light[i].GetGlobalAmbient();
		dataPtr2->Ka[i] = Ka[i];
		dataPtr2->Ke[i] = Ke[i];
		dataPtr2->Ks[i] = Ks[i];
		dataPtr2->Kd[i] = Kd[i];
		dataPtr2->cameraPosition = cameraPosition;
		dataPtr2->lightDirection[i] = light[i].GetDirection();
		dataPtr2->shininess[i] = light[i].GetShininess();
	}


	// 解锁常量缓冲
	deviceContext->Unmap(m_pLightMaterialBuffer, 0);

	//// 设置缓冲索引为1，因为这是vs中的第二个常量缓冲，第一个为矩阵.
	//bufferNumber = 1;

	//设置缓冲索引为0，因为这是ps中的第一个常量缓冲.
	bufferNumber = 0;

	// 设置光照材质常量缓冲.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pLightMaterialBuffer);

	return true;
}

void LightShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// 绑定顶点布局.
	deviceContext->IASetInputLayout(m_pLayout);

	// 设置渲染使用vs和ps.
	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	// 渲染三角形
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
