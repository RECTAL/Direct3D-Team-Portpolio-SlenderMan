#include "TitleSceneLoader.h"

void TitleSceneLoader::loadResources_Texture()
{
	for (int i = 0; i < 5000; i++) {
		printf("a");
	}
		m_bIsTextureDownLoad = true;
	
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
