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
	~ColorShader();

	bool initialize(ID3D11Device* device, HWND hwnd);
	void shutDown();
	bool render(ID3D11DeviceContext* context, int indexcount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);

private:
	bool initializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void shutDownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool setShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void renderShader(ID3D11DeviceContext*, int);

private:

	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pLayout;
	ID3D11Buffer*			m_pMatrixBuffer;

};

