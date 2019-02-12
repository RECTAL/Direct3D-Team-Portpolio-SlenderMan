#include "CMainPlayScene.h"
#include "../MapToolScene/CMapToolScene.h"
#include "../../../Utility/Base/CStage.h"
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
#include "../../../Utility/Manager/CSceneManager.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Button.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Container.h"

CMainPlayScene::CMainPlayScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CMainPlayScene::~CMainPlayScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pSpotObj);
	SAFE_DELETE(menuContainer);
}

void CMainPlayScene::init()
{	
	CScene::init();
	if (isFirst)
	{

		crashFptr = new std::function<void(void)>;
		beginFptr = new std::function<void(void)>;
		pressFptr = new std::function<void(void)>;
		endFptr = new std::function<void(void)>;

		this->createWindowUI();
		this->createRenderTarget();
		this->createCamera();
		this->createContainer();

		m_pSpotObj = this->createSpotObj();
		isFirst = false;

	}


	CMapToolScene* pMapToolScene = dynamic_cast<CMapToolScene*>(FIND_SCENE(GAMESCENE_MAPTOOL));
	pMapToolScene->init();
	m_pStage = pMapToolScene->getStage();
	//m_pStage->setCameraObjMain(m_pCamera);
	//m_pStage->addSpotLightObj(m_pSpotObj);
	//m_pStage->setObjEffectTechname("FogStaticMesh");
	//
	//int nNumSpot = ++m_pStage->getTerrainObj()->getSTParameters().m_nNumSpotLight;
	//if (nNumSpot < 10)
	//{
	//	m_pStage->getTerrainObj()->getSTParameters().m_pSpotLight[nNumSpot - 1] = m_pSpotObj;
	//}
	//
	//m_pStage->getTerrainObj()->getTechniqueName() = "fogTerrain";
	//m_pCamera->setPosition(D3DXVECTOR3(100, 200, 100));



	CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
	CLightObject** ppPointLightObj = new CLightObject*[10];

	CTerrainObject::STParameters stParameters;
	stParameters.m_pCamera = m_pCamera;
	stParameters.m_vfScale = D3DXVECTOR3(1.0f, 0.010f, 1.0f);
	stParameters.m_oHeightFilepath = "Resources/Datas/terrain.raw";
	stParameters.m_oSplatFilepath = "Resources/Textures/Terrain/SplatMap.jpg";
	stParameters.m_oEffectFilepath = "Resources/Effects/DefaultTerrain.fx";

	stParameters.m_stMapSize.cx = 513;
	stParameters.m_stMapSize.cy = 513;

	stParameters.m_nSmoothLevel = 1;

	stParameters.m_nNumSpotLight = 0;
	stParameters.m_pSpotLight = ppSpotLightObj;

	stParameters.m_nNumPointLight = 0;
	stParameters.m_pPointLight = ppPointLightObj;


	for (int i = 0; i < CTerrainObject::MAX_TERRAIN_TEX; ++i) {
		char szFilepath[MAX_PATH] = "";
		sprintf(szFilepath, "Resources/Textures/Terrain/Terrain_%02d.jpg", i + 1);

		stParameters.m_oTextureFilepathList.push_back(szFilepath);
	}


	WIN32_FIND_DATAA fileData;
	if (FindFirstFile("Resources/Datas/ObjPacket.map", &fileData) != INVALID_HANDLE_VALUE)
	{
		m_pStage->setCameraObj(m_pCamera);
		m_pStage->getbIsMaptool() = FALSE;
		m_pStage->load(stParameters, "Resources/Datas/ObjPacket.map");
	}
	else
	{
		m_pStage->setCameraObj(m_pCamera);
		m_pStage->getbIsMaptool() = FALSE;
		m_pStage->load(stParameters, "");
	}
	m_pStage->setCameraObjMain(m_pCamera);
	m_pStage->addSpotLightObj(m_pSpotObj);
	m_pStage->getTerrainObj()->getTechniqueName() = "fogTerrain";
	m_pStage->setObjEffectTechname("FogStaticMesh");
	int nNumSpot = ++m_pStage->getTerrainObj()->getSTParameters().m_nNumSpotLight;
	if (nNumSpot < 10)
	{
		m_pStage->getTerrainObj()->getSTParameters().m_pSpotLight[nNumSpot - 1] = m_pSpotObj;
	}

	m_pCamera->setPosition(D3DXVECTOR3(100, 200, 100));
}

void CMainPlayScene::createWindowUI()
{
	this->createButton();

	menuContainer = new CSpriteObject_Container("Resources/Textures/Scene/MainPlayScene/menuWindow", "png", 500, 500, 1);
	menuContainer->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
	menuContainer->setVisible(false);
	menuContainer->init(nullptr, nullptr, nullptr, nullptr);

	exitButton = new CSpriteObject_Button("Resources/Textures/Scene/MainPlayScene/exit", "png", 100, 100, 2);
	exitButton->setPosition(D3DXVECTOR3(0, 0, 0));
	(*crashFptr) = [=](void)->void
	{
		exitButton->getTextureOffset() = 1;
	};
	(*endFptr) = [=](void)->void
	{
		CHANGE_SCENE_LOADING(GAMESCENE_TITLE, TRUE);
	};
	exitButton->init(crashFptr, nullptr, nullptr, endFptr, true);


	menuContainer->addChildSpriteObject("backTitle", CWindowType::BUTTON, exitButton);
}

void CMainPlayScene::createRenderTarget()
{
	D3DVIEWPORT9 stViewport;
	GET_DEVICE()->GetViewport(&stViewport);
	GET_RENDERTARGET_MANAGER()->addRenderTarget("TestRenderTarget", new CRenderTarget(GET_WINDOW_SIZE().cx, GET_WINDOW_SIZE().cy, &stViewport));

}

void CMainPlayScene::createCamera()
{
	m_pCamera = new CCameraObject((float)GET_WINDOW_SIZE().cx / (float)GET_WINDOW_SIZE().cy);
	m_pCamera->setPosition(D3DXVECTOR3(0.0f, 0.0f, -5.0f));
}

void CMainPlayScene::createSound()
{
	GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/mainBGM_1.wav", true);
}

void CMainPlayScene::setStateSound()
{
	switch (m_pPlayerState)
	{
	case EPlayerState::WALKGRASS:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Grass.wav", false);
		
		if (m_fRunTime > 10.0f) {
			GET_SOUND_MANAGER()->setEffectSoundsVolume(0.9f);
		}
		break;
	case EPlayerState::WALKROCK:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Rock.wav", false);
		GET_SOUND_MANAGER()->setEffectSoundsVolume(0.9f);
		break;
	case EPlayerState::WALKREED:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Reed.wav", false);
		GET_SOUND_MANAGER()->setEffectSoundsVolume(0.9f);
		break;
	case EPlayerState::PICK:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Pick.wav", false);
		break;
	case EPlayerState::SLENDER:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Slenderman.wav", false);
		break;
	case EPlayerState::HEARTBEAT:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/HeartBeat.wav", false);
		break;
	case EPlayerState::NONE:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Breathe.wav", true);
		if (m_fRunTime <= 3.0f) {
			GET_SOUND_MANAGER()->setEffectSoundsVolume(0.8f);
		}
		break;
	}
}

void CMainPlayScene::setBGMSound()
{
	switch (m_pPlayingBGM)
	{
	case EPlayingBGM::WIND:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Wind.wav", false);
		break;
	case EPlayingBGM::RAIN:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Rain.wav", false);
		break;
	case EPlayingBGM::CRIKET:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Criket.wav", false);
		break;
	case EPlayingBGM::BIRD:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Bird.wav", false);
		break;
	case EPlayingBGM::FIRE:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Fire.wav", false);
		break;
	case EPlayingBGM::OWL:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Owl.wav", false);
		break;
	case EPlayingBGM::MYSTERIOUSE:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Mysteriouse.wav", false);
		break;
	case EPlayingBGM::NOISE_1:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Noise_1.wav", false);
		break;
	case EPlayingBGM::NOISE_2:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Noise_2.wav", false);
		break;
	case EPlayingBGM::NOISE_3:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Noise_3.wav", false);
		break;
	case EPlayingBGM::NONE:
		break;
	}
}

void CMainPlayScene::setTimer()
{
	if (m_pPlayerState != EPlayerState::NONE)
	{
		m_fRunTime += GET_DELTA_TIME();
	}
	else {
		m_fRunTime -= GET_DELTA_TIME();
		if (m_fRunTime <= 0.0f)
		{
			m_fRunTime = 0.0f;
		}
	}
}

CSpotLightObject * CMainPlayScene::createSpotObj()
{
	return new CSpotLightObject(0,300.0f,D3DXToRadian(5.0f),D3DXToRadian(15.0f));
}

void CMainPlayScene::createButton()
{
	
}

void CMainPlayScene::createContainer()
{
	
}

void CMainPlayScene::update(void)
{
	CScene::update();
	m_pCamera->update();
	m_pSpotObj->update();
	m_pStage->update();
	menuContainer->update();
	setTimer();
	this->setStateSound();

	m_pSpotObj->setPosition(m_pCamera->getPosition());
	if (isBGMPlay)
	{
		this->createSound();
		isBGMPlay = false;
	}
	if (IS_KEY_PRESSED(DIK_ESCAPE)) {
		menuContainer->setVisible(true);
	}

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

		if (IS_KEY_DOWN(DIK_SPACE)) {
			m_pCamera->moveByYAxis(10 * GET_DELTA_TIME());
		}

		if (IS_KEY_DOWN(DIK_W)) {
		
			m_pCamera->moveByZAxis(fSpeed * GET_DELTA_TIME());
			m_pSpotObj->moveByZAxis(fSpeed * GET_DELTA_TIME());
			m_pPlayerState = EPlayerState::WALKGRASS;
		}
		if(IS_KEY_RELEASED(DIK_W))
		{
			m_pPlayerState = EPlayerState::NONE;
		}
		if (IS_KEY_DOWN(DIK_S)) {
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
	
	m_pStage->draw();

	/***************************************************/
	//Back Buffer로 재설정
	/***************************************************/
	GET_RENDERTARGET_MANAGER()->resetRenderTarget();

	D3DXMATRIXA16 stWorldMatrix;
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
	menuContainer->drawUI();
}

LRESULT CMainPlayScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{
		
	}
	return S_OK;
}
