#include "CScene.h"


CScene::CScene(std::string a_stSceneName)
	:m_stSceneName(a_stSceneName)
{
}

CScene::~CScene()
{
}

void CScene::init()
{
}

void CScene::update(void)
{
}

void CScene::draw(void)
{
}

void CScene::drawUI(void)
{
}

LRESULT CScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	return S_OK;
}
