#include "CGameOverScene.h"

CGameOverScene::CGameOverScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CGameOverScene::~CGameOverScene()
{
}

void CGameOverScene::init()
{
	CScene::init();
}

void CGameOverScene::createWindowUI()
{
}

void CGameOverScene::update(void)
{
	CScene::update();
}

void CGameOverScene::draw(void)
{
	CScene::draw();
}

void CGameOverScene::drawUI(void)
{
	CScene::drawUI();
}

LRESULT CGameOverScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
