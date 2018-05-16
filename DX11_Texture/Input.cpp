#include "Input.h"



Input::Input()
{
}


Input::Input(const Input&)
{

}

Input::~Input()
{
}

void Input::Initialize()
{
	int i;

	for (i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
	return;
}

void Input::KeyDown(unsigned int key)
{
	m_keys[key] = true;
	return;
}

void Input::KeyUp(unsigned int key)
{
	m_keys[key] = false;
	return;
}

bool Input::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}
