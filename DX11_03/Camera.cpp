#include "Camera.h"

Camera::Camera()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}


Camera::~Camera()
{
}

void Camera::strafe(float units)
{
	m_pos += m_right*units;
}

void Camera::walk(float units)
{
	m_pos += m_look*units;
}

void Camera::fly(float units)
{
	m_pos += m_up*units;
}

void Camera::pitch(float angle)
{
	D3DXMATRIX t;
	D3DXMatrixRotationAxis(&t, &m_right, angle);

	// 绕着right向量，旋转up和look
	D3DXVec3TransformCoord(&m_look, &m_look, &t);
	D3DXVec3TransformCoord(&m_up, &m_up, &t);
}

void Camera::yaw(float angle)
{
	D3DXMATRIX t;
	D3DXMatrixRotationAxis(&t, &m_up, angle);

	D3DXVec3TransformCoord(&m_right, &m_right, &t);
	D3DXVec3TransformCoord(&m_look, &m_look, &t);
}

void Camera::roll(float angle)
{
	D3DXMATRIX t;
	D3DXMatrixRotationAxis(&t, &m_look, angle);

	D3DXVec3TransformCoord(&m_right, &m_right, &t);
	D3DXVec3TransformCoord(&m_up, &m_up, &t);
}

void Camera::getPosition(D3DXVECTOR3* _pos)
{
	*_pos = m_pos;
}

void Camera::setPosition(D3DXVECTOR3* _pos)
{
	m_pos = *_pos;
}

void Camera::getViewMatrix(D3DXMATRIX* _viewMatrix)
{
	//保证view局部坐标系，各轴彼此正交
	D3DXVec3Normalize(&m_look, &m_look);
	
	D3DXVec3Cross(&m_up, &m_look, &m_right);
	D3DXVec3Normalize(&m_up, &m_up);

	D3DXVec3Cross(&m_right, &m_up, &m_look);
	D3DXVec3Normalize(&m_right, &m_right);

	//生成view矩阵
	float x = -D3DXVec3Dot(&m_right, &m_pos);
	float y = -D3DXVec3Dot(&m_up, &m_pos);
	float z = -D3DXVec3Dot(&m_look, &m_pos);

	(*_viewMatrix)(0, 0) = m_right.x;	(*_viewMatrix)(0, 1) = m_up.x;	(*_viewMatrix)(0, 2) = m_look.x;	(*_viewMatrix)(0, 3) = 0.0f;
	(*_viewMatrix)(1, 0) = m_right.y;	(*_viewMatrix)(1, 1) = m_up.y;	(*_viewMatrix)(1, 2) = m_look.y;	(*_viewMatrix)(1, 3) = 0.0f;
	(*_viewMatrix)(2, 0) = m_right.z;	(*_viewMatrix)(2, 1) = m_up.z;	(*_viewMatrix)(2, 2) = m_look.z;	(*_viewMatrix)(2, 3) = 0.0f;
	(*_viewMatrix)(3, 0) = x;			(*_viewMatrix)(3, 1) = y;		(*_viewMatrix)(3, 2) = z;			(*_viewMatrix)(3, 3) = 1.0f;
}

void Camera::getRight(D3DXVECTOR3* _right)
{
	*_right = m_right;
}

void Camera::getLook(D3DXVECTOR3* _look)
{
	*_look = m_look;
}

void Camera::getUp(D3DXVECTOR3* _up)
{
	*_up = m_up;
}

void Camera::reset()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}
