#include "ColorShader.h"

ColorShader::ColorShader()
{
	m_pLayout = 0;
	m_pMatrixBuffer = 0;
	m_pVertexShader = 0;
	m_pPixelShader = 0;
}

ColorShader::~ColorShader()
{

}

bool ColorShader::initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = initializeShader(device, hwnd, L"color.hlsl", L"color.hlsl");
	if (!result)
		return false;
}

bool ColorShader::initializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, WCHAR* psFileName)
{
	HRESULT result;
	ID3D10Blob*	errorMessage;
	ID3D10Blob* vsBlob;
	ID3D10Blob* psBlob;
	D3D11_INPUT_ELEMENT_DESC layout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
#endif

	errorMessage = nullptr;
	vsBlob = nullptr;
	psBlob = nullptr;

	result = D3DX11CompileFromFile(vsFileName, nullptr, nullptr, "ColorVertexShader", "vs_5_0",
		shaderFlags, 0, nullptr, &vsBlob, &errorMessage, nullptr);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, vsFileName);
		}
		else

		{
			MessageBox(hwnd, vsFileName, L"missing shader file", MB_OK);
		}
		HR(result);
		return false;
	}

	result = D3DX11CompileFromFile(psFileName, nullptr, nullptr, "ColorPixelShader", "ps_5_0",
		shaderFlags, 0, nullptr, &psBlob, &errorMessage, nullptr);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, psFileName);
		}
		else
		{
			MessageBox(hwnd, psFileName, L"miss shader file", MB_OK);
		}
		HR(result);
		return false;
	}

	result = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_pVertexShader);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}
	result = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	layout[0].SemanticName = "POSITION";
	layout[0].SemanticIndex = 0;
	layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[0].InputSlot = 0;
	layout[0].AlignedByteOffset = 0;
	layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[0].InstanceDataStepRate = 0;
		   
	layout[1].SemanticName = "COLOR";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;//12�Ƕ����ֽ�λ�ƣ�ǰһ��Ԫ����D3DXVECTOR3����С3 * sizeof(D3DXVECTOR3) = 12��Ϊ�˷������D3D11_APPEND_ALIGNED_ELEMENT���Դ���12  
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	numElements = sizeof(layout) / sizeof(layout[0]);
	result = device->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_pLayout);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	//release buffer
	vsBlob->Release();
	vsBlob = 0;
	psBlob->Release();
	psBlob = 0;
	
	//set dynamic matrix desc
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, nullptr, &m_pMatrixBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	return true;
}

void ColorShader::shutDownShader()
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

void ColorShader::outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileError;
	unsigned long bufferSize, i;
	ofstream fout;

	compileError = (char*)errorMessage->GetBufferPointer();
	bufferSize = errorMessage->GetBufferSize();

	fout.open("shader-error.txt");
	for (i = 0; i < bufferSize; ++i)
	{
		fout << compileError[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = 0;

	MessageBox(hwnd, L"Error compiling shader,check shader-error.txt for message", shaderFilename, MB_OK);
}

bool ColorShader::setShaderParameters(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// ����shaderǰ��ȷ������ת�ã�����D3D11��Ҫ��.
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&proj, &proj);

	//  �����������壬�Ա��ܹ�д��.
	result = context->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}
	// �õ�const bufferָ��.
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	// ����world,view�Լ�projection����.
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = proj;

	//// ������������.
	context->Unmap(m_pMatrixBuffer, 0);

	// ���ó�������λ��.
	bufferNumber = 0;
	context->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);
	return true;

}

void ColorShader::renderShader(ID3D11DeviceContext* context, int indexcount)
{
	context->IASetInputLayout(m_pLayout);

	context->VSSetShader(m_pVertexShader, nullptr, 0);
	context->PSSetShader(m_pPixelShader, nullptr, 0);

	context->DrawIndexed(indexcount, 0, 0);
}

void ColorShader::shutDown()
{
	shutDownShader();
}

bool ColorShader::render(ID3D11DeviceContext* context, int indexcount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj)
{
	bool result;

	result = setShaderParameters(context, world, view, proj);
	if (!result)
	{
		HR(result);
		return false;
	}

	renderShader(context, indexcount);
	return true;
}
