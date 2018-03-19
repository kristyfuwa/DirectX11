#pragma once
#include <d3d11.h>
#include <D3DX10math.h>
class Light
{
public:
	Light();
	Light(const Light&);
	~Light();

	void SetLightPosition(float, float, float,float);
	void SetGlobalAmbient(float, float, float,float);
	void SetDirection(float, float, float);
	void SetLightColor(float, float, float,float);
	void SetShininess(float);

	D3DXVECTOR4 GetPosition();
	D3DXVECTOR4 GetGlobalAmbient();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetLightColor();
	float		GetShininess();


private:
	D3DXVECTOR4		m_vPosition; //��Դλ��
	D3DXVECTOR4		m_vGlobalAmbient; //ȫ�ֻ�����ϵ��
	D3DXVECTOR3		m_vDirection; //��Դ����,ƽ�й�
	D3DXVECTOR4		m_vLightColor; //��Դ��ɫ
	float			m_fShininess;  //�߹�ϵ��
};
