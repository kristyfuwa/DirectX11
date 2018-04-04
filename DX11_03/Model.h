#pragma once
#include <d3d11.h>
#include <D3DX10math.h>
#include "common.h"

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3	position;
		D3DXVECTOR4 color;
	};
public:

	Model();
	~Model();

	bool initialize(ID3D11Device* device);
	void shutDown();
	void render(ID3D11DeviceContext* context);

	int getIndexCount();
private:
	bool initializeBuffers(ID3D11Device* device);
	void shutDownBuffers();
	void renderBuffers(ID3D11DeviceContext* context);

	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11Buffer*			m_pIndexBuffer;
	int						m_iVertexCount;
	int						m_iIndexCount;
};

