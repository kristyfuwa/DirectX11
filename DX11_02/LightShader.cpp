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

	// ��ʼ��vs��ps.
	result = InitializeShader(device, hwnd, L"light.hlsl", L"light.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

void LightShader::Shutdown()
{
	// �ͷ�shader��Դ.
	ShutdownShader();

	return;
}

bool LightShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix, D3DXVECTOR4 cameraPosition)
{
	bool result;

	// ����shader����.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix,cameraPosition);
	if (!result)
	{
		HR(result);
		return false;
	}

	// ��ָ��shader��Ⱦ���㻺��.
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


	// ��ʼ��ָ��Ϊ��.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// ����vs����.
	//����D3D10_SHADER_DEBUG����Ϊ����perfstuido�в鿴hlsl����
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		HR(result);
		// ���vs����ʧ�ܣ����������Ϣ
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// ���û���κδ�����Ϣ��������shader�ļ���ʧ.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	//  ����ps����.

	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// ���ps����ʧ�ܣ����������Ϣ.
		HR(result);
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// ���û���κδ�����Ϣ��������shader�ļ���ʧ.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// �ӻ��崴��vs shader.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// �ӻ����д���pvs shader.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// �������ݲ��֣��Ա���shader��ʹ��.
	// ����Ҫ��ModelClass�еĶ���ṹһ��.
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

	// �õ�layout�е�Ԫ������
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// �����������벼��.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_pLayout);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	//�ͷŶ�������ػ���.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	// ���ö�̬��������.
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

	// ����const bufferָ�룬�Ա����shader����.
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
	// �ͷŹ��ղ��ʻ���.
	if (m_pLightMaterialBuffer)
	{
		m_pLightMaterialBuffer->Release();
		m_pLightMaterialBuffer = 0;
	}

	// �ͷų�������
	if (m_pMatrixBuffer)
	{
		m_pMatrixBuffer->Release();
		m_pMatrixBuffer = 0;
	}

	//�ͷŶ��㲼��.
	if (m_pLayout)
	{
		m_pLayout->Release();
		m_pLayout = 0;
	}

	// �ͷ�ps .
	if (m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = 0;
	}

	// �ͷ�vs
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


	// �õ�������Ϣbufferָ��.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// �õ�buffer�ĳ���.
	bufferSize = errorMessage->GetBufferSize();

	// ��һ���ļ�д������Ϣ.
	fout.open("shader-error.txt");

	// �����������.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// �ر��ļ�
	fout.close();

	// �ͷŴ�����Ϣ.
	errorMessage->Release();
	errorMessage = 0;

	//����������ʾ.
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

	// ����shaderǰ��ȷ������ת�ã�����D3D11��Ҫ��.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	//  �����������壬�Ա��ܹ�д��.
	result = deviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// �õ�const bufferָ��.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// ����world,view�Լ�projection����.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// ������������.
	deviceContext->Unmap(m_pMatrixBuffer, 0);

	// ���ó�������λ��.
	bufferNumber = 0;

	// �ø��º��ֵ���ó�������.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);


	// �������ղ��ʻ���.
	result = deviceContext->Map(m_pLightMaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//�õ�������������ָ��
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
		// �������帳ֵ.
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


	// ������������
	deviceContext->Unmap(m_pLightMaterialBuffer, 0);

	//// ���û�������Ϊ1����Ϊ����vs�еĵڶ����������壬��һ��Ϊ����.
	//bufferNumber = 1;

	//���û�������Ϊ0����Ϊ����ps�еĵ�һ����������.
	bufferNumber = 0;

	// ���ù��ղ��ʳ�������.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pLightMaterialBuffer);

	return true;
}

void LightShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// �󶨶��㲼��.
	deviceContext->IASetInputLayout(m_pLayout);

	// ������Ⱦʹ��vs��ps.
	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	// ��Ⱦ������
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
