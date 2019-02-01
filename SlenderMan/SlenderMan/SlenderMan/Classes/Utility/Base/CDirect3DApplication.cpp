#include "CDirect3DApplication.h"
#include "../Manager/CDeviceManager.h"
#include "../Manager/CDeviceManager.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CInputManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager//CSoundManager.h"
#include "../Manager/CRendertargetManager.h"




CDirect3DApplication::CDirect3DApplication(HINSTANCE a_hInstance, const SIZE & a_rstWindowSize, int a_nShowOptions)
	:
	CWindowApplication(a_hInstance, a_rstWindowSize, a_nShowOptions)
{
	// Do Nothing
}

CDirect3DApplication::~CDirect3DApplication(void)
{
	SAFE_RELEASE(m_pSprite);
}

void CDirect3DApplication::initApplication(void)
{
	GET_SCENE_MANAGER()->init();
	CWindowApplication::initApplication();
	GET_DEVICE_MANAGER()->init();

	GET_RENDERTARGET_MANAGER()->init();
	m_pSprite = this->createSprite();
}

void CDirect3DApplication::update(void)
{
	GET_SCENE_MANAGER()->update();
}

void CDirect3DApplication::draw(void)
{
	GET_SCENE_MANAGER()->draw();
}

void CDirect3DApplication::drawUI(void)
{
	GET_SCENE_MANAGER()->drawUI();
}


void CDirect3DApplication::setWindowSize(const SIZE & a_rstWindowSize)
{
	CWindowApplication::setWindowSize(a_rstWindowSize);
}

int CDirect3DApplication::runMessageLoop(void)
{
	// manager init
	GET_TIME_MANAGER()->init();
	GET_INPUT_MANAGER()->init();
	GET_SOUND_MANAGER()->init();

	MSG stMessage;
	ZeroMemory(&stMessage, sizeof(stMessage));


	while (stMessage.message != WM_QUIT) {
		if (PeekMessage(&stMessage, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&stMessage);
			DispatchMessage(&stMessage);
		}
		GET_TIME_MANAGER()->update();
		GET_INPUT_MANAGER()->update();
		
		//clear main buffer
		GET_DEVICE()->Clear(0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
			D3DCOLOR_XRGB(100, 100, 100),
			1.0f,
			0);

		// draw Scene
		if (SUCCEEDED(GET_DEVICE()->BeginScene())) {
			this->update();
			this->draw();





			this->drawUI();
			GET_DEVICE()->EndScene();
		}


		// swap chain
		GET_DEVICE()->Present(NULL, NULL, NULL, NULL);
	}

	return (int)stMessage.wParam;
}

LPD3DXSPRITE CDirect3DApplication::createSprite()
{
	LPD3DXSPRITE pSprite = nullptr;
	
	D3DXCreateSprite(
		GET_DEVICE(),
		&pSprite
	);

	return pSprite;
}

LRESULT CDirect3DApplication::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	GET_SCENE_MANAGER()->handleWindowMessage(a_hWindow, a_nMessage, a_wParam, a_lParam);

	return CWindowApplication::handleWindowMessage(a_hWindow, a_nMessage, a_wParam, a_lParam);
}
