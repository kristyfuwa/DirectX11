#include "Camera.h"



Camera::Camera()
{
	m_fPositionX = 0.0f;
	m_fPositionY = 0.0f;
	m_fPositionZ = 0.0f;

	m_fRotationX = 0.0f;
	m_fRotationY = 0.0f;
	m_fRotationZ = 0.0f;
}


Camera::~Camera()
{
}

Camera::Camera(const Camera& other)
{

}

void Camera::SetPosition(float x, float y, float z)
{
	m_fPositionX = x;
	m_fPositionY = y;
	m_fPositionZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_fRotationX = x;
	m_fRotationY = y;
	m_fRotationZ = z;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return D3DXVECTOR3(m_fPositionX, m_fPositionY, m_fPositionZ);
}

D3DXVECTOR3 Camera::GetRotation()
{
	return D3DXVECTOR3(m_fRotationX, m_fRotationY, m_fRotationZ);
}

void Camera::Render()
{
	D3DXVECTOR3 position, up, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;
	
	position.x = m_fPositionX;
	position.y = m_fPositionY;
	position.z = m_fPositionZ;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//���������lookAt����
	//lookAt.x = 0.0f;
	//lookAt.y = 0.0f;
	//lookAt.z = 1.0f;

	//���������ʼ��ָ��ԭ�㣻
	D3DXVec3Normalize(&lookAt, &position);
	lookAt = lookAt*(-1);

	// �õ����ȵ�λ��ŷ����ת yaw (Y axis), pitch (X axis), �Լ� roll (Z axis) .
	//���� = PI/180 * �Ƕ� ;
	//�Ƕ� = 180/PI * ���� ;
	yaw = m_fRotationY * 0.0174532925f;
	pitch = m_fRotationX * 0.0174532925f;
	roll = m_fRotationZ * 0.0174532925f;

	//�õ���ת����
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// �任lookat��up������һ��view�ܹ���ԭ�㱻��ȷ��ת.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// ƽ����ת��������λ��.
	lookAt = position + lookAt;

	

	D3DXMatrixLookAtLH(&m_mViewMatrix, &position, &lookAt, &up);


}

void Camera::GetViewMatirx(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_mViewMatrix;
	return;
}

