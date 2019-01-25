#include "CMapToolScene.h"

CMapToolScene::CMapToolScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CMapToolScene::~CMapToolScene()
{
}

void CMapToolScene::init()
{
	CScene::init();
}

void CMapToolScene::createWindowUI()
{
}


void CMapToolScene::update(void)
{
	CScene::update();
}

void CMapToolScene::draw(void)
{
	CScene::draw();
}

void CMapToolScene::drawUI(void)
{
	CScene::drawUI();
}

LRESULT CMapToolScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
