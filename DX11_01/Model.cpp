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

bool Model::Initialize(ID3D11Device* device)
{
	bool result;

	//初始化顶点缓冲和顶点索引缓冲
	result = InitializeBuffers(device);
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

bool Model::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//创建2个临时缓冲存放顶点和索引缓冲

	m_iVertexCount = 3;

	m_iIndexCount = 3;

	vertices = new VertexType[m_iVertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_iIndexCount];
	if (!indices)
	{
		return false;
	}

	//创建顺时针方向的三角形，左手规则;
	//vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	//vertices[0].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	//vertices[1].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	//vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[2].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);

	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // 左下
	vertices[0].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // 中上.
	vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // 底右
	vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);


	//设置索引缓冲数据;
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

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
		return false;

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
		return false;

	delete[]vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;

}