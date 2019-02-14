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
	SAFE_DELETE(m_pBackGround);
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
	m_pBackGround->update();
	if(m_pLoader!=nullptr)
		m_pLoader->IsAllDownLoad(&m_bIsAllDownLoad, 0, true);
	if (m_bIsAllDownLoad)
	{
		//MessageBox(GET_WINDOW_HANDLE(), _T(m_stNextSceneName.c_str()), _T("ok"), S_OK);
		if (m_pLoader != nullptr)
		{
			m_pLoadThread->join();
			SAFE_DELETE(m_pLoadThread);
			SAFE_DELETE(m_pLoader);
		}
		if (alpha <= 0)
			CHANGE_SCENE_DIRECT(m_stNextSceneName, FALSE);
		else
		{
			alpha -= 60 * GET_DELTA_TIME();
			m_pBackGround->setColor(D3DCOLOR_ARGB((int)alpha, 255, 255, 255));
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
	m_pBackGround->drawUI();
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
	m_pBackGround = new CSpriteObject_Default("Resources/Textures/Scene/LoadingScene/load", "png", 1366, 768, 1);
	m_pBackGround->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
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
	angle += D3DXToRadian(120*GET_DELTA_TIME());
	alpha = 255 * cosf(angle)+100;
	alpha = max(alpha, 150);
	alpha = min(alpha, 255);
	m_pBackGround->setColor(D3DCOLOR_ARGB((int)alpha, 255, 255, 255));
}