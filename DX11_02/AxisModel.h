#pragma once
#include <d3d11.h>
#include <D3DX10math.h>
#include "common.h"
class AxisModel
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};
public:
	AxisModel();
	AxisModel(const AxisModel& other);
	~AxisModel();

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	void Render(ID3D11DeviceContext* context);

	int GetIndexCount();


private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutDownBuffers();
	void RenderBuffers(ID3D11DeviceContext* device);

	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11Buffer*			m_pIndexBuffer;
	int						m_iVertexCount;
	int						m_iIndexCount;
};

