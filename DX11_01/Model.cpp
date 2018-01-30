#include "Model.h"



Model::Model()
{
	m_iIndexCount = 0;
	m_iVertexCount = 0;
}

Model::Model(const Model& others)
{

}

Model::~Model()
{

}

bool Model::Initialize(ID3D11Device* device,int m,int n,float dx)
{
	bool result;

	//初始化顶点缓冲和顶点索引缓冲
	result = InitializeBuffers(device,m,n,dx);
	if (!result)
		return false;

	return true;
}

void Model::Shutdown()
{
	ShutdownBuffers();

	return;
}

void Model::ShutdownBuffers()
{
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = 0;
	}
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = 0;
	}
	return;
}

int Model::GetIndexCount()
{
	return m_iIndexCount;
}

float Model::GetHeight(float x, float z) const
{
	return 0.1*(z*sinf(0.1*x) + x*cosf(0.1*z));
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
	return;
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// 设置顶点缓冲跨度和偏移.
	stride = sizeof(VertexType);
	offset = 0;

	//在input assemberl阶段绑定顶点缓冲，以便能够被渲染
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	////在input assemberl阶段绑定索引缓冲，以便能够被渲染
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 设置体元语义，渲染三角形列表
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Model::InitializeBuffers(ID3D11Device* device,int m,int n,float dx)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//创建2个临时缓冲存放顶点和索引缓冲

	m_iVertexCount = m*n;
	m_iIndexCount = (m - 1)*(n - 1) * 2 * 3;

	vertices = new VertexType[m_iVertexCount];
	if (!vertices)
	{
		return false;
	}
	float halfWidth = (n - 1)*dx*0.5f;
	float halfDepth = (m - 1)*dx*0.5f;

	for(int i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dx;
		for (int j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;
			// 计算得到y值. 
			float y = GetHeight(x, z);
			vertices[i*n + j].position = D3DXVECTOR3(x, y, z);
			//根据高度来定义颜色
			if (y < -10.0f)
				vertices[i*n + j].color = BEACH_SAND;
			else if (y < 5.0f)
				vertices[i*n + j].color = LIGHT_YELLOW_GREEN;
			else if (y < 20.0f)
				vertices[i*n + j].color = DARKBROWN;
			else
				vertices[i*n + j].color = WHITE;
		}
	}

	indices = new unsigned long[m_iIndexCount];
	if (!indices)
	{
		return false;
	}
	//迭代每个grid，计算得出索引
	
	int k = 0;
	for (int i = 0; i < m-1; ++i)
	{
		for (int j = 0; j < n-1; ++j)
		{
			indices[k] = i*n + j;
			indices[k + 1] = i*n + j + 1;
			indices[k + 2] = (i + 1)*n + j;
			
			indices[k + 3] = (i + 1)*n + j;
			indices[k + 4] = i*n + j + 1;
			indices[k + 5] = (i + 1)*n + j + 1;
			k += 6;
		}
	}

	////创建顺时针方向的三角形，左手规则;
	////vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	////vertices[0].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	////vertices[1].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	////vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	////vertices[2].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	////vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);

	//vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // 左下
	//vertices[0].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	//vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // 中上.
	//vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // 底右
	//vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);


	////设置索引缓冲数据;
	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 2;

	//创建顺时针方向的三角形，左手规则;
	// 设置顶点数据.
		//vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		//vertices[0].color = WHITE;

		//vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
		//vertices[1].color = BLACK;

		//vertices[2].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
		//vertices[2].color = RED;

		//vertices[3].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
		//vertices[3].color = GREEN;

		//vertices[4].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
		//vertices[4].color = BLUE;

		//vertices[5].position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
		//vertices[5].color = YELLOW;

		//vertices[6].position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		//vertices[6].color = CYAN;

		//vertices[7].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
		//vertices[7].color = MAGENTA;

		//// 设置索引缓冲数据.

		//indices[0] = 0;  // 前面
		//indices[1] = 1;
		//indices[2] = 2;
		//indices[3] = 0;
		//indices[4] = 2;
		//indices[5] = 3;
		//indices[6] = 4;  // 后面
		//indices[7] = 6;
		//indices[8] = 5;
		//indices[9] = 4;
		//indices[10] = 7;
		//indices[11] = 6;
		//indices[12] = 4;  // 左面
		//indices[13] = 5;
		//indices[14] = 1;
		//indices[15] = 4;
		//indices[16] = 1;
		//indices[17] = 0;
		//indices[18] = 3;  //右面
		//indices[19] = 2;
		//indices[20] = 6;
		//indices[21] = 3;
		//indices[22] = 6;
		//indices[23] = 7;
		//indices[24] = 1;  // 上面
		//indices[25] = 5;
		//indices[26] = 6;
		//indices[27] = 1;
		//indices[28] = 6;
		//indices[29] = 2;
		//indices[30] = 4; // 下面
		//indices[31] = 0;
		//indices[32] = 3;
		//indices[33] = 4;
		//indices[34] = 3;
		//indices[35] = 7;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_iVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 指向保存顶点数据的临时缓冲.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}
		
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)*m_iIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}
		

	delete[]vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;

}