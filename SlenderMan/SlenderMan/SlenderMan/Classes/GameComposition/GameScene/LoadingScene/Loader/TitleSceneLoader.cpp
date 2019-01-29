#include "TitleSceneLoader.h"
#include "../../TitleScene/CTitleScene.h"
#include "../../../../Utility/Manager/CSceneManager.h"

void TitleSceneLoader::loadResources_Init()
{
	auto scene = dynamic_cast<CTitleScene*>(FIND_SCENE(GAMESCENE_TITLE));
	scene->init();
	
	m_bIsInitDownLoad = true;
}

void TitleSceneLoader::loadResources_Effects()
{
	m_bIsEffectsDownLoad = true;
}

void TitleSceneLoader::loadResources_Sounds()
{
	m_bIsSoundsDownLoad = true;
}

void TitleSceneLoader::loadResources_Meshes()
{
	m_bIsMeshesDownLoad = true;
}
