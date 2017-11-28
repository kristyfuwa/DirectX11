#include "SystemClass.h"

int	WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* pSystem;
	bool result;

	pSystem = new SystemClass;
	if (!pSystem)
	{
		return 0;
	}

	result = pSystem->Initialize();
	if (result)
	{
		pSystem->Run();
	}

	pSystem->Shutdown();
	delete pSystem;
	pSystem = 0;
	return 0;
}

