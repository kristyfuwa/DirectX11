#include "PlaneModel.h"



PlaneModel::PlaneModel()
{
	m_pIndexBuffer = 0;
	m_pVertexBuffer = 0;
	m_iIndexCount = 0;
	m_iVertexCount = 0;
}


PlaneModel::PlaneModel(const PlaneModel& other)
{

}

PlaneModel::~PlaneModel()
{
}

bool PlaneModel::Initialize(ID3D11Device* device)
{
	bool result;
	//int* p = (int*)malloc(sizeof(int) * 2);
	result = InitializeBuffers(device);
	if (!result)
		return false;
	return true;
}

void PlaneModel::Shutdown()
{
	ShutDownBuffers();
	return;
}

void PlaneModel::Render(ID3D11DeviceContext* context)
{
	RenderBuffers(context);
}

int PlaneModel::GetIndexCount()
{
	return m_iIndexCount;
}

bool PlaneModel::InitializeBuffers(ID3D11Device* device)
{
	HRESULT result;
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	//首先，我们创建2个临时缓冲存放顶点和索引数据，以便后面使用。. 
	m_iVertexCount = 4;
	m_iIndexCount = 6;

	vertices = new VertexType[m_iVertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[m_iIndexCount];
	if (!indices)
		return false;

	//创建顺时针方向的三角形，左手规则
	// 设置顶点数据.
	vertices[0].position = D3DXVECTOR3(-50.0f, -3.0f, -50.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f,1.0f,0.0f);

	vertices[1].position = D3DXVECTOR3(-50.0f, -3.0f, 50.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(50.0f, -3.0f, 50.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	vertices[3].position = D3DXVECTOR3(50.0f, -3.0f, -50.0f);
	vertices[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 设置索引缓冲数据.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;


	//设置顶点缓冲描述
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)*m_iVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	//vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 指向保存顶点数据的临时缓冲.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;


	// 创建顶点缓冲
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
	if (FAILED(result))
		return false;

	//设置索引缓冲
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)*m_iIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 指向存临时索引缓冲.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//// 创建索引缓冲.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	if (FAILED(result))
		return false;

	// 释放临时缓冲.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;

}

void PlaneModel::ShutDownBuffers()
{
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}

}

void PlaneModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//设置顶点缓冲跨度和偏移
	stride = sizeof(VertexType);
	offset = 0;

	//在input assemberl阶段绑定顶点缓冲，以便能够被渲染
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//在input assemberl阶段绑定索引缓冲，以便能够被渲染
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 设置体元语义，渲染线段，画出坐标轴
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}
