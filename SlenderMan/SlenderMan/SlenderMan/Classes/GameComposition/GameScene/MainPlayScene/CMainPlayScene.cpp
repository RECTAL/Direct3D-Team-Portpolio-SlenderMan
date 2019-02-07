#include "CMainPlayScene.h"
#include "../../../Function/GlobalFunction.h"
#include "../../../Utility/Object/TerrainObject/CTerrainObject.h"
#include "../../../Utility/Object/StaticObject/CStaticObject.h"
#include "../../../Utility/Object/LightObject/CLightObject.h"
#include "../../../Utility/Object/LightObject/SpotLightObject/SpotLightObject.h"
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
	SAFE_DELETE(m_pTerrain);
	SAFE_RELEASE(m_pSphere);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pLightObj);
	SAFE_DELETE(m_pSpotObj);
}

void CMainPlayScene::init()
{	
	CScene::init();
	if (isFirst)
	{
		this->createWindowUI();
		this->createRenderTarget();
		this->createMesh();
		this->createCamera();
		this->createSound();

		m_pLightObj = this->createDirectionalLight();
		m_pSpotObj = this->createSpotObj();

		m_pTerrain = this->createTerrain();
		m_pTerrain->getTechniqueName() = "DefaultTerrain";
		m_pTerrain->setPosition(D3DXVECTOR3(0, 0, 0));

		m_pStaticMesh = this->createStaticMesh();
		m_pStaticMesh->getTechniqueName() = "DefaultStaticMesh";
		m_pStaticMesh->setPosition(D3DXVECTOR3(0.0f, 20.0f, 0.0f));

		m_pStaticMesh1 = this->createStaticMesh();
		m_pStaticMesh1->getTechniqueName() = "DefaultStaticMesh";
		m_pStaticMesh1->setPosition(D3DXVECTOR3(1.0f, 20.0f, 2.0f));

		m_pStaticMesh2 = this->createStaticMesh();
		m_pStaticMesh2->getTechniqueName() = "DefaultStaticMesh";
		m_pStaticMesh2->setPosition(D3DXVECTOR3(-1.0f, 20.0f, -2.0f));


		isFirst = false;
	}
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

CTerrainObject * CMainPlayScene::createTerrain()
{
	CTerrainObject::STParameters stParameters;
	stParameters.m_pCamera = m_pCamera;
	stParameters.m_vfScale = D3DXVECTOR3(1.0f, 0.007f, 1.0f);
	stParameters.m_oHeightFilepath = "Resources/Datas/HeightMap.raw";
	stParameters.m_oSplatFilepath = "Resources/Textures/Terrain/HeightMap.bmp";
	stParameters.m_oEffectFilepath = "Resources/Effects/DefaultTerrain.fx";

	stParameters.m_stMapSize.cx = 257;
	stParameters.m_stMapSize.cy = 257;

	stParameters.m_nSmoothLevel = 1;

	stParameters.m_nNumSpotLight = 1;
	stParameters.m_pSpotLight = m_pSpotObj;

	stParameters.m_nNumPointLight = 0;
	stParameters.m_pPointLight = NULL;


	for (int i = 0; i < CTerrainObject::MAX_TERRAIN_TEX; ++i) {
		char szFilepath[MAX_PATH] = "";
		sprintf(szFilepath, "Resources/Textures/Terrain/Terrain_%02d.jpg", i + 1);

		stParameters.m_oTextureFilepathList.push_back(szFilepath);
	}

	return new CTerrainObject(stParameters);
}

CStaticObject * CMainPlayScene::createStaticMesh()
{
	CStaticObject::STParameters stParameters = {
		m_pCamera,
		m_pLightObj,
		1,m_pSpotObj,
		0,NULL,
		"Resources/Meshes/slenderMan/slenderMan.X",
		"Resources/Effects/DefaultStaticMesh.fx"
	};
	return new CStaticObject(stParameters);
}

CLightObject * CMainPlayScene::createDirectionalLight()
{
	return new CLightObject(0);
}

CSpotLightObject * CMainPlayScene::createSpotObj()
{
	return new CSpotLightObject(0,140.0f,D3DXToRadian(5.0f), D3DXToRadian(20.0f));
}

void CMainPlayScene::createSound()
{
	GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/BGM_1.wav", true);
}


void CMainPlayScene::update(void)
{
	CScene::update();
	m_pCamera->update();
	m_pTerrain->update();
	m_pLightObj->update();
	m_pStaticMesh->update();
	m_pStaticMesh1->update();
	m_pStaticMesh2->update();
	m_pSpotObj->update();

	//m_pSpotObj->setPosition(m_pCamera->getPosition());
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
		m_pSpotObj->rotateByXAxis(fDeltaY / 5.0f);

		m_pCamera->rotateByYAxis(fDeltaX / 5.0f,false);
		m_pSpotObj->rotateByYAxis(fDeltaX / 5.0f,false);

		
		if (IS_KEY_DOWN(DIK_UP)) {
			m_pCamera->moveByZAxis(fSpeed * GET_DELTA_TIME());
			
		}
		else if (IS_KEY_DOWN(DIK_DOWN)) {
			m_pCamera->moveByZAxis(-fSpeed * GET_DELTA_TIME());
			
		}

		if (IS_KEY_DOWN(DIK_LEFT)) {
			m_pCamera->moveByXAxis(-fSpeed * GET_DELTA_TIME());
			
		}
		else if (IS_KEY_DOWN(DIK_RIGHT)) {
			m_pCamera->moveByXAxis(fSpeed * GET_DELTA_TIME());
			
		}

		if (IS_KEY_DOWN(DIK_W)) {
			m_pCamera->moveByZAxis(fSpeed * GET_DELTA_TIME());
			m_pSpotObj->moveByZAxis(fSpeed * GET_DELTA_TIME());
		}
		else if (IS_KEY_DOWN(DIK_S)) {
			m_pCamera->moveByZAxis(-fSpeed * GET_DELTA_TIME());
			m_pSpotObj->moveByZAxis(-fSpeed * GET_DELTA_TIME());
		}

		if (IS_KEY_DOWN(DIK_A)) {
			m_pCamera->moveByXAxis(-fSpeed * GET_DELTA_TIME());
			m_pSpotObj->moveByXAxis(-fSpeed * GET_DELTA_TIME());
		}
		else if (IS_KEY_DOWN(DIK_D)) {
			m_pCamera->moveByXAxis(fSpeed * GET_DELTA_TIME());
			m_pSpotObj->moveByXAxis(fSpeed * GET_DELTA_TIME());
		}

		if (IS_KEY_DOWN(DIK_Q)) {
			//m_pCamera->rotateByYAxis(-90.0f * GET_DELTA_TIME(), false);
			m_pSpotObj->rotateByYAxis(-90.0f * GET_DELTA_TIME(), false);
		}
		else if (IS_KEY_DOWN(DIK_E)) {
			//m_pCamera->rotateByYAxis(90.0f * GET_DELTA_TIME(), false);
			m_pSpotObj->rotateByYAxis(90.0f * GET_DELTA_TIME(), false);
		}
		else if (IS_KEY_DOWN(DIK_Z)) {
			//m_pCamera->rotateByYAxis(-90.0f * GET_DELTA_TIME(), false);
			m_pSpotObj->rotateByXAxis(-90.0f * GET_DELTA_TIME(), false);
		}
		else if (IS_KEY_DOWN(DIK_C)) {
			//m_pCamera->rotateByYAxis(90.0f * GET_DELTA_TIME(), false);
			m_pSpotObj->rotateByXAxis(90.0f * GET_DELTA_TIME(), false);
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
	GET_DEVICE()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0.0f);
	

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

	m_pTerrain->draw();
	m_pStaticMesh->draw();
	m_pStaticMesh1->draw();
	m_pStaticMesh2->draw();

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
