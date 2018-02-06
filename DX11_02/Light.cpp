#include "Light.h"

Light::Light(void)
{
	m_vPosition = D3DXVECTOR3(1.0, 8.0, 1.0);
	m_vGlobalAmbient = D3DXVECTOR3(1.0, 1.0, 1.0);
	m_vDirection = D3DXVECTOR3(-1.0, -1.0, 1.0);
	m_vLightColor = D3DXVECTOR3(1.0, 1.0, 1.0);
	m_fShininess = 5.0f;
}

Light::Light(const Light & others)
{

}

Light::~Light(void)
{
}

void  Light::SetLightPosition(float x, float y, float z)
{
	m_vPosition = D3DXVECTOR3(x, y, z);
}

//����ȫ�ֻ�����ϵ��
void Light::SetGlobalAmbient(float red, float green, float blue)
{
	m_vGlobalAmbient = D3DXVECTOR3(red, green, blue);
	return;
}

//����ƽ�йⷽ��
void Light::SetDirection(float x, float y, float z)
{
	m_vDirection = D3DXVECTOR3(x, y, z);
	return;
}

//���ù�Դ��ɫ
void Light::SetLightColor(float red, float green, float blue)
{
	m_vLightColor = D3DXVECTOR3(red, green, blue);
	return;
}

//����ȫ�ֻ�����ϵ��
void Light::SetShininess(float shininess)
{
	m_fShininess = shininess;
	return;
}

//�õ���Դλ��
D3DXVECTOR3 Light::GetPosition()
{
	return m_vDirection;
}

//�õ�ȫ�ֻ�����ϵ��
D3DXVECTOR3 Light::GetGlobalAmbient()
{
	return m_vGlobalAmbient;
}

//�õ�ƽ�йⷽ��
D3DXVECTOR3 Light::GetDirection()
{
	return m_vDirection;
}

//�õ���Դ��ɫ
D3DXVECTOR3 Light::GetLightColor()
{
	return m_vLightColor;
}

//�õ�ȫ�ֻ�����ϵ��
float Light::GetShininess()
{
	return m_fShininess;
}

