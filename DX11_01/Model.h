#pragma once
#include <d3d11.h>
#include <D3DX10math.h>

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};
public:
	Model();
	Model(const Model&);
	~Model();
	
	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int	 GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	//���㻺��Ͷ�����������

	ID3D11Buffer*	m_pVertexBuffer;
	ID3D11Buffer*	m_pIndexBuffer;
	int				m_iVertexCount;
	int				m_iIndexCount;
};

