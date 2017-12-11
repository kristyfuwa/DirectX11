#include "EngineManager.h"

int	WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	EngineManager* pSystem;
	bool result;

	pSystem = new EngineManager;
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

