#include "AxisModel.h"



AxisModel::AxisModel()
{
	m_pIndexBuffer = 0;
	m_pVertexBuffer = 0;
	m_iIndexCount = 0;
	m_iVertexCount = 0;
}


AxisModel::AxisModel(const AxisModel& other)
{

}

AxisModel::~AxisModel()
{
}

bool AxisModel::Initialize(ID3D11Device* device)
{
	bool result;
	//int* p = (int*)malloc(sizeof(int) * 2);
	result = InitializeBuffers(device);
	if (!result)
		return false;
	return true;
}

void AxisModel::Shutdown()
{
	ShutDownBuffers();
	return;
}

void AxisModel::Render(ID3D11DeviceContext* context)
{
	RenderBuffers(context);
}

int AxisModel::GetIndexCount()
{
	return m_iIndexCount;
}

bool AxisModel::InitializeBuffers(ID3D11Device* device)
{
	HRESULT result;
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;	
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	//���ȣ����Ǵ���2����ʱ�����Ŷ�����������ݣ��Ա����ʹ�á�. 
	m_iVertexCount = 6;
	m_iIndexCount = 6;

	vertices = new VertexType[m_iVertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[m_iIndexCount];
	if (!indices)
		return false;

	// ���ö�������.
	//x�ᣬ��ɫ
	vertices[0].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertices[0].color = RED;

	vertices[1].position = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	vertices[1].color = RED;

	vertices[2].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertices[2].color = GREEN;

	vertices[3].position = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	vertices[3].color = GREEN;

	vertices[4].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertices[4].color = BLUE;

	vertices[5].position = D3DXVECTOR3(0.0f, 10.0f, 10.0f);
	vertices[5].color = BLUE;

	// ����������������.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;


	//���ö��㻺������
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)*m_iVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	//vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// ָ�򱣴涥�����ݵ���ʱ����.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;


	// �������㻺��
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
	if (FAILED(result))
		return false;

	//������������
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)*m_iIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ָ�����ʱ��������.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//// ������������.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	if (FAILED(result))
		return false;

	// �ͷ���ʱ����.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;

}

void AxisModel::ShutDownBuffers()
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

void AxisModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//���ö��㻺���Ⱥ�ƫ��
	stride = sizeof(VertexType);
	offset = 0;

	//��input assemberl�׶ΰ󶨶��㻺�壬�Ա��ܹ�����Ⱦ
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//��input assemberl�׶ΰ��������壬�Ա��ܹ�����Ⱦ
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ������Ԫ���壬��Ⱦ�߶Σ�����������
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}
