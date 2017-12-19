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

	//��ʼ�����㻺��Ͷ�����������
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

	// ���ö��㻺���Ⱥ�ƫ��.
	stride = sizeof(VertexType);
	offset = 0;

	//��input assemberl�׶ΰ󶨶��㻺�壬�Ա��ܹ�����Ⱦ
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	////��input assemberl�׶ΰ��������壬�Ա��ܹ�����Ⱦ
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ������Ԫ���壬��Ⱦ�������б�
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

	//����2����ʱ�����Ŷ������������

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

	//����˳ʱ�뷽��������Σ����ֹ���;
	//vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	//vertices[0].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	//vertices[1].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	//vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[2].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);

	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // ����
	vertices[0].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // ����.
	vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // ����
	vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);


	//����������������;
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_iVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// ָ�򱣴涥�����ݵ���ʱ����.
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