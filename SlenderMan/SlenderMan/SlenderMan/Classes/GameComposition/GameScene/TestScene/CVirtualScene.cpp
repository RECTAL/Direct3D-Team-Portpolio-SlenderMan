#include "CVirtualScene.h"
#include "../../../Utility/Manager/CSceneManager.h"

CVirtualScene::CVirtualScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CVirtualScene::~CVirtualScene()
{
}

void CVirtualScene::update(void)
{
	CScene::update();
	CHANGE_SCENE_LOADING(GAMESCENE_TITLE, TRUE);
}

void CVirtualScene::draw(void)
{
	CScene::draw();
}

void CVirtualScene::drawUI(void)
{
	CScene::drawUI();
}

LRESULT CVirtualScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}

void CVirtualScene::init()
{
	CScene::init();
}

void CVirtualScene::createWindowUI()
{
}
