#include "InputClass.h"



InputClass::InputClass()
{
}


InputClass::InputClass(const InputClass&)
{

}

InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	int i;

	for (i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
	return;
}

void InputClass::KeyDown(unsigned int key)
{
	m_keys[key] = true;
	return;
}

void InputClass::KeyUp(unsigned int key)
{
	m_keys[key] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}
