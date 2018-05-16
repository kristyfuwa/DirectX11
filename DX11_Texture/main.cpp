#include "EngineManager.h"

int	WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

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

