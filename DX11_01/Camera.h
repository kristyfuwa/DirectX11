#pragma once
#include <D3DX10math.h>

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();


	void SetPosition(float, float, float);
	void SetRotation(float, float, float);


	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatirx(D3DXMATRIX&);

private:

	//���������������ϵ�е�λ��
	float	m_fPositionX;
	float	m_fPositionY;
	float	m_fPositionZ;

	//�������ŷ����ת�Ƕ�
	float	m_fRotationX;
	float	m_fRotationY;
	float	m_fRotationZ;

	D3DXMATRIX	m_mViewMatrix;


};

