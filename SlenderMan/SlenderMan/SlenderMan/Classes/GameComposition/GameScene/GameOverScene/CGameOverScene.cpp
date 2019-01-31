#include "CGameOverScene.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Default.h"
#include "../../../Utility/Manager/CSceneManager.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CTimeManager.h"
#include "../../../Utility/Manager/CInputManager.h"

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
	m_pGameOverSprite = new CSpriteObject_Default("Resources/Textures/Scene/GameOverScene/gameOver", "png", 1633, 768, 5, true);
	m_pGameOverSprite->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
}

void CGameOverScene::updateSprite(void)
{
	static float fTime = 0.0f;

	fTime += GET_DELTA_TIME();
	
	if (fTime > 0.08f) {
		m_pGameOverSprite->update();
		fTime = 0.0f;
	}
	if (IS_KEY_DOWN(DIK_RETURN))
	{
		CHANGE_SCENE_DIRECT(GAMESCENE_TITLE, TRUE);
	}
}