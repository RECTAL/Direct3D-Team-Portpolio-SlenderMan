#include "CWindowApplication.h"
#include "../Manager/CWindowManager.h"

// 전역 변수
static CWindowApplication *g_pInstance = nullptr;

CWindowApplication::CWindowApplication(HINSTANCE a_hInstance, const SIZE & a_rstWindowSize, int a_nShowOptions)
	:
	m_hInstance(a_hInstance),
	m_stWindowSize(a_rstWindowSize),
	m_nShowOptions(a_nShowOptions)
{
	g_pInstance = this;

	if (AllocConsole()) {
		freopen("CONIN$", "rb", stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
	}
}

CWindowApplication::~CWindowApplication(void)
{
	FreeConsole();
}

void CWindowApplication::initApplication(void)
{
	GET_WINDOW_MANAGER()->init(m_hInstance, m_stWindowSize, this);
}

LRESULT CWindowApplication::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage) {
	case WM_DESTROY: {
		this->handleDestroyMessage(a_wParam, a_lParam);
		break;
	}
	}

	return DefWindowProc(a_hWindow, a_nMessage, a_wParam, a_lParam);
}

CWindowApplication * CWindowApplication::getInstance(void)
{
	return g_pInstance;
}

void CWindowApplication::setWindowSize(const SIZE & a_rstWindowSize)
{
	m_stWindowSize.cx = a_rstWindowSize.cx;
	m_stWindowSize.cy = a_rstWindowSize.cy;
}

int CWindowApplication::runApplication(void)
{
	this->initApplication();

	// 윈도우를 출력한다
	ShowWindow(GET_WINDOW_HANDLE(), m_nShowOptions);
	UpdateWindow(GET_WINDOW_HANDLE());

	return this->runMessageLoop();
}

void CWindowApplication::handleDestroyMessage(WPARAM a_wParam, LPARAM a_lParam)
{
	PostQuitMessage(0);
}

int CWindowApplication::runMessageLoop(void)
{
	MSG stMessage;
	ZeroMemory(&stMessage, sizeof(stMessage));

	while (GetMessage(&stMessage, NULL, 0, 0)) {
		TranslateMessage(&stMessage);
		DispatchMessage(&stMessage);
	}

	return (int)stMessage.wParam;
}
