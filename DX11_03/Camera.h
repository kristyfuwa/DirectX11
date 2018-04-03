#pragma once
#include <D3DX10math.h>
class Camera
{
public:
	Camera();
	~Camera();

	void				strafe(float units);
	void				walk(float units);
	void				fly(float units);

	void				pitch(float angle);
	void				yaw(float angle);
	void				roll(float angle);

	void				getPosition(D3DXVECTOR3* _pos);
	void				setPosition(D3DXVECTOR3*	_pos);
	void				getViewMatrix(D3DXMATRIX* _viewMatrix);

	void				getRight(D3DXVECTOR3* _right);
	void				getLook(D3DXVECTOR3* _look);
	void				getUp(D3DXVECTOR3* _up);
	void				reset();
private:
	D3DXVECTOR3				m_up;
	D3DXVECTOR3				m_right;
	D3DXVECTOR3				m_look;
	D3DXVECTOR3				m_pos;

};

