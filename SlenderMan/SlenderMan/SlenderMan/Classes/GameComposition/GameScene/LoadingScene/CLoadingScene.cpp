#include "CLoadingScene.h"
#include "../../../Function/GlobalFunction.h"
#include "../LoadingScene/Loader/TitleSceneLoader.h"
#include "../LoadingScene/Loader/MainPlaySceneLoader.h"
#include "../LoadingScene/Loader/MapToolSceneLoader.h"
#include "../LoadingScene/Loader/GameOverSceneLoader.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CSceneManager.h"
#include "../../../Utility/Manager/CTimeManager.h"
#include "../../../Utility/Manager/CDeviceManager.h"
#include "../../../Utility/Manager/CRendertargetManager.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Default.h"

CLoadingScene::CLoadingScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CLoadingScene::~CLoadingScene()
{
	SAFE_DELETE(m_pLoader);
	for (int i = 0; i < 3; i++) {
		SAFE_DELETE(m_pBackGround[i]);
	}
}

void CLoadingScene::init()
{
	CScene::init();

	m_pLoadThread = new std::thread(std::bind(&CLoadingScene::loadResources, this));
	m_bIsAllDownLoad = false;

	if (isFirst)
	{
		this->createBackGround();
		this->createRenderTarget();
		isFirst = false;
	}
	if (m_stNextSceneName == GAMESCENE_TITLE) {
		m_nIndex = 0;
	}
	else if (m_stNextSceneName == GAMESCENE_MAPTOOL) {
		m_nIndex = 1;
	}
	else if (m_stNextSceneName == GAMESCENE_MAINPLAY) {
		m_nIndex = 2;
	}
	else {
		m_nIndex = 0;
	}
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
	if (m_stNextSceneName == GAMESCENE_GAMEOVER)
	{
		m_pLoader = new GameOverSceneLoader();
	}

	if (m_pLoader != nullptr)
	{
		m_pLoader->loadResources_Effects();
		m_pLoader->loadResources_Meshes();
		m_pLoader->loadResources_Sounds();
		m_pLoader->loadResources_Init();
		m_pLoader->CheckAllDownLoad();
	}
	m_stMutex.unlock();
}

void CLoadingScene::createWindowUI()
{

}

void CLoadingScene::update(void)
{
	CScene::update();
	m_pBackGround[m_nIndex]->update();
	if(m_pLoader!=nullptr)
		m_pLoader->IsAllDownLoad(&m_bIsAllDownLoad, 0, true);
	if (m_bIsAllDownLoad)
	{
		if (m_pLoader != nullptr)
		{
			m_pLoadThread->join();
			SAFE_DELETE(m_pLoadThread);
			SAFE_DELETE(m_pLoader);
		}
		if (alpha <= 0)
		{
			CHANGE_SCENE_DIRECT(m_stNextSceneName, FALSE);
		}
		else
		{
			alpha -= 100 * GET_DELTA_TIME();
			m_pBackGround[m_nIndex]->setColor(D3DCOLOR_ARGB((int)alpha, 255, 255, 255));
		}
	}
	else
	{
		this->updateImage();
	}
}

void CLoadingScene::draw(void)
{
	CScene::draw();
	D3DXMATRIXA16 stWorldMatrix;
	D3DXMatrixIdentity(&stWorldMatrix);

	/***************************************************/
	//LoadingRenderTarget에 draw
	/***************************************************/
	GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("LoadingRenderTarget")->m_stRenderTarget.m_pTexSurf);
	GET_DEVICE()->SetDepthStencilSurface(FIND_RENDERTARGET("LoadingRenderTarget")->m_stRenderTarget.m_pDepthStencil);
	GET_DEVICE()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0.0f);

	/***************************************************/
	//Back Buffer로 재설정
	/***************************************************/
	GET_RENDERTARGET_MANAGER()->resetRenderTarget();

	FIND_RENDERTARGET("LoadingRenderTarget")->m_pCopyEffect->SetMatrix("g_stWorldMatrix", &stWorldMatrix);
	FIND_RENDERTARGET("LoadingRenderTarget")->m_pCopyEffect->SetTexture("g_pTexture", FIND_RENDERTARGET("LoadingRenderTarget")->m_stRenderTarget.m_pTex);

	RunEffectLoop(FIND_RENDERTARGET("LoadingRenderTarget")->m_pCopyEffect, "CopyTexture", [=](int nPassNum)->void {
		FIND_RENDERTARGET("LoadingRenderTarget")->getPlaneMesh()->DrawSubset(0);
	});
}

void CLoadingScene::drawUI(void)
{
	CScene::drawUI();

	m_pBackGround[m_nIndex]->drawUI();
}

LRESULT CLoadingScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}

void CLoadingScene::createBackGround(void)
{
	m_pBackGround[0] = new CSpriteObject_Default("Resources/Textures/Scene/LoadingScene/load", "png", 1366, 768, 1);
	
	m_pBackGround[1] = new CSpriteObject_Default("Resources/Textures/Scene/LoadingScene/mapToolLoad", "png", 1366, 768, 1);
	
	m_pBackGround[2] = new CSpriteObject_Default("Resources/Textures/Scene/LoadingScene/mainGameLoad", "png", 1366, 768, 1);
	for (int i = 0; i < 3; i++) {
		m_pBackGround[i]->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
	}
}

void CLoadingScene::createRenderTarget(void)
{	
	D3DVIEWPORT9 stViewport;
	GET_DEVICE()->GetViewport(&stViewport);
	GET_RENDERTARGET_MANAGER()->addRenderTarget("LoadingRenderTarget", new CRenderTarget(GET_WINDOW_SIZE().cx, GET_WINDOW_SIZE().cy, &stViewport));
}

void CLoadingScene::updateImage(void)
{
	static float angle = 0.0f;
	angle += D3DXToRadian(-180 * GET_DELTA_TIME() * 0.25f);
	alpha = 255 * cosf(angle) + 100;
	alpha = max(alpha, 50);
	alpha = min(alpha, 255);

	m_pBackGround[m_nIndex]->setColor(D3DCOLOR_ARGB((int)alpha, 255, 255, 255));
}