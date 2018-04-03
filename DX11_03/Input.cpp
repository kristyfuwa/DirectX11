#include "Input.h"



Input::Input()
{
}


Input::~Input()
{
}

void Input::initialize()
{
	for (size_t i = 0; i < 256; ++i)
	{
		m_keys[i] = 0;
	}
}

void Input::keyDown(unsigned int key)
{
	m_keys[key] = 1;
}

void Input::keyUp(unsigned int key)
{
	m_keys[key] = 0;
}

bool Input::isKeyDown(unsigned int i)
{
	return m_keys[i];
}

