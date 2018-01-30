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

bool WaterModel::Initialize(ID3D11Device* device, int m, int n, float dx, float dt, float speed, float damping) //阻尼，减幅，衰减;
{
	bool result;
	// 初始化顶点缓冲和顶点索引缓冲.
	result = InitializeBuffers(device, m, n, dx, dt, speed, damping);
	if (!result)
		return false;
	return true;
}

void WaterModel::Shutdown()
{
	// 释放顶点和索引缓冲.
	ShutdownBuffers();
}

void WaterModel::Render(ID3D11DeviceContext* deviceContext)
{
	// 把顶点和索引缓冲放入图形管线，准备渲染.
	RenderBuffers(deviceContext);
}

int WaterModel::GetIndexCount()
{
	//返回索引顶点计数
	return m_iIndexCount;
}

//damping表示阻尼系数
bool WaterModel::InitializeBuffers(ID3D11Device* device, int m, int n, float dx, float dt, float speed, float damping)
{
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_iNumRows = m;
	m_iNumCols = n;

	//计算得到顶点和索引顶点数目
	//首先得到三角形的数目，然后乘以3就是顶点索引数目
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

	//在system memory中产生顶点坐标
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

	//如果顶点临时缓冲不为空，先释放
	if (m_tVertices)
	{
		// 释放临时缓冲.
		delete[] m_tVertices;
		m_tVertices = 0;
	}

	// 创建顶点临时缓冲.
	m_tVertices = new VertexType[m_iVertexCount];
	if (!m_tVertices)
	{
		return false;
	}

	// 创建索引缓冲.
	indices = new unsigned long[m_iIndexCount];
	if (!indices)
	{
		return false;
	}

	// 迭代每个grid，计算得出索引.
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

			k += 6; //下一个grid
		}
	}

	// 设置顶点缓冲描述，动态缓冲
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC; //仅支持GPU读，仅支持CPU写;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)*m_iVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 指向保存顶点数据的临时缓冲.
	vertexData.pSysMem = m_tVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// 设置索引缓冲描述.
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE; //创建后资源从不变化，系统可能会有优化操作 仅支持GPU读，不支持GPU写，不支持CPU读写
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

	delete[] indices;
	indices = 0;

	return true;
}

void WaterModel::ShutdownBuffers()
{
	// 释放顶点缓冲.
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = 0;
	}

	// 释放索引缓冲
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = 0;
	}

	//释放water顶点位置
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
		// 释放临时缓冲.
		delete[] m_tVertices;
		m_tVertices = 0;
	}
}

void WaterModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// 设置顶点缓冲跨度和偏移.
	stride = sizeof(VertexType);
	offset = 0;

	//在input assemberl阶段绑定顶点缓冲，以便能够被渲染
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//在input assemberl阶段绑定索引缓冲，以便能够被渲染
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 设置体元语义，渲染三角形列表.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool WaterModel::Update(ID3D11DeviceContext* deviceContext, float dt)
{
	static float t = 0;
	//累加时间
	t += dt;
	//在指定的时间点更新模拟
	if (t >= m_fTimeStep)
	{
		//仅更新迭代点
		for (int i = 1; i < m_iNumRows - 1; ++i)
		{
			for (int j = 1; j < m_iNumCols - 1; ++j)
			{
				//更新后，放弃m_fPreSolution内容，用新产生的缓冲覆盖它。
				//j索引x，i索引z
				m_vPreSolution[i*m_iNumCols + j].y = 
				m_fK1*m_vPreSolution[i*m_iNumCols + j].y +
				m_fK2*m_vCurrSolution[i*m_iNumCols + j].y +
				m_fK3*(m_vCurrSolution[(i+1)*m_iNumCols + j].y +
					m_vCurrSolution[(i - 1)*m_iNumCols + j].y +
					m_vCurrSolution[i*m_iNumCols + j + 1].y +
					m_vCurrSolution[i*m_iNumCols + j - 1].y);//水波纹算法
			}
		}
		//交换缓冲
		std::swap(m_vPreSolution, m_vCurrSolution);

		t = 0.0f;//复位时间
				 //更新顶点缓冲 
		for (int i = 0; i < m_iVertexCount; ++i)
		{
			m_tVertices[i].position = m_vCurrSolution[i];
			m_tVertices[i].color = BLUE;
		}

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		VertexType*	verticesPtr;
		HRESULT result;


		// 锁定顶点缓冲以便能写.
		result = deviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
			return false;

		//得到一个顶点缓冲的指针
		verticesPtr = (VertexType*)mappedResource.pData;
		// 把新生成的顶点数据拷贝到顶点缓冲
		memcpy(verticesPtr, (void*)m_tVertices, (sizeof(VertexType)*m_iVertexCount));

		//解锁顶点缓冲
		deviceContext->Unmap(m_pVertexBuffer, 0);

	}
	return true;
}

void WaterModel::Disturb(int i, int j, float magnitude)//大小
{
	//不在边界处扰动
	assert(i > 1 && i < m_iNumRows - 2);
	assert(j > 1 && j < m_iNumCols - 2);

	float halfMag = 0.5f*magnitude;

	//扰动第ij个顶点的高度和它的邻接点
	m_vCurrSolution[i*m_iNumCols + j].y += magnitude;
	m_vCurrSolution[i*m_iNumCols + j + 1].y += halfMag;
	m_vCurrSolution[i*m_iNumCols + j - 1].y += halfMag;
	m_vCurrSolution[(i + 1)*m_iNumCols + j].y += halfMag;
	m_vCurrSolution[(i - 1)*m_iNumCols + j].y += halfMag;
}