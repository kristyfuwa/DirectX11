#include "Light.h"

Light::Light(void)
{
	m_vPosition = D3DXVECTOR4(5.0, 5.0, -3.0,1.0);
	m_vGlobalAmbient = D3DXVECTOR4(1.0,1.0, 1.0, 1.0);
	m_vDirection = D3DXVECTOR3(-1.0, -1.0, 1.0);
	m_vLightColor = D3DXVECTOR4(1.0, 1.0, 1.0, 1.0);
	m_fShininess = 5.0f;
	m_vAttenuation = D3DXVECTOR4(1.0, 0.0, 0.0, 1.0);
	m_vSpotLight = D3DXVECTOR4(0.7, 0.6, 0.0, 1.0);
}

Light::Light(const Light & others)
{

}

Light::~Light(void)
{
}

void  Light::SetLightPosition(float x, float y, float z,float w)
{
	m_vPosition = D3DXVECTOR4(x, y, z,w);
}

//设置全局环境光系数
void Light::SetGlobalAmbient(float red, float green, float blue,float alpha)
{
	m_vGlobalAmbient = D3DXVECTOR4(red, green, blue,alpha);
	return;
}

//设置平行光方向
void Light::SetDirection(float x, float y, float z)
{
	m_vDirection = D3DXVECTOR3(x, y, z);
	return;
}

//设置光源颜色
void Light::SetLightColor(float red, float green, float blue,float alpha)
{
	m_vLightColor = D3DXVECTOR4(red, green, blue,alpha);
	return;
}

void Light::SetLightAttenuation(float x, float y, float z, float w)
{
	m_vAttenuation = D3DXVECTOR4(x, y, z, w);
}

void Light::SetLightSpotLight(float x, float y, float z, float w)
{
	m_vSpotLight = D3DXVECTOR4(x, y, z, w);
}

//设置全局环境光系数
void Light::SetShininess(float shininess)
{
	m_fShininess = shininess;
	return;
}

//得到光源位置
D3DXVECTOR4 Light::GetPosition()
{
	return m_vPosition;
}

//得到全局环境光系数
D3DXVECTOR4 Light::GetGlobalAmbient()
{
	return m_vGlobalAmbient;
}

//得到平行光方向
D3DXVECTOR3 Light::GetDirection()
{
	return m_vDirection;
}

//得到光源颜色
D3DXVECTOR4 Light::GetLightColor()
{
	return m_vLightColor;
}

D3DXVECTOR4 Light::GetLightAttenuation()
{
	return m_vAttenuation;
}

D3DXVECTOR4 Light::GetSpotLight()
{
	return m_vSpotLight;
}

//得到全局环境光系数
float Light::GetShininess()
{
	return m_fShininess;
}

