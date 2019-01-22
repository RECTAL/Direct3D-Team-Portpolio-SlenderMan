#include "CTitleScene.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Default.h"
#include "../../../Utility/Manager/CWindowManager.h"
CTitleScene::CTitleScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CTitleScene::~CTitleScene()
{
	SAFE_DELETE(m_pSprite_BackGround);
}

void CTitleScene::init()
{
	CScene::init();
	m_pSprite_BackGround = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/background", "png", 1);
	m_pSprite_BackGround->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
	m_pSprite_BackGround->setScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pSprite_BackGround->setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
}


void CTitleScene::update(void)
{
	CScene::update();
	m_pSprite_BackGround->update();
}

void CTitleScene::draw(void)
{
	CScene::draw();
}

void CTitleScene::drawUI(void)
{
	CScene::drawUI();
	m_pSprite_BackGround->drawUI();
}
