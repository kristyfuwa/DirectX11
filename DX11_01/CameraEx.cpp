#include "CameraEx.h"



CameraEx::CameraEx()
{
	m_eCameraType = AIRCRAFT;
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}


CameraEx::CameraEx(const CameraEx& other)
{

}

CameraEx::~CameraEx()
{
}
//扫视，是指保持观察方向不变，沿向量right方向从一边平移到另一边
void CameraEx::strafe(float units)
{
	if (m_eCameraType == LANDOBJECT)
	{
		m_vPos += D3DXVECTOR3(m_vRight.x, 0.0f, m_vRight.z)*units;
	}
	if (m_eCameraType == AIRCRAFT)
	{
		m_vPos += m_vRight*units;
	}
}
//飞行模式，升降，指沿着向量up方向的移动
void CameraEx::fly(float units)
{
	if (m_eCameraType == LANDOBJECT)
	{
		m_vPos.y += units;
	}
	if (m_eCameraType == AIRCRAFT)
	{
		m_vPos += m_vUp*units;
	}
}
//行走，沿着摄像机观察方向的移动
void CameraEx::walk(float units)
{
	//仅在x,z平面移动
	if (m_eCameraType == LANDOBJECT)
	{
		m_vPos += D3DXVECTOR3(m_vLook.x, 0, m_vLook.z)*units;
	}
	if (m_eCameraType == AIRCRAFT)
	{
		m_vPos += m_vLook*units;
	}
}

void CameraEx::pitch(float angle)
{
	D3DXMATRIX matrix;
	D3DXMatrixRotationAxis(&matrix, &m_vRight, angle);
	// 绕着right向量，旋转up和look

	D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matrix);
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &matrix);
}

void CameraEx::roll(float angle)
{
	//只有AirCraft模式才做roll旋转
	if (m_eCameraType == AIRCRAFT)
	{
		D3DXMATRIX matrix;
		D3DXMatrixRotationAxis(&matrix, &m_vLook, angle);

		//绕着look向量，旋转up,right;
		D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matrix);
		D3DXVec3TransformCoord(&m_vRight, &m_vRight, &matrix);
	}

}

void CameraEx::yaw(float angle)
{
	D3DXMATRIX matrix;
	//对LANDOBJECT,总是绕着(0,1,0)旋转。
	if (m_eCameraType == LANDOBJECT)
	{
		D3DXMatrixRotationY(&matrix, angle);
	}

	//对于aircraft,绕着up向量旋转
	if (m_eCameraType == AIRCRAFT)
	{
		D3DXMatrixRotationAxis(&matrix, &m_vUp, angle);
	}
	// 绕着up或者y轴，旋转right和look
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &matrix);
	D3DXVec3TransformCoord(&m_vRight, &m_vRight, &matrix);
}

void CameraEx::getViewMatrix(D3DXMATRIX* view)
{
	// 保持view局部坐标系，各轴的彼此正交
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	//look X right
	D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);
	D3DXVec3Normalize(&m_vRight,&m_vRight);


	float x = -D3DXVec3Dot(&m_vRight,&m_vPos);
	float y = -D3DXVec3Dot(&m_vUp, &m_vPos);
	float z = -D3DXVec3Dot(&m_vLook, &m_vPos);

	//平移+旋转组成视矩阵

	(*view)(0, 0) = m_vRight.x;     (*view)(0, 1) = m_vUp.x;		(*view)(0, 2) = m_vLook.x;		(*view)(0, 3) = 0.0f;
	(*view)(1, 0) = m_vRight.y;	    (*view)(1, 1) = m_vUp.y;		(*view)(1, 2) = m_vLook.y;		(*view)(1, 3) = 0.0f;
	(*view)(2, 0) = m_vRight.z;		(*view)(2, 1) = m_vUp.z;		(*view)(2, 2) = m_vLook.z;		(*view)(2, 3) = 0.0f;
	(*view)(3, 0) = x;				(*view)(3, 1) = y;				(*view)(3, 2) = z;				(*view)(3, 3) = 1.0f;
}

void CameraEx::setCameraType(CameraType cameraType)
{
	m_eCameraType = cameraType;
}

void CameraEx::getPosition(D3DXVECTOR3* position)
{
	*position = m_vPos;
}

void CameraEx::setPosition(D3DXVECTOR3* position)
{
	m_vPos = *position;
}

void CameraEx::getRight(D3DXVECTOR3* right)
{
	right = &m_vRight;
}

void CameraEx::getUp(D3DXVECTOR3* up)
{
	up = &m_vUp;
}

void CameraEx::getLook(D3DXVECTOR3* look)
{
	look = &m_vLook;
}
