#include "Texture.h"
#include "common.h"
#include <d3d11.h>
#include <d3dx11tex.h>

Texture::Texture()
{
}


Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device* device, WCHAR* file)
{
	HRESULT  result;
	result = D3DX11CreateShaderResourceViewFromFile(device, file, NULL, NULL, &m_pResourceView, NULL);
	if (FAILED(result))
		return false;
	return true;
}

void Texture::ShutDown()
{
	if (m_pResourceView)
	{
		m_pResourceView->Release();
		m_pResourceView = 0;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_pResourceView;
}
