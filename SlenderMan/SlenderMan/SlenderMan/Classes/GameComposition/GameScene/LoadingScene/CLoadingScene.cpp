#include "CLoadingScene.h"
#include "../LoadingScene/Loader/TitleSceneLoader.h"
#include "../LoadingScene/Loader/MainPlaySceneLoader.h"
#include "../LoadingScene/Loader/MapToolSceneLoader.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CSceneManager.h"
CLoadingScene::CLoadingScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CLoadingScene::~CLoadingScene()
{
	
	SAFE_DELETE(m_pLoadThread);
	SAFE_DELETE(m_pLoader);
}

void CLoadingScene::init()
{
	CScene::init();
	m_pLoadThread = new std::thread(std::bind(&CLoadingScene::loadResources, this));
	m_bIsAllDownLoad = false;

}

void CLoadingScene::loadResources()
{
	m_stMutex.lock();
	if (m_pLoader != nullptr) SAFE_DELETE(m_pLoader);
	
	if (m_stNextSceneName == GAMESCENE_TITLE)
	{
		m_pLoader = new TitleSceneLoader();
	}
	if (m_stNextSceneName == GAMESCENE_MAPTOOL)
	{
		m_pLoader = new MapToolSceneLoader();
	}
	if (m_stNextSceneName == GAMESCENE_MAINPLAY)
	{
		m_pLoader = new MainPlaySceneLoader();
	}

	if (m_pLoader != nullptr)
	{
		m_pLoader->loadResources_Texture();
		m_pLoader->loadResources_Effects();
		m_pLoader->loadResources_Meshes();
		m_pLoader->loadResources_Sounds();
		m_pLoader->CheckAllDownLoad();
	}
	m_stMutex.unlock();
}


void CLoadingScene::update(void)
{
	CScene::update();
	m_pLoader->IsAllDownLoad(&m_bIsAllDownLoad, 0, true);
	if (m_bIsAllDownLoad)
	{
		MessageBox(GET_WINDOW_HANDLE(), _T(m_stNextSceneName.c_str()), _T("ok"), S_OK);
		m_pLoadThread->join();
		SAFE_DELETE(m_pLoader);
		CHANGE_SCENE_DIRECT(m_stNextSceneName);
	}
}

void CLoadingScene::draw(void)
{
	CScene::draw();
}

void CLoadingScene::drawUI(void)
{
	CScene::drawUI();
}

