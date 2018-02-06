#pragma once
#include <D3DX10math.h>

class CameraEx
{
	// 支持两种摄像机模型 AIRCRAFT 允许在空间自由运动, 具有6个自由度
	// LANDOBJECT 沿某些特定轴进行移动
public:
	enum CameraType{LANDOBJECT,AIRCRAFT};

public:
	CameraEx();
	CameraEx(const CameraEx& other);
	~CameraEx();

	void Reset();

	void strafe(float units);//左右
	void fly(float units);//上下
	void walk(float units);//前后

	void pitch(float angle);//旋转view坐标系right向量
	void roll(float angle);//旋转look向量
	void yaw(float angle);//旋转up向量


	void getViewMatrix(D3DXMATRIX* view);
	void setCameraType(CameraType cameraType);
	void getPosition(D3DXVECTOR3* position);
	void setPosition(D3DXVECTOR3* position);


	void getRight(D3DXVECTOR3* right);
	void getUp(D3DXVECTOR3* up);
	void getLook(D3DXVECTOR3* look);

private:
	CameraType				m_eCameraType;
	D3DXVECTOR3				m_vRight;
	D3DXVECTOR3				m_vUp;
	D3DXVECTOR3				m_vLook;
	D3DXVECTOR3				m_vPos;
};


