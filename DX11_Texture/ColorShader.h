#pragma once
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
#include "common.h"
using namespace std;

class ColorShader
{
	//和shader中const buffer结构一致，主要用来给这些shader参数赋值。
private:
	struct MatrixBufferType
	{
		D3DXMATRIX	world;
		D3DXMATRIX  view;
		D3DXMATRIX  projection;
	};

public:
	ColorShader();
	ColorShader(const ColorShader&);
	~ColorShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,ID3D11ShaderResourceView*);


private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pLayout;
	ID3D11Buffer*			m_pMatrixBuffer;

	//采样状态
	ID3D11SamplerState*	m_pSampleState;

};

