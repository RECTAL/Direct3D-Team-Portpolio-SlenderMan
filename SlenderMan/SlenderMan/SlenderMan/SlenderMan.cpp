#include "Classes/Utility/Base/CDirect3DApplication.h"


int WINAPI _tWinMain(HINSTANCE a_hInstance,
	HINSTANCE a_hPrevInstance,
	TCHAR *a_pszCommand,
	const int a_nShowOptions)
{
	SIZE stWindowSize = {
		1366  ,768  
	};
	CDirect3DApplication oApplication(a_hInstance, stWindowSize, a_nShowOptions);

	return oApplication.runApplication();
}

