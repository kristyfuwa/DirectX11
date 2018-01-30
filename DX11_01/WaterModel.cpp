#include "WaterModel.h"



WaterModel::WaterModel():m_vPreSolution(0),m_vCurrSolution(0),m_tVertices(0)
{
}


WaterModel::WaterModel(const WaterModel& other)
{

}

WaterModel::~WaterModel()
{
}

bool WaterModel::Initialize(ID3D11Device* device, int m, int n, float dx, float dt, float speed, float damping) //���ᣬ������˥��;
{
	bool result;
	// ��ʼ�����㻺��Ͷ�����������.
	result = InitializeBuffers(device, m, n, dx, dt, speed, damping);
	if (!result)
		return false;
	return true;
}

void WaterModel::Shutdown()
{
	// �ͷŶ������������.
	ShutdownBuffers();
}

void WaterModel::Render(ID3D11DeviceContext* deviceContext)
{
	// �Ѷ���������������ͼ�ι��ߣ�׼����Ⱦ.
	RenderBuffers(deviceContext);
}

int WaterModel::GetIndexCount()
{
	//���������������
	return m_iIndexCount;
}

//damping��ʾ����ϵ��
bool WaterModel::InitializeBuffers(ID3D11Device* device, int m, int n, float dx, float dt, float speed, float damping)
{
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_iNumRows = m;
	m_iNumCols = n;

	//����õ����������������Ŀ
	//���ȵõ������ε���Ŀ��Ȼ�����3���Ƕ���������Ŀ
	m_iVertexCount = m*n;
	m_iIndexCount = (m - 1)*(n - 1) * 2 * 3;

	m_fTimeStep = dt;
	m_fSpatialStep = dx;

	float    d = damping* dt + 2.0f;
	float	 e = (speed*speed)*(dt*dt) / (dx*dx);
	m_fK1 = (damping*dt - 2.0f) / d;
	m_fK2 = (4.0f - 8.0f*e) / d;
	m_fK3 = (2.0f*e) / d;

	m_vPreSolution = new D3DXVECTOR3[m*n];
	m_vCurrSolution = new D3DXVECTOR3[m*n];

	//��system memory�в�����������
	float halfWidth = (n - 1)*dx*0.5f;
	float halfDepth = (m - 1)*dx*0.5f;
	for (int i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dx;
		for (int j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			m_vPreSolution[i*n + j] = D3DXVECTOR3(x, 0.0f, z);
			m_vCurrSolution[i*n + j] = D3DXVECTOR3(x, 0.0f, z);
		}
	}

	//���������ʱ���岻Ϊ�գ����ͷ�
	if (m_tVertices)
	{
		// �ͷ���ʱ����.
		delete[] m_tVertices;
		m_tVertices = 0;
	}

	// ����������ʱ����.
	m_tVertices = new VertexType[m_iVertexCount];
	if (!m_tVertices)
	{
		return false;
	}

	// ������������.
	indices = new unsigned long[m_iIndexCount];
	if (!indices)
	{
		return false;
	}

	// ����ÿ��grid������ó�����.
	int k = 0;
	for (int i = 0; i < m - 1; ++i)
	{
		for (int j = 0; j < n - 1; ++j)
		{
			indices[k] = i*n + j;
			indices[k + 1] = i*n + j + 1;
			indices[k + 2] = (i + 1)*n + j;

			indices[k + 3] = (i + 1)*n + j;
			indices[k + 4] = i*n + j + 1;
			indices[k + 5] = (i + 1)*n + j + 1;

			k += 6; //��һ��grid
		}
	}

	// ���ö��㻺����������̬����
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC; //��֧��GPU������֧��CPUд;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)*m_iVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// ָ�򱣴涥�����ݵ���ʱ����.
	vertexData.pSysMem = m_tVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// ����������������.
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE; //��������Դ�Ӳ��仯��ϵͳ���ܻ����Ż����� ��֧��GPU������֧��GPUд����֧��CPU��д
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_iIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ָ�����ʱ��������.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// ������������.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	delete[] indices;
	indices = 0;

	return true;
}

void WaterModel::ShutdownBuffers()
{
	// �ͷŶ��㻺��.
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = 0;
	}

	// �ͷ���������
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = 0;
	}

	//�ͷ�water����λ��
	if (m_vPreSolution)
	{
		delete m_vPreSolution;
		m_vPreSolution = 0;
	}
	if (m_vCurrSolution)
	{
		delete m_vCurrSolution;
		m_vCurrSolution = 0;
	}
	if (m_tVertices)
	{
		// �ͷ���ʱ����.
		delete[] m_tVertices;
		m_tVertices = 0;
	}
}

void WaterModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// ���ö��㻺���Ⱥ�ƫ��.
	stride = sizeof(VertexType);
	offset = 0;

	//��input assemberl�׶ΰ󶨶��㻺�壬�Ա��ܹ�����Ⱦ
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//��input assemberl�׶ΰ��������壬�Ա��ܹ�����Ⱦ
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ������Ԫ���壬��Ⱦ�������б�.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool WaterModel::Update(ID3D11DeviceContext* deviceContext, float dt)
{
	static float t = 0;
	//�ۼ�ʱ��
	t += dt;
	//��ָ����ʱ������ģ��
	if (t >= m_fTimeStep)
	{
		//�����µ�����
		for (int i = 1; i < m_iNumRows - 1; ++i)
		{
			for (int j = 1; j < m_iNumCols - 1; ++j)
			{
				//���º󣬷���m_fPreSolution���ݣ����²����Ļ��帲������
				//j����x��i����z
				m_vPreSolution[i*m_iNumCols + j].y = 
				m_fK1*m_vPreSolution[i*m_iNumCols + j].y +
				m_fK2*m_vCurrSolution[i*m_iNumCols + j].y +
				m_fK3*(m_vCurrSolution[(i+1)*m_iNumCols + j].y +
					m_vCurrSolution[(i - 1)*m_iNumCols + j].y +
					m_vCurrSolution[i*m_iNumCols + j + 1].y +
					m_vCurrSolution[i*m_iNumCols + j - 1].y);//ˮ�����㷨
			}
		}
		//��������
		std::swap(m_vPreSolution, m_vCurrSolution);

		t = 0.0f;//��λʱ��
				 //���¶��㻺�� 
		for (int i = 0; i < m_iVertexCount; ++i)
		{
			m_tVertices[i].position = m_vCurrSolution[i];
			m_tVertices[i].color = BLUE;
		}

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		VertexType*	verticesPtr;
		HRESULT result;


		// �������㻺���Ա���д.
		result = deviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
			return false;

		//�õ�һ�����㻺���ָ��
		verticesPtr = (VertexType*)mappedResource.pData;
		// �������ɵĶ������ݿ��������㻺��
		memcpy(verticesPtr, (void*)m_tVertices, (sizeof(VertexType)*m_iVertexCount));

		//�������㻺��
		deviceContext->Unmap(m_pVertexBuffer, 0);

	}
	return true;
}

void WaterModel::Disturb(int i, int j, float magnitude)//��С
{
	//���ڱ߽紦�Ŷ�
	assert(i > 1 && i < m_iNumRows - 2);
	assert(j > 1 && j < m_iNumCols - 2);

	float halfMag = 0.5f*magnitude;

	//�Ŷ���ij������ĸ߶Ⱥ������ڽӵ�
	m_vCurrSolution[i*m_iNumCols + j].y += magnitude;
	m_vCurrSolution[i*m_iNumCols + j + 1].y += halfMag;
	m_vCurrSolution[i*m_iNumCols + j - 1].y += halfMag;
	m_vCurrSolution[(i + 1)*m_iNumCols + j].y += halfMag;
	m_vCurrSolution[(i - 1)*m_iNumCols + j].y += halfMag;
}