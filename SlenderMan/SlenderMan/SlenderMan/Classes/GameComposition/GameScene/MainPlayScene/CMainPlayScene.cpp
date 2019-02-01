#include "CMainPlayScene.h"
#include "../../../Function/GlobalFunction.h"
#include "../../../Utility/System/RenderSystem/RenderSystem_CRenderTarget.h"
#include "../../../Utility/Object/CameraObject/CCameraObject.h"
#include "../../../Utility/Manager/CSoundManager.h"
#include "../../../Utility/Manager/CRendertargetManager.h"
#include "../../../Utility/Manager/CDeviceManager.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CInputManager.h"
#include "../../../Utility/Manager/CTimeManager.h"

CMainPlayScene::CMainPlayScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CMainPlayScene::~CMainPlayScene()
{
	SAFE_RELEASE(m_pSphere);
	SAFE_DELETE(m_pCamera);
}

void CMainPlayScene::init()
{	
	CScene::init();
	this->createWindowUI();
	this->createRenderTarget();
	this->createMesh();
	this->createCamera();
	this->createSound();
}

void CMainPlayScene::createWindowUI()
{
}

void CMainPlayScene::createRenderTarget()
{
	D3DVIEWPORT9 stViewport;
	GET_DEVICE()->GetViewport(&stViewport);
	GET_RENDERTARGET_MANAGER()->addRenderTarget("TestRenderTarget", new CRenderTarget(GET_WINDOW_SIZE().cx, GET_WINDOW_SIZE().cy, &stViewport));

}

void CMainPlayScene::createMesh()
{
	D3DXCreateSphere(GET_DEVICE(), 3.0f, 10, 10, &m_pSphere, NULL);
}

void CMainPlayScene::createCamera()
{
	m_pCamera = new CCameraObject((float)GET_WINDOW_SIZE().cx/(float)GET_WINDOW_SIZE().cy);
	m_pCamera->setPosition(D3DXVECTOR3(0.0f,0.0f,-5.0f));
}

void CMainPlayScene::createSound()
{
	GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/BGM.wav", true);
}


void CMainPlayScene::update(void)
{
	CScene::update();
	m_pCamera->update();

	if (IS_MOUSE_BUTTON_DOWN(EMouseInput::RIGHT)) {
		float fSpeed = 15.0f;

		if (IS_KEY_DOWN(DIK_LSHIFT)) {
			fSpeed = 50.0f;
		}

		if (IS_MOUSE_BUTTON_PRESSED(EMouseInput::RIGHT)) {
			m_stPrevMousePosition = GET_MOUSE_POSITION();
		}

		auto stMousePosition = GET_MOUSE_POSITION();
		float fDeltaY = stMousePosition.y - m_stPrevMousePosition.y;
		float fDeltaX = stMousePosition.x - m_stPrevMousePosition.x;

		m_stPrevMousePosition = stMousePosition;

		m_pCamera->rotateByXAxis(fDeltaY / 5.0f);
		m_pCamera->rotateByYAxis(fDeltaX / 5.0f,false);

		if (IS_KEY_DOWN(DIK_W)) {
			m_pCamera->moveByZAxis(fSpeed * GET_DELTA_TIME());
		}
		else if (IS_KEY_DOWN(DIK_S)) {
			m_pCamera->moveByZAxis(-fSpeed * GET_DELTA_TIME());
		}

		if (IS_KEY_DOWN(DIK_A)) {
			m_pCamera->moveByXAxis(-fSpeed * GET_DELTA_TIME());
		}
		else if (IS_KEY_DOWN(DIK_D)) {
			m_pCamera->moveByXAxis(fSpeed * GET_DELTA_TIME());
		}

		if (IS_KEY_DOWN(DIK_Q)) {
			m_pCamera->rotateByYAxis(-90.0f * GET_DELTA_TIME(), false);
		}
		else if (IS_KEY_DOWN(DIK_E)) {
			m_pCamera->rotateByYAxis(90.0f * GET_DELTA_TIME(), false);
		}
	}

}

void CMainPlayScene::draw(void)
{
	CScene::draw();
	/***************************************************/
	//TestRenderTarget에 draw
	/***************************************************/
	GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("TestRenderTarget")->m_stRenderTarget.m_pTexSurf);
	GET_DEVICE()->SetDepthStencilSurface(FIND_RENDERTARGET("TestRenderTarget")->m_stRenderTarget.m_pDepthStencil);
	GET_DEVICE()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0.0f);
	

	GET_DEVICE()->SetRenderState(D3DRS_LIGHTING, TRUE);
	GET_DEVICE()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	GET_DEVICE()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXMATRIXA16 stWorldMatrix;
	D3DXMatrixIdentity(&stWorldMatrix);
	GET_DEVICE()->SetTransform(D3DTS_WORLD, &stWorldMatrix);
	
	m_pSphere->DrawSubset(0);

	GET_DEVICE()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	GET_DEVICE()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GET_DEVICE()->SetRenderState(D3DRS_LIGHTING, TRUE);


	/***************************************************/
	//Back Buffer로 재설정
	/***************************************************/
	GET_RENDERTARGET_MANAGER()->resetRenderTarget();
	
	D3DXMatrixIdentity(&stWorldMatrix);

	FIND_RENDERTARGET("TestRenderTarget")->m_pCopyEffect->SetMatrix("g_stWorldMatrix",&stWorldMatrix);
	FIND_RENDERTARGET("TestRenderTarget")->m_pCopyEffect->SetTexture("g_pTexture",FIND_RENDERTARGET("TestRenderTarget")->m_stRenderTarget.m_pTex);

	RunEffectLoop(FIND_RENDERTARGET("TestRenderTarget")->m_pCopyEffect, "CopyTexture", [=](int nPassNum)->void {
		FIND_RENDERTARGET("TestRenderTarget")->getPlaneMesh()->DrawSubset(0);
	});

}

void CMainPlayScene::drawUI(void)
{
	CScene::drawUI();
}

LRESULT CMainPlayScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
