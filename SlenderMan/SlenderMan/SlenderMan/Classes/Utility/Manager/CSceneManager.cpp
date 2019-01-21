#include "CSceneManager.h"
#include "../../GameComposition/GameScene/TitleScene/CTitleScene.h"
#include "../../GameComposition/GameScene/MapToolScene/CMapToolScene.h"
#include "../../GameComposition/GameScene/MainPlayScene/CMainPlayScene.h"
#include "../../GameComposition/GameScene/GameOverScene/CGameOverScene.h"
#include "../../GameComposition/GameScene/VictoryScene/CVictoryScene.h"
#include "../../GameComposition/GameScene/LoadingScene/CLoadingScene.h"

void CSceneManager::init()
{
	if (FAILED(this->addScene(GAMESCENE_TITLE, new CTitleScene(GAMESCENE_TITLE))))
	{
		assert("addScene: TitleScene Failed");
	}
	if (FAILED(this->addScene(GAMESCENE_LOADING, new CLoadingScene(GAMESCENE_LOADING))))
	{
		assert("addScene: LoadingScene Failed");
	}
	if (FAILED(this->addScene(GAMESCENE_MAPTOOL, new CMapToolScene(GAMESCENE_MAPTOOL))))
	{
		assert("addScene: MapToolScene Failed");
	}
	if (FAILED(this->addScene(GAMESCENE_MAINPLAY, new CMainPlayScene(GAMESCENE_MAINPLAY))))
	{
		assert("addScene: MainPlayScene Failed");
	}
	if (FAILED(this->addScene(GAMESCENE_GAMEOVER, new CMainPlayScene(GAMESCENE_GAMEOVER))))
	{
		assert("addScene: GameOverScene Failed");
	}
	if (FAILED(this->addScene(GAMESCENE_VICTORY, new CVictoryScene(GAMESCENE_VICTORY))))
	{
		assert("addScene: VictoryScene Failed");
	}

	this->changeScene_rootLoadingScene(GAMESCENE_TITLE);
}

CSceneManager::~CSceneManager()
{
	this->release();
}


void CSceneManager::release()
{
	for (auto iter: m_oSceneMapList)
	{
		SAFE_DELETE(iter.second);
	}
	m_oSceneMapList.clear();
}

void CSceneManager::update(void)
{
	m_pCurrentScene->update();
}

void CSceneManager::draw(void)
{
	m_pCurrentScene->draw();
}

void CSceneManager::drawUI(void)
{
	m_pCurrentScene->drawUI();
}

CScene * CSceneManager::changeScene_rootDirect(std::string a_stSceneName)
{
	if (m_oSceneMapList.find(a_stSceneName) != m_oSceneMapList.end())
	{
		m_oSceneMapList[a_stSceneName]->init();
		m_pCurrentScene = m_oSceneMapList[a_stSceneName];
		return m_oSceneMapList[a_stSceneName];
	}
	return nullptr;
}

CScene * CSceneManager::changeScene_rootLoadingScene(std::string a_stNextSceneName)
{
	if (m_oSceneMapList.find(GAMESCENE_LOADING) != m_oSceneMapList.end())
	{
		CLoadingScene* pLoadScene = dynamic_cast<CLoadingScene*>(m_oSceneMapList[GAMESCENE_LOADING]);
		pLoadScene->setNextSceneName(a_stNextSceneName);
		pLoadScene->init();
		m_pCurrentScene = pLoadScene;
		return pLoadScene;
	}
	return nullptr;
}

HRESULT CSceneManager::addScene(std::string a_stSceneName, CScene * a_pScene)
{
	if (m_oSceneMapList.find(a_stSceneName) == m_oSceneMapList.end())
	{
		m_oSceneMapList.insert(decltype(m_oSceneMapList)::value_type(a_stSceneName, a_pScene));
		return S_OK;
	}
	return S_FALSE;
}

HRESULT CSceneManager::deleteScene(std::string a_stSceneName)
{
	if (m_oSceneMapList.find(a_stSceneName) != m_oSceneMapList.end())
	{
		SAFE_DELETE(m_oSceneMapList[a_stSceneName]);
		return S_OK;
	}
	return S_FALSE;
}