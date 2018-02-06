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

//设置全局环境光系数
void Light::SetGlobalAmbient(float red, float green, float blue)
{
	m_vGlobalAmbient = D3DXVECTOR3(red, green, blue);
	return;
}

//设置平行光方向
void Light::SetDirection(float x, float y, float z)
{
	m_vDirection = D3DXVECTOR3(x, y, z);
	return;
}

//设置光源颜色
void Light::SetLightColor(float red, float green, float blue)
{
	m_vLightColor = D3DXVECTOR3(red, green, blue);
	return;
}

//设置全局环境光系数
void Light::SetShininess(float shininess)
{
	m_fShininess = shininess;
	return;
}

//得到光源位置
D3DXVECTOR3 Light::GetPosition()
{
	return m_vDirection;
}

//得到全局环境光系数
D3DXVECTOR3 Light::GetGlobalAmbient()
{
	return m_vGlobalAmbient;
}

//得到平行光方向
D3DXVECTOR3 Light::GetDirection()
{
	return m_vDirection;
}

//得到光源颜色
D3DXVECTOR3 Light::GetLightColor()
{
	return m_vLightColor;
}

//得到全局环境光系数
float Light::GetShininess()
{
	return m_fShininess;
}

