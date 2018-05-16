#pragma once
#include <d3d11.h>
#include<D3DX11tex.h>
class Texture
{
public:
	Texture();
	~Texture();

	bool Initialize(ID3D11Device* device, WCHAR* file);
	void ShutDown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView*			m_pResourceView;
};

