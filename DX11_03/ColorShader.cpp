#include "ColorShader.h"




ColorShader::ColorShader()
{
	m_pLayout = 0;
	m_pMatrixBuffer = 0;
	m_pVertexShader = 0;
	m_pPixelShader = 0;
}

ColorShader::ColorShader(const ColorShader& other)
{

}

ColorShader::~ColorShader()
{

}

bool ColorShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	//��ʼ��vs,ps
	result = InitializeShader(device, hwnd, L"color.hlsl", L"color.hlsl");
	if (!result)
		return false;
	return true;

}

void ColorShader::Shutdown()
{
	ShutdownShader();
}

void ColorShader::ShutdownShader()
{
	// �ͷų�������
	if (m_pMatrixBuffer)
	{
		m_pMatrixBuffer->Release();
		m_pMatrixBuffer = 0;
	}
	//�ͷŶ��㲼��
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
}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	//�õ�������Ϣbufferָ��
	compileErrors = (char*)errorMessage->GetBufferPointer();

	//�õ�buffer�ĳ���
	bufferSize = errorMessage->GetBufferSize();

	//��һ���ļ�д������Ϣ
	fout.open("shader-error.txt");

	//���������Ϣ
	for (i = 0; i < bufferSize; ++i)
	{
		fout << compileErrors[i];
	}
	//�ͷŴ�����Ϣ���ر��ļ�������������ʾ

	fout.close();
	errorMessage->Release();
	errorMessage = 0;
	MessageBox(hwnd, L"error compiling shader,Check shader-error.tex for message.", shaderFilename, MB_OK);

}

bool ColorShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectMatrix)
{
	bool result;

	//����shader����
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectMatrix);
	if (!result)
	{
		HR(result);
		return false;
	}
		

	//��shader��Ⱦָ�����嶥��
	RenderShader(deviceContext, indexCount);
	return true;
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType*	dataPtr;
	unsigned int bufferNumber;

	//����shaderǰ��ȷ������ת�ã�����D3D11Ҫ��
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectMatrix, &projectMatrix);

	//�����������壬�Ա��ܹ�д��
	result = deviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	//�õ�const bufferָ��
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//����world��view��projection����
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectMatrix;

	//������������.
	deviceContext->Unmap(m_pMatrixBuffer, 0);

	//���ó��������λ��
	bufferNumber = 0;

	//�ø��º��ֵ���ó�������.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

	return true;

}

void ColorShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//�󶨶��㲼�֡�
	deviceContext->IASetInputLayout(m_pLayout);

	//������Ⱦʹ��VS��PS
	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	//��Ⱦ������
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

bool ColorShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, WCHAR* psFileName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
#endif


	//��ʼ��ָ��Ϊ��
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//����VS����
	result = D3DX11CompileFromFile(vsFileName, NULL, NULL, "ColorVertexShader", "vs_5_0", shaderFlags, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// ���vs����ʧ�ܣ����������Ϣ.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFileName);
		}
		// ���û���κδ�����Ϣ��������shader�ļ���ʧ.
		else
		{
			MessageBox(hwnd, vsFileName, L"Missing shader file", MB_OK);
		}
		HR(result);
		return false;
	}

	//����ps����
	result = D3DX11CompileFromFile(psFileName, NULL, NULL, "ColorPixelShader", "ps_5_0", shaderFlags, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// ���vs����ʧ�ܣ����������Ϣ.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFileName);
		}
		// ���û���κδ�����Ϣ��������shader�ļ���ʧ.
		else
		{
			MessageBox(hwnd, psFileName, L"Missing shader file", MB_OK);
		}
		HR(result);
		return false;
	}
	if(errorMessage)
		errorMessage->Release();


	//�ӻ��崴��vs shader,ps shader
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}
		

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}



	//�������ݲ��֣��Ա���shader��ʹ��
	//����Ҫ��Model�ж���ṹһ�£�
	polygonLayout[0] =
	{
		"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0,
	};
	polygonLayout[1]=
	{
		"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0,
	};

	//�õ�Layout��Ԫ�ص�����
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//�����������벼��
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pLayout);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}


	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//���ö�̬��������
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//// ����const bufferָ�룬�Ա����shader����.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}
	return true;
}