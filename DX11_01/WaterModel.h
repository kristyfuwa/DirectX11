#pragma once
#include <d3d11.h>
#include <D3DX10math.h>
#include <vector>
#include "common.h"

using namespace std;
class WaterModel
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};
public:
	WaterModel();
	WaterModel(const WaterModel& other);
	~WaterModel();

	bool Initialize(ID3D11Device* device, int, int, float, float, float, float);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	bool Update(ID3D11DeviceContext* deviceContext,float dt);
	void Disturb(int, int, float);

private:
	bool InitializeBuffers(ID3D11Device* device, int, int, float, float, float, float);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11Buffer*			m_pIndexBuffer;
	int						m_iVertexCount;
	int						m_iIndexCount;

	//二维网格的行数、列数
	int						m_iNumRows;
	int						m_iNumCols;

	// 欲计算的模拟常量.
	float					m_fK1;
	float					m_fK2;
	float					m_fK3;

	float					m_fTimeStep;
	float					m_fSpatialStep;// spatial空间的

	D3DXVECTOR3*			m_vPreSolution;
	D3DXVECTOR3*			m_vCurrSolution;

	//顶点临时缓冲
	VertexType*				m_tVertices;
};

