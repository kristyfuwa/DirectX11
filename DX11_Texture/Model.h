#pragma once
#include <d3d11.h>
#include <D3DX10math.h>
#include "Texture.h"
#include "common.h"

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	Model();
	Model(const Model&);
	~Model();
	
	bool Initialize(ID3D11Device*,WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int	 GetIndexCount();
	
	ID3D11ShaderResourceView* GetTexture();
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();
	//¶¥µã»º³åºÍ¶¥µãË÷Òý»º³å

	ID3D11Buffer*	m_pVertexBuffer;
	ID3D11Buffer*	m_pIndexBuffer;
	int				m_iVertexCount;
	int				m_iIndexCount;

	Texture*		m_pTexture;
};

