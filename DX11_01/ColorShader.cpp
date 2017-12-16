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
}