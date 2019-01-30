#include "MapToolSceneLoader.h"

void MapToolSceneLoader::loadResources_Init()
{
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
