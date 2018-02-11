#include "CubeModel.h"



CubeModel::CubeModel()
{
	m_iIndexCount = 0;
	m_iVertexCount = 0;
	m_pModel = 0;
}

CubeModel::CubeModel(const CubeModel& others)
{

}

CubeModel::~CubeModel()
{

}

bool CubeModel::Initialize(ID3D11Device* device,char* modelFileName)
{
	bool result;

	//load in the model;
	result = LoadModel(modelFileName);
	if (!result)
		return false;
	//初始化顶点缓冲和顶点索引缓冲
	result = InitializeBuffers(device);
	if (!result)
		return false;

	return true;
}

void CubeModel::Shutdown()
{
	ShutdownBuffers();
	//Release the model data;
	ReleaseModel();
	return;
}

void CubeModel::ShutdownBuffers()
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

int CubeModel::GetIndexCount()
{
	return m_iIndexCount;
}


void CubeModel::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
	return;
}

void CubeModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

bool CubeModel::LoadModel(char* fileName)
{
	ifstream fin;
	char input;
	//open model file

	fin.open(fileName);
	if (fin.fail())
		return false;
	

	//read up to the value of vertex count
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	//read in the vertex count;
	fin >> m_iVertexCount;


	//set the number of indices to be the same as the vertex count;
	m_iIndexCount = m_iVertexCount;
	//Create the model using the vertex count that was read in
	m_pModel = new ModelType[m_iVertexCount];
	if (!m_pModel)
		return false;
	//read up to the beginning of data;
	fin.get(input);//换行符\n
	while (input != ':')
	{
		fin.get(input);
	}

	fin.get(input);//换行符\n
	fin.get(input);//换行符\n

	//read in the vertex data;
	for (int i = 0; i < m_iVertexCount; ++i)
	{
		fin >> m_pModel[i].x >> m_pModel[i].y >> m_pModel[i].z;
		fin >> m_pModel[i].tu >> m_pModel[i].tv;
		fin >> m_pModel[i].nx >> m_pModel[i].ny >> m_pModel[i].nz;
	}
	fin.close();
	return true;
}

void CubeModel::ReleaseModel()
{
	if (m_pModel)
	{
		delete[] m_pModel;
		m_pModel = 0;
	}
}

bool CubeModel::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//创建2个临时缓冲存放顶点和索引缓冲

	m_iVertexCount = 8;
	m_iIndexCount = 36;

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

	//load the vertex array and index array with data;
	for (int i = 0; i < m_iVertexCount; ++i)
	{
		vertices[i].position = D3DXVECTOR3(m_pModel[i].x, m_pModel[i].y, m_pModel[i].z);
		vertices[i].normal = D3DXVECTOR3(m_pModel[i].nx, m_pModel[i].ny, m_pModel[i].nz);

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
