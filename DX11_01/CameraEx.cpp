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
//ɨ�ӣ���ָ���ֹ۲췽�򲻱䣬������right�����һ��ƽ�Ƶ���һ��
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
//����ģʽ��������ָ��������up������ƶ�
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
//���ߣ�����������۲췽����ƶ�
void CameraEx::walk(float units)
{
	//����x,zƽ���ƶ�
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
	// ����right��������תup��look

	D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matrix);
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &matrix);
}

void CameraEx::roll(float angle)
{
	//ֻ��AirCraftģʽ����roll��ת
	if (m_eCameraType == AIRCRAFT)
	{
		D3DXMATRIX matrix;
		D3DXMatrixRotationAxis(&matrix, &m_vLook, angle);

		//����look��������תup,right;
		D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matrix);
		D3DXVec3TransformCoord(&m_vRight, &m_vRight, &matrix);
	}

}

void CameraEx::yaw(float angle)
{
	D3DXMATRIX matrix;
	//��LANDOBJECT,��������(0,1,0)��ת��
	if (m_eCameraType == LANDOBJECT)
	{
		D3DXMatrixRotationY(&matrix, angle);
	}

	//����aircraft,����up������ת
	if (m_eCameraType == AIRCRAFT)
	{
		D3DXMatrixRotationAxis(&matrix, &m_vUp, angle);
	}
	// ����up����y�ᣬ��תright��look
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &matrix);
	D3DXVec3TransformCoord(&m_vRight, &m_vRight, &matrix);
}

void CameraEx::getViewMatrix(D3DXMATRIX* view)
{
	// ����view�ֲ�����ϵ������ı˴�����
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	//look X right
	D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);
	D3DXVec3Normalize(&m_vRight,&m_vRight);


	float x = -D3DXVec3Dot(&m_vRight,&m_vPos);
	float y = -D3DXVec3Dot(&m_vUp, &m_vPos);
	float z = -D3DXVec3Dot(&m_vLook, &m_vPos);

	//ƽ��+��ת����Ӿ���

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
