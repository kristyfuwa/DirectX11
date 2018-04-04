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
	void SetLightAttenuation(float, float, float, float);
	void SetLightSpotLight(float, float, float, float);
	void SetShininess(float);

	D3DXVECTOR4 GetPosition();
	D3DXVECTOR4 GetGlobalAmbient();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetLightColor();
	D3DXVECTOR4 GetLightAttenuation();
	D3DXVECTOR4 GetSpotLight();
	float		GetShininess();


private:
	D3DXVECTOR4		m_vPosition; //光源位置
	D3DXVECTOR4		m_vGlobalAmbient; //全局环境光系数
	D3DXVECTOR3		m_vDirection; //光源方向,平行光
	D3DXVECTOR4		m_vLightColor; //光源颜色
	float			m_fShininess;  //高光系数
	D3DXVECTOR4		m_vAttenuation;//x, y,z分别保存常量、线性以及二次衰减系数
	D3DXVECTOR4		m_vSpotLight;//x,y分别保存内角和外角余弦值
};

