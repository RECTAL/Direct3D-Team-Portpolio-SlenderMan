#include "CGameOverScene.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Default.h"
#include "../../../Utility/Manager/CSceneManager.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CTimeManager.h"
#include "../../../Utility/Manager/CInputManager.h"
#include "../../../Utility/Manager/CSoundManager.h"

CGameOverScene::CGameOverScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CGameOverScene::~CGameOverScene()
{
	SAFE_DELETE(m_pGameOverSprite);
	SAFE_DELETE(m_pDeadSprite);
}

void CGameOverScene::init()
{
	CScene::init();
	//GET_SOUND_MANAGER()->stopAllEffectSounds();
	if (isFirst)
	{
		this->createScene();
		isFirst = false;
	}
	m_bIsStartSound = true;
}

void CGameOverScene::createWindowUI()
{
}

void CGameOverScene::update(void)
{
	CScene::update();
	m_fCurrentTime += GET_DELTA_TIME();
	updateSprite();
	if (m_bIsStartSound) {
		createSound();
		m_bIsStartSound = false;
	}
}

void CGameOverScene::draw(void)
{
	CScene::draw();
}

void CGameOverScene::drawUI(void)
{
	CScene::drawUI();
	
	if (m_fCurrentTime > 5.0f) {
		m_pGameOverSprite->drawUI();
	}
	else {
		m_pDeadSprite->drawUI();
	}
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
	m_pDeadSprite = new CSpriteObject_Default("Resources/Textures/Scene/GameOverScene/Dead", "png", 1633, 768, 10);
	m_pDeadSprite->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));

	m_pGameOverSprite = new CSpriteObject_Default("Resources/Textures/Scene/GameOverScene/gameOver", "png", 1633, 768, 5, true);
	m_pGameOverSprite->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
}

void CGameOverScene::updateSprite(void)
{
	static float fTime = 0.0f;

	fTime += GET_DELTA_TIME();

	if (fTime > 0.1f && m_fCurrentTime < 5.0f) {
		m_pDeadSprite->update();
		fTime = 0.0f;
	}

	if (m_fCurrentTime > 5.0f) {
		if (fTime > 0.08f) {
			m_pGameOverSprite->update();
			fTime = 0.0f;
		}
		if (IS_KEY_DOWN(DIK_RETURN) || m_fCurrentTime > 10.0f)
		{
			m_fCurrentTime = 0.0f;
			GET_SOUND_MANAGER()->stopBackgroundSound();
			CHANGE_SCENE_DIRECT(GAMESCENE_TITLE, TRUE);
		}
	}
}

void CGameOverScene::createSound(void)
{
	GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/EffectSounds/Noise_3.wav", true);
	GET_SOUND_MANAGER()->setBackgroundSoundVolume(0.9f);
}
