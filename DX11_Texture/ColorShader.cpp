#include "ColorShader.h"




ColorShader::ColorShader()
{
	m_pLayout = 0;
	m_pMatrixBuffer = 0;
	m_pVertexShader = 0;
	m_pPixelShader = 0;
	m_pSampleState = 0;
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
	//初始化vs,ps
	result = InitializeShader(device, hwnd, L"color.vs", L"color.ps");
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
	// 释放常量缓冲
	if (m_pMatrixBuffer)
	{
		m_pMatrixBuffer->Release();
		m_pMatrixBuffer = 0;
	}
	//释放顶点布局
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

	SafeRelease(m_pSampleState);
}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	//得到错误信息buffer指针
	compileErrors = (char*)errorMessage->GetBufferPointer();

	//得到buffer的长度
	bufferSize = errorMessage->GetBufferSize();

	//打开一个文件写错误信息
	fout.open("shader-error.txt");

	//输出错误信息
	for (i = 0; i < bufferSize; ++i)
	{
		fout << compileErrors[i];
	}
	//释放错误信息，关闭文件，弹出错误提示

	fout.close();
	errorMessage->Release();
	errorMessage = 0;
	MessageBox(hwnd, L"error compiling shader,Check shader-error.tex for message.", shaderFilename, MB_OK);

}

bool ColorShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectMatrix, ID3D11ShaderResourceView* resourceView)
{
	bool result;

	//设置shader参数
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectMatrix,resourceView);
	if (!result)
	{
		HR(result);
		return false;
	}
		

	//用shader渲染指定缓冲顶点
	RenderShader(deviceContext, indexCount);
	return true;
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectMatrix, ID3D11ShaderResourceView* resourceView)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType*	dataPtr;
	unsigned int bufferNumber;

	//传入shader前，确保矩阵转置，这是D3D11要求
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectMatrix, &projectMatrix);

	//锁定常量缓冲，以便能够写入
	result = deviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	//得到const buffer指针
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//设置world，view，projection矩阵
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectMatrix;

	//解锁常量缓冲.
	deviceContext->Unmap(m_pMatrixBuffer, 0);

	//设置常量缓冲的位置
	bufferNumber = 0;

	//用更新后的值设置常量缓冲.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);
	deviceContext->PSSetShaderResources(0, 1, &resourceView);

	return true;

}

void ColorShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//绑定顶点布局。
	deviceContext->IASetInputLayout(m_pLayout);

	//设置渲染使用VS和PS
	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	deviceContext->PSSetSamplers(0, 1, &m_pSampleState);

	//渲染三角形
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
	D3D11_SAMPLER_DESC samplerDesc;
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
#endif


	//初始化指针为空
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//编译VS代码
	result = D3DX11CompileFromFile(vsFileName, NULL, NULL, "ColorVertexShader", "vs_5_0", shaderFlags, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// 如果vs编译失败，输出错误消息.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFileName);
		}
		// 如果没有任何错误消息，可能是shader文件丢失.
		else
		{
			MessageBox(hwnd, vsFileName, L"Missing shader file", MB_OK);
		}
		HR(result);
		return false;
	}

	//编译ps代码
	result = D3DX11CompileFromFile(psFileName, NULL, NULL, "ColorPixelShader", "ps_5_0", shaderFlags, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// 如果vs编译失败，输出错误消息.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFileName);
		}
		// 如果没有任何错误消息，可能是shader文件丢失.
		else
		{
			MessageBox(hwnd, psFileName, L"Missing shader file", MB_OK);
		}
		HR(result);
		return false;
	}
	if(errorMessage)
		errorMessage->Release();


	//从缓冲创建vs shader,ps shader
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



	//设置数据布局，以便在shader中使用
	//定义要和Model中顶点结构一致；
	polygonLayout[0] =
	{
		"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0,
	};
	polygonLayout[1]=
	{
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0,
	};

	//得到Layout中元素的数量
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//创建顶点输入布局
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

	//设置动态矩阵描述
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//// 创建const buffer指针，以便访问shader常量.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;//在超出[0,1]范围的坐标上，其作用MIRROR和CLAMP的结合效果相似。取纹理坐标的绝对值，因此，是围绕原点进行镜像的，然后再截取其最大值；
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;//mip map level 指定使用第 2 级别的 MIP 并且偏移量为 3，则实际上 MIP 使用的是第 5 级别
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 1.0;
	samplerDesc.MinLOD = 0; //where 0 is the largest and most detailed mipmap level and any level higher than that is less detailed.
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;//如果最大值指定为 1，那么级别为 0 的 MIP纹理就不会被访问到(注意级别为 0 的 MIP 就是分辨率最高的纹理)。


	result = device->CreateSamplerState(&samplerDesc, &m_pSampleState);
	if (FAILED(result))
		return false;
	return true;
}