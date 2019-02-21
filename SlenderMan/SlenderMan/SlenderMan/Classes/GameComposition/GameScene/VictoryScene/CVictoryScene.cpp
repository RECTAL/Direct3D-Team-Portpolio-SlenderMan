#include "CVictoryScene.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CTimeManager.h"
#include "../../../Utility/Manager/CSceneManager.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Default.h"
#include "../../../Utility/Manager/CSoundManager.h"

CVictoryScene::CVictoryScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CVictoryScene::~CVictoryScene()
{
	SAFE_DELETE(background);
}

void CVictoryScene::init()
{
	CScene::init();
	if (isFirst)
	{
		background = new CSpriteObject_Default("Resources/Textures/Scene/VictoryScene/ending/ending", "png", 1366, 768, 64, false);
		background->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
		isFirst = false;
	}
	m_bIsStartSound = true;
}

void CVictoryScene::createSound(void)
{
	GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/Victory.wav", false);
}



void CVictoryScene::update(void)
{
	CScene::update();
	static float time = 0.0f;

	time += GET_DELTA_TIME();
	if (time >= 0.05f)
	{
		background->update();
		if (background->getTextureOffset() >= 63)
		{
			CHANGE_SCENE_DIRECT(GAMESCENE_TITLE, TRUE);
		}
		time = 0;
	}
	if (m_bIsStartSound)
	{
		this->createSound();
		m_bIsStartSound = false;
	}

}

void CVictoryScene::draw(void)
{
	CScene::draw();
}

void CVictoryScene::drawUI(void)
{
	CScene::drawUI();
	background->drawUI();
}
