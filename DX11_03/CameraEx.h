#pragma once
#include <D3DX10math.h>

class CameraEx
{
	// ֧�����������ģ�� AIRCRAFT �����ڿռ������˶�, ����6�����ɶ�
	// LANDOBJECT ��ĳЩ�ض�������ƶ�
public:
	enum CameraType{LANDOBJECT,AIRCRAFT};

public:
	CameraEx();
	CameraEx(const CameraEx& other);
	~CameraEx();

	void Reset();

	void strafe(float units);//����
	void fly(float units);//����
	void walk(float units);//ǰ��

	void pitch(float angle);//��תview����ϵright����
	void roll(float angle);//��תlook����
	void yaw(float angle);//��תup����


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


