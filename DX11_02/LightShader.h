#pragma once
#pragma once
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
#include "common.h"
using namespace std;

class LightShader
{
	//和shader中const buffer结构一致，主要用来给这些shader参数赋值。
private:
	struct MatrixBufferType
	{
		D3DXMATRIX	world;
		D3DXMATRIX  view;
		D3DXMATRIX  projection;
	};

	struct LightMaterialBufferType
	{
		D3DXVECTOR4 lightPosition;
		D3DXVECTOR4 lightColor;
		D3DXVECTOR4 globalAmbient;
		D3DXVECTOR4 cameraPosition;
		D3DXVECTOR4 Ke;
		D3DXVECTOR4 Ka;
		D3DXVECTOR4 Kd;
		D3DXVECTOR4 Ks;
		D3DXVECTOR3 lightDirection;
		float		shininess;
	};

public:
	LightShader();
	LightShader(const LightShader&);
	~LightShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4,
		D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, float);


private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4,
		D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, float);
	void RenderShader(ID3D11DeviceContext*, int);

	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pLayout;
	ID3D11Buffer*			m_pMatrixBuffer;
	ID3D11Buffer*			m_pLightMaterialBuffer; //材质光照参数buffer
};

