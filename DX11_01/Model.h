#pragma once
#include <d3d11.h>
#include <D3DX10math.h>

//定义一些常用颜色
const D3DXVECTOR4 WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXVECTOR4 BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXVECTOR4 RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXVECTOR4 GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXVECTOR4 BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const D3DXVECTOR4 YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const D3DXVECTOR4 CYAN(0.0f, 1.0f, 1.0f, 1.0f); //蓝绿色
const D3DXVECTOR4 MAGENTA(1.0f, 0.0f, 1.0f, 1.0f); //洋红色
const D3DXVECTOR4 BEACH_SAND(1.0f, 0.96f, 0.62f, 1.0f);
const D3DXVECTOR4 LIGHT_YELLOW_GREEN(0.48f, 0.77f, 0.46f, 1.0f);
const D3DXVECTOR4 DARK_YELLOW_GREEN(0.1f, 0.48f, 0.19f, 1.0f);
const D3DXVECTOR4 DARKBROWN(0.45f, 0.39f, 0.34f, 1.0f);

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

	//顶点缓冲和顶点索引缓冲

	ID3D11Buffer*	m_pVertexBuffer;
	ID3D11Buffer*	m_pIndexBuffer;
	int				m_iVertexCount;
	int				m_iIndexCount;
};

