#include "MainPlaySceneLoader.h"
#include "../../MainPlayScene/CMainPlayScene.h"
#include "../../../../Utility/Manager/CSceneManager.h"

void MainPlaySceneLoader::loadResources_Init()
{
	auto scene = dynamic_cast<CMainPlayScene*>(FIND_SCENE(GAMESCENE_MAINPLAY));
	scene->init();

	m_bIsInitDownLoad = true;
}

void MainPlaySceneLoader::loadResources_Effects()
{
	m_bIsEffectsDownLoad = true;
}

void MainPlaySceneLoader::loadResources_Sounds()
{
	m_bIsSoundsDownLoad = true;
}

void MainPlaySceneLoader::loadResources_Meshes()
{
	m_bIsMeshesDownLoad = true;
}
