#pragma once
#include <d3d11.h>
#include <D3DX10math.h>
#include "common.h"

class CubeModel
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
	};
public:
	CubeModel();
	CubeModel(const CubeModel&);
	~CubeModel();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int	 GetIndexCount();
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	//¶¥µã»º³åºÍ¶¥µãË÷Òý»º³å

	ID3D11Buffer*	m_pVertexBuffer;
	ID3D11Buffer*	m_pIndexBuffer;
	int				m_iVertexCount;
	int				m_iIndexCount;
};

