#include "CTitleScene.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Default.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CTimeManager.h"
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
	m_pSprite_BackGround = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/TitleImage", "png", 1);
	m_pSprite_BackGround->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
	m_pSprite_BackGround->setScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}


void CTitleScene::update(void)
{
	CScene::update();
	m_pSprite_BackGround->update();
	static int alpha = 100.0f;
	static float angle = 0.0f;
	angle += 90 * GET_DELTA_TIME();
	alpha = 255 * cosf(D3DXToRadian(angle))+100;
	alpha = min(alpha, 255);
	alpha = max(alpha, 100);
	m_pSprite_BackGround->setColor(D3DCOLOR_ARGB(alpha, 255, 255, 255));
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

LRESULT CTitleScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
