#include "CGameOverScene.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Default.h"
#include "../../../Utility/Manager/CSceneManager.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CTimeManager.h"

CGameOverScene::CGameOverScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CGameOverScene::~CGameOverScene()
{
	SAFE_DELETE(m_pGameOverSprite);
}

void CGameOverScene::init()
{
	CScene::init();
	this->createScene();
}

void CGameOverScene::createWindowUI()
{
}

void CGameOverScene::update(void)
{
	CScene::update();
	updateSprite();
}

void CGameOverScene::draw(void)
{
	CScene::draw();
}

void CGameOverScene::drawUI(void)
{
	CScene::drawUI();
	
	m_pGameOverSprite->drawUI();
}

LRESULT CGameOverScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}

void CGameOverScene::createScene(void)
{
	m_pGameOverSprite = new CSpriteObject_Default("GameOver", "png", 1366, 768, 11, true);
	m_pGameOverSprite->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
}

void CGameOverScene::updateSprite(void)
{
	static float fTime = 0.0f;
	static int count = 0;

	fTime += GET_DELTA_TIME();
	
	if (fTime > 0.03f) {
		m_pGameOverSprite->update();
		fTime = 0.0f;
	}
	if (m_pGameOverSprite->getTextureOffset() < 0 && m_pGameOverSprite->getIsLast())
	{
		count++;
	}
	if (count > 5)
	{
		CHANGE_SCENE_DIRECT(GAMESCENE_TITLE, TRUE);
	}
}