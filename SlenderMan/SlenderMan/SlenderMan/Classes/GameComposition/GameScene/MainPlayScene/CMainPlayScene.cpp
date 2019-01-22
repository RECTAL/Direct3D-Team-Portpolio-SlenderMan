#include "CMainPlayScene.h"

CMainPlayScene::CMainPlayScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CMainPlayScene::~CMainPlayScene()
{
}

void CMainPlayScene::init()
{	
	CScene::init();
}

void CMainPlayScene::update(void)
{
	CScene::update();
}

void CMainPlayScene::draw(void)
{
	CScene::draw();
}

void CMainPlayScene::drawUI(void)
{
	CScene::drawUI();
}

LRESULT CMainPlayScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
