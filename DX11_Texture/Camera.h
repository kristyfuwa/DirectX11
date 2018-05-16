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

	//摄像机在世界坐标系中的位置
	float	m_fPositionX;
	float	m_fPositionY;
	float	m_fPositionZ;

	//摄像机的欧拉旋转角度
	float	m_fRotationX;
	float	m_fRotationY;
	float	m_fRotationZ;

	D3DXMATRIX	m_mViewMatrix;


};

