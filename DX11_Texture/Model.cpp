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

bool Model::Initialize(ID3D11Device* device,WCHAR* textureFile)
{
	bool result;

	//初始化顶点缓冲和顶点索引缓冲
	result = InitializeBuffers(device);
	if (!result)
		return false;

	result = LoadTexture(device, textureFile);
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

ID3D11ShaderResourceView* Model::GetTexture()
{
	return m_pTexture->GetTexture();
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

bool Model::LoadTexture(ID3D11Device* device, WCHAR* textureFile)
{
	HRESULT result;
	m_pTexture = new Texture;
	if (!m_pTexture)
		return false;

	result = m_pTexture->Initialize(device, textureFile);
	if (FAILED(result))
		return false;
	return true;
}

void Model::ReleaseTexture()
{
	if (m_pTexture)
	{
		m_pTexture->ShutDown();
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

bool Model::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//创建2个临时缓冲存放顶点和索引缓冲

	m_iVertexCount = 30;
	m_iIndexCount = 30;

	vertices = new VertexType[m_iVertexCount];


	indices = new unsigned long[m_iIndexCount];
	if (!indices)
	{
		return false;
	}


	////创建顺时针方向的三角形，左手规则;
	//创建顺时针方向的三角形，左手规则
	// 设置顶点数据.
	// Load the vertex array with data.
	vertices[0].position = D3DXVECTOR3(-3.0f, -1.0f, 0.0f);
	vertices[0].texture = D3DXVECTOR2(-1.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(-3.0f, 1.0f, 0.0f);
	vertices[1].texture = D3DXVECTOR2(-1.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertices[2].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[3].position = D3DXVECTOR3(-3.0f, -1.0f, 0.0f);
	vertices[3].texture = D3DXVECTOR2(-1.0f, 1.0f);

	vertices[4].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertices[4].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertices[5].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[6].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertices[6].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[7].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertices[7].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[8].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertices[8].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[9].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertices[9].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[10].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertices[10].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[11].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertices[11].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[12].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertices[12].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[13].position = D3DXVECTOR3(-1.0f, 3.0f, 0.0f);
	vertices[13].texture = D3DXVECTOR2(0.0f, -1.0f);

	vertices[14].position = D3DXVECTOR3(1.0f, 3.0f, 0.0f);
	vertices[14].texture = D3DXVECTOR2(1.0f, -1.0f);

	vertices[15].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertices[15].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[16].position = D3DXVECTOR3(1.0f, 3.0f, 0.0f);
	vertices[16].texture = D3DXVECTOR2(1.0f, -1.0f);

	vertices[17].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertices[17].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[18].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertices[18].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[19].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertices[19].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[20].position = D3DXVECTOR3(3.0f, 1.0f, 0.0f);
	vertices[20].texture = D3DXVECTOR2(2.0f, 0.0f);

	vertices[21].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertices[21].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[22].position = D3DXVECTOR3(3.0f, 1.0f, 0.0f);
	vertices[22].texture = D3DXVECTOR2(2.0f, 0.0f);

	vertices[23].position = D3DXVECTOR3(3.0f, -1.0f, 0.0f);
	vertices[23].texture = D3DXVECTOR2(2.0f, 1.0f);

	vertices[24].position = D3DXVECTOR3(-1.0f, -3.0f, 0.0f);
	vertices[24].texture = D3DXVECTOR2(0.0f, 2.0f);

	vertices[25].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertices[25].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[26].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertices[26].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[27].position = D3DXVECTOR3(-1.0f, -3.0f, 0.0f);
	vertices[27].texture = D3DXVECTOR2(0.0f, 2.0f);

	vertices[28].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertices[28].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[29].position = D3DXVECTOR3(1.0f, -3.0f, 0.0f);
	vertices[29].texture = D3DXVECTOR2(1.0f, 2.0f);
	// 设置索引缓冲数据.
	for (int i = 0; i < m_iIndexCount; i++)
	{
		indices[i] = i;
	}

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