#include "MapToolSceneLoader.h"
#include "../../MapToolScene/CMapToolScene.h"
#include "../../../../Utility/Manager/CSceneManager.h"

void MapToolSceneLoader::loadResources_Init()
{
	auto scene = dynamic_cast<CMapToolScene*>(FIND_SCENE(GAMESCENE_MAPTOOL));
	scene->init();

	m_bIsInitDownLoad = true;
}

void MapToolSceneLoader::loadResources_Effects()
{
	m_bIsEffectsDownLoad = true;
}

void MapToolSceneLoader::loadResources_Sounds()
{
	m_bIsSoundsDownLoad = true;
}

void MapToolSceneLoader::loadResources_Meshes()
{
	m_bIsMeshesDownLoad = true;
}
