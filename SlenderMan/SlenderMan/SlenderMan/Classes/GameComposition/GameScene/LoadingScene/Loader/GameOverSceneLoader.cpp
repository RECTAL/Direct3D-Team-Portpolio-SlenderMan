#include "GameOverSceneLoader.h"
#include "../../GameOverScene/CGameOverScene.h"
#include "../../../../Utility/Manager/CSceneManager.h"

void GameOverSceneLoader::loadResources_Init()
{
	auto scene = dynamic_cast<CGameOverScene*>(FIND_SCENE(GAMESCENE_GAMEOVER));
	scene->init();

	m_bIsInitDownLoad = true;
}

void GameOverSceneLoader::loadResources_Effects()
{

	m_bIsEffectsDownLoad = true;
}

void GameOverSceneLoader::loadResources_Sounds()
{

	m_bIsSoundsDownLoad = true;
}

void GameOverSceneLoader::loadResources_Meshes()
{

	m_bIsMeshesDownLoad = true;
}
