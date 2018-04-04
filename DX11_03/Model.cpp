#include "Model.h"



Model::Model()
{
	m_iIndexCount = 0;
	m_iVertexCount = 0;
}

Model::~Model()
{

}

bool Model::initialize(ID3D11Device* device)
{
	bool result;
	result = initializeBuffers(device);
	if (!result)
		return false;
	return true;
}

void Model::shutDown()
{
	shutDownBuffers();
}

void Model::render(ID3D11DeviceContext* context)
{
	renderBuffers(context);
}

int Model::getIndexCount()
{
	return m_iIndexCount;
}

void Model::shutDownBuffers()
{
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = 0;
	}
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = 0;
	}
}


bool Model::initializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_iVertexCount = 8;
	m_iIndexCount = 36;

	vertices = new VertexType[m_iVertexCount];
	if (!vertices)
		return false;
	indices = new unsigned long[m_iIndexCount];
	if (!indices)
		return false;

	vertices[0].position = D3DXVECTOR3(4.0f, -1.0f, -1.0f);
	vertices[0].color = WHITE;

	vertices[1].position = D3DXVECTOR3(4.0f, 1.0f, -1.0f);
	vertices[1].color = BLACK;

	vertices[2].position = D3DXVECTOR3(6.0f, 1.0f, -1.0f);
	vertices[2].color = RED;

	vertices[3].position = D3DXVECTOR3(6.0f, -1.0f, -1.0f);
	vertices[3].color = GREEN;

	vertices[4].position = D3DXVECTOR3(4.0f, -1.0f, 1.0f);
	vertices[4].color = BLUE;

	vertices[5].position = D3DXVECTOR3(4.0f, 1.0f, 1.0f);
	vertices[5].color = YELLOW;

	vertices[6].position = D3DXVECTOR3(6.0f, 1.0f, 1.0f);
	vertices[6].color = CYAN;

	vertices[7].position = D3DXVECTOR3(6.0f, -1.0f, 1.0f);
	vertices[7].color = MAGENTA;

	// 设置索引缓冲数据.
	indices[0] = 0;  // 前面
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	indices[6] = 4;  // 后面
	indices[7] = 6;
	indices[8] = 5;
	indices[9] = 4;
	indices[10] = 7;
	indices[11] = 6;

	indices[12] = 4;  // 左面
	indices[13] = 5;
	indices[14] = 1;
	indices[15] = 4;
	indices[16] = 1;
	indices[17] = 0;

	indices[18] = 3;  //右面
	indices[19] = 2;
	indices[20] = 6;
	indices[21] = 3;
	indices[22] = 6;
	indices[23] = 7;

	indices[24] = 1;  // 上面
	indices[25] = 5;
	indices[26] = 6;
	indices[27] = 1;
	indices[28] = 6;
	indices[29] = 2;

	indices[30] = 4;  // 下面
	indices[31] = 0;
	indices[32] = 3;
	indices[33] = 4;
	indices[34] = 3;
	indices[35] = 7;

	ZeroMemory(&vertexBufferDesc,sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;//支持GPU读写，不支持CPU读写
	vertexBufferDesc.ByteWidth = m_iVertexCount * sizeof(VertexType);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	ZeroMemory(&vertexData, sizeof(vertexData));
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
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_iIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 指向存临时索引缓冲.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 创建索引缓冲.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// 释放临时缓冲.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
	
}

void Model::renderBuffers(ID3D11DeviceContext* context)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}