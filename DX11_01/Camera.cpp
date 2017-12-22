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

	//设置摄像机lookAt方向
	//lookAt.x = 0.0f;
	//lookAt.y = 0.0f;
	//lookAt.z = 1.0f;

	//设置摄像机始终指向原点；
	D3DXVec3Normalize(&lookAt, &position);
	lookAt = lookAt*(-1);

	// 得到弧度单位的欧拉旋转 yaw (Y axis), pitch (X axis), 以及 roll (Z axis) .
	//弧度 = PI/180 * 角度 ;
	//角度 = 180/PI * 弧度 ;
	yaw = m_fRotationY * 0.0174532925f;
	pitch = m_fRotationX * 0.0174532925f;
	roll = m_fRotationZ * 0.0174532925f;

	//得到旋转矩阵
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// 变换lookat和up向量，一般view能够在原点被正确旋转.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// 平移旋转后的摄像机位置.
	lookAt = position + lookAt;

	

	D3DXMatrixLookAtLH(&m_mViewMatrix, &position, &lookAt, &up);


}

void Camera::GetViewMatirx(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_mViewMatrix;
	return;
}

