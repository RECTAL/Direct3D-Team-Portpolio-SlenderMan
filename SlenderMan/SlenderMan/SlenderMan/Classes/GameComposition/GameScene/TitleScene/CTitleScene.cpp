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
	SAFE_DELETE(sprite_fire);
	SAFE_DELETE(titleImage);
	SAFE_DELETE(gameStartImage);
	SAFE_DELETE(optionImage);
	SAFE_DELETE(exitImage);
	
}

void CTitleScene::init()
{
	CScene::init();
	m_pSprite_BackGround = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/background", "png", 1);
	m_pSprite_BackGround->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));

	sprite_fire = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/fire/fire", "png", 33);
	sprite_fire->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));

	titleImage = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/title", "png", 1);
	titleImage->setPosition(D3DXVECTOR3(450, 250, 0));

	gameStartImage = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/play", "png", 1);
	gameStartImage->setPosition(D3DXVECTOR3(400, 550, 0));

	optionImage = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/option", "png", 1);
	optionImage->setPosition(D3DXVECTOR3(400, 750, 0));

	exitImage = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/exit", "png", 1);
	exitImage->setPosition(D3DXVECTOR3(400, 950, 0));
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
	m_pSprite_BackGround->setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	

	static float time = 0.0f;
	time += GET_DELTA_TIME();
	if (time >= 0.05f)
	{
		sprite_fire->update();
		time = 0;
	}
	titleImage->update();
	gameStartImage->update();
	optionImage->update();
	exitImage->update();
}

void CTitleScene::draw(void)
{
	CScene::draw();
}

void CTitleScene::drawUI(void)
{
	CScene::drawUI();
	m_pSprite_BackGround->drawUI();
	sprite_fire->doDrawUI();
	titleImage->drawUI();
	gameStartImage->drawUI();
	optionImage->drawUI();
	exitImage->drawUI();
}

LRESULT CTitleScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
