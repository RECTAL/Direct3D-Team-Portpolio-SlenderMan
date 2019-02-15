#include "CMainPlayScene.h"
#include "../MapToolScene/CMapToolScene.h"
#include "../../../Utility/Base/CStage.h"
#include "../../../Function/GlobalFunction.h"
#include "../../../Utility/Object/LabelObject/CLabelObject.h"
#include "../../../Utility/Object/TerrainObject/CTerrainObject.h"
#include "../../../Utility/Object/StaticObject/CStaticObject.h"
#include "../../../Utility/Object/LightObject/CLightObject.h"
#include "../../../Utility/Object/LightObject/SpotLightObject/SpotLightObject.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Default.h"
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
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_ScrollBar.h"
#include "../../../GameComposition/GameCharactor/Player/player.h"

CMainPlayScene::CMainPlayScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CMainPlayScene::~CMainPlayScene()
{
	SAFE_DELETE(m_pPlayTime);
	SAFE_DELETE(m_pCamCoderView);
	SAFE_DELETE(pPlayer);

	this->releaseUI();
}

void CMainPlayScene::init()
{	
	CScene::init();
	ShowCursor(false);
	
	if (m_bIsFirst)
	{
		crashFptr = new std::function<void(void)>;
		beginFptr = new std::function<void(void)>;
		pressFptr = new std::function<void(void)>;
		endFptr = new std::function<void(void)>;

		this->createWindowUI();
		this->createRenderTarget();
		pPlayer = new player;
		pPlayer->init();
		this->createContainer();
		this->createSpriteDefault();
		this->createLabel();

		m_bIsFirst = false;
	}

	CMapToolScene* pMapToolScene = dynamic_cast<CMapToolScene*>(FIND_SCENE(GAMESCENE_MAPTOOL));
	pMapToolScene->init();
	m_pStage = pMapToolScene->getStage();

	ppSpotLightObj = new CSpotLightObject*[10];
	ppPointLightObj = new CLightObject*[10];

	CTerrainObject::STParameters stParameters;
	stParameters.m_pCamera = pPlayer->getCamera();
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
		m_pStage->setCameraObj(pPlayer->getCamera());
		m_pStage->getbIsMaptool() = FALSE;
		m_pStage->load(stParameters, "Resources/Datas/ObjPacket.map");
	}
	else
	{
		m_pStage->setCameraObj(pPlayer->getCamera());
		m_pStage->getbIsMaptool() = FALSE;
		m_pStage->load(stParameters, "");
	}
	m_pStage->setCameraObjMain(pPlayer->getCamera());
	m_pStage->addSpotLightObj(pPlayer->getLightObj());
	m_pStage->getTerrainObj()->getTechniqueName() = "fogTerrain";
	m_pStage->setObjEffectTechname("FogStaticMesh");
	int nNumSpot = ++m_pStage->getTerrainObj()->getSTParameters().m_nNumSpotLight;
	if (nNumSpot < 10)
	{
		m_pStage->getTerrainObj()->getSTParameters().m_pSpotLight[nNumSpot - 1] = pPlayer->getLightObj();
	}
	GET_SOUND_MANAGER()->stopAllEffectSounds();
	m_fPlayTime = 0.0f;

}

void CMainPlayScene::createWindowUI()
{
	this->createButton();
}

void CMainPlayScene::createRenderTarget()
{
	D3DVIEWPORT9 stViewport;
	GET_DEVICE()->GetViewport(&stViewport);
	GET_RENDERTARGET_MANAGER()->addRenderTarget("StageRenderTarget", new CRenderTarget(GET_WINDOW_SIZE().cx, GET_WINDOW_SIZE().cy, &stViewport));
	GET_RENDERTARGET_MANAGER()->addRenderTarget("CamCoderRenderTarget", new CRenderTarget(GET_WINDOW_SIZE().cx, GET_WINDOW_SIZE().cy, &stViewport));
}

void CMainPlayScene::createStageSound()
{
	switch (m_eStageSound)
	{
	case EStageSound::START:
		GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/mainBGM_1.wav", true);
		break;
	case EStageSound::STAGE_1: 
		GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/mainBGM_1.wav", true);
		m_ePlayingBGM = EPlayingBGM::CROW; 
		break;
	case EStageSound::STAGE_2: 
		GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/mainBGM_1.wav", true);
		m_ePlayingBGM = EPlayingBGM::OWL;
		break;
	case EStageSound::STAGE_3:
		GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/mainBGM_2.wav", true); 
		break;
	case EStageSound::STAGE_4:
		GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/mainBGM_2.wav", true);
		m_ePlayingBGM = EPlayingBGM::WIND;
		break;
	case EStageSound::STAGE_5: 
		GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/mainBGM_2.wav", true); 
		m_ePlayingBGM = EPlayingBGM::CRIKET; 
		break;
	case EStageSound::STAGE_6: 
		GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/mainBGM_3.wav", true); 
		break;
	case EStageSound::STAGE_7: 
		GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/mainBGM_3.wav", true);
		m_ePlayingBGM = EPlayingBGM::RAIN; 
		break;
	case EStageSound::STAGE_8: 
		GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/mainBGM_3.wav", true);
		m_ePlayingBGM = EPlayingBGM::FIRE; 
		break;
	case EStageSound::EXIT:
		break;
	case EStageSound::NONE:
		break;
	}

}

void CMainPlayScene::setStateSound()
{
	switch (pPlayer->getPlayerState())
	{
	case (int)EPlayerState::WALKGRASS:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Grass.wav", false);
		if (m_fRunTime > 8.0f) {
			GET_SOUND_MANAGER()->setEffectSoundsVolume(1.0f);
		}
		else if (m_fRunTime > 6.0f) {
			GET_SOUND_MANAGER()->setEffectSoundsVolume(0.95f);
		}
		break;
	case (int)EPlayerState::WALKROCK:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Rock.wav", false);
		GET_SOUND_MANAGER()->setEffectSoundsVolume(0.9f);
		break;
	case (int)EPlayerState::WALKREED:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Reed.wav", false);
		GET_SOUND_MANAGER()->setEffectSoundsVolume(0.9f);
		break;
	case (int)EPlayerState::PICK:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Pick.wav", false);
		break;
	case (int)EPlayerState::SLENDER:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Slenderman.wav", false);
		break;
	case (int)EPlayerState::NONE:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Breathe.wav", false);
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/HeartBeat.wav", false);
		if (m_fRunTime <= 1.0f) {
			GET_SOUND_MANAGER()->setEffectSoundsVolume(0.9f);
		}
		else if (m_fRunTime <= 3.0f) {
			GET_SOUND_MANAGER()->setEffectSoundsVolume(0.95f);
		}
		break;
	}
}

void CMainPlayScene::setBGMSound()
{
	switch (m_ePlayingBGM)
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
	case EPlayingBGM::CROW:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Crow.wav", false);
		break;
	case EPlayingBGM::FIRE:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Fire.wav", false);
		break;
	case EPlayingBGM::OWL:
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Owl.wav", false);
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
	if (pPlayer->getPlayerState() != (int)EPlayerState::NONE)
	{
		m_fRunTime += GET_DELTA_TIME();
		if (m_fRunTime >= 20.0f) m_fRunTime = 20.0f;
	}
	else {
		m_fRunTime -= GET_DELTA_TIME();
		if (m_fRunTime <= 0.0f)
		{
			m_fRunTime = 0.0f;
		}
	}
}

void CMainPlayScene::setVolume()
{
	GET_SOUND_MANAGER()->setBackgroundSoundVolume(m_pScrollBar[0]->getSetValue() / 300);
	GET_SOUND_MANAGER()->setEffectSoundsVolume(m_pScrollBar[1]->getSetValue() / 300);
}

void CMainPlayScene::setPlayState()
{	
}

CSpotLightObject * CMainPlayScene::createSpotObj()
{
	return new CSpotLightObject(0,300.0f,D3DXToRadian(5.0f),D3DXToRadian(15.0f));
}

void CMainPlayScene::createButton()
{
	// 종료 버튼
	// {
	m_pExitButton = new CSpriteObject_Button("Resources/Textures/Scene/MainPlayScene/exit", "png", 100, 50, 2);
	m_pExitButton->setPosition(D3DXVECTOR3(0, -60, 0));
	(*crashFptr) = [=](void)->void
	{
		m_pExitButton->getTextureOffset() = 1;

	};
	(*endFptr) = [=](void)->void
	{
		GET_SOUND_MANAGER()->stopAllEffectSounds();
		CHANGE_SCENE_LOADING(GAMESCENE_TITLE, TRUE);
	};
	m_pExitButton->init(crashFptr, nullptr, nullptr, endFptr, true, m_pExitButton->getPosition());
	// }

	// 옵션 버튼
	// {
	m_pOptionButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/option", "png", 150, 50, 2);
	m_pOptionButton->setPosition(D3DXVECTOR3(0, 60, 0));
	(*crashFptr) = [=](void)->void
	{
		m_pOptionButton->getTextureOffset() = 1;

	};
	(*endFptr) = [=](void)->void
	{
		m_pMenuContainer->setVisible(false);
		m_pExitButton->setVisible(false);
		m_pOptionButton->setVisible(false);

		m_pSoundContainer->setVisible(true);
		m_pScrollBar[0]->setVisible(true);
		m_pScrollBar[1]->setVisible(true);
		m_pBackButton->setVisible(true);
	};
	m_pOptionButton->init(crashFptr, nullptr, nullptr, endFptr, true, m_pOptionButton->getPosition());

	m_pBackButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/back", "png", 200, 100, 2);
	(*crashFptr) = [=](void)->void
	{
		m_pBackButton->getTextureOffset() = 1;
	};

	(*endFptr) = [=](void)->void
	{
		m_pMenuContainer->setVisible(true);
		m_pExitButton->setVisible(true);
		m_pOptionButton->setVisible(true);

		m_pScrollBar[0]->setVisible(false);
		m_pScrollBar[1]->setVisible(false);
		m_pSoundContainer->setVisible(false);
		m_pBackButton->setVisible(false);
	};
	m_pBackButton->init(crashFptr, nullptr, nullptr, endFptr, true, D3DXVECTOR3(260.0f, 230.0f, 0.0f));
	// 사운드 스크롤바
	// {
	m_pScrollBar[0] = new CSpriteObject_ScrollBar("Resources/Textures/Scene/TitleScene/whiteCover", "png", 300, 20, 1);

	m_pScrollBarButton[0] = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/grayCover", "png", 10, 50, 1);
	m_pScrollBarButton[0]->init(nullptr, nullptr, nullptr, nullptr, true);

	m_pScrollBar[0]->init(nullptr, nullptr, nullptr, nullptr, 0, 300, m_pScrollBarButton[0], true, D3DXVECTOR3(30, -70, 0.0f));

	m_pScrollBar[1] = new CSpriteObject_ScrollBar("Resources/Textures/Scene/TitleScene/whiteCover", "png", 300, 20, 1);

	m_pScrollBarButton[1] = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/grayCover", "png", 10, 50, 1);
	m_pScrollBarButton[1]->init(nullptr, nullptr, nullptr, nullptr, true);

	m_pScrollBar[1]->init(nullptr, nullptr, nullptr, nullptr, 0, 300, m_pScrollBarButton[1], true, D3DXVECTOR3(30, 10, 0.0f));
	// }
	// }
}

void CMainPlayScene::releaseUI()
{
	SAFE_DELETE(m_pMenuContainer);
	SAFE_DELETE(m_pSoundContainer);
	SAFE_DELETE(m_pBackButton);
	SAFE_DELETE(m_pOptionButton);
	SAFE_DELETE(m_pScrollBar[0]);
	SAFE_DELETE(m_pScrollBar[1]);
	SAFE_DELETE(m_pScrollBarButton[0]);
	SAFE_DELETE(m_pScrollBarButton[1]);
}

void CMainPlayScene::createLabel()
{
	m_pPlayTime = new CLabelObject("", 20);
	m_pPlayTime->setPosition(D3DXVECTOR3(1220, 75, 0));
}

void CMainPlayScene::createSpriteDefault()
{
	m_pCamCoderView = new CSpriteObject_Default("Resources/Textures/Scene/MainPlayScene/camCoderView", "png", 1366, 768, 1);
	m_pCamCoderView->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx/2.0f, GET_WINDOW_SIZE().cy / 2.0f,0.0f));
}

void CMainPlayScene::calcPlayTime(float a_fTime, int & a_nHour, int & a_nMin, int & a_nSec)
{
	int nTime = a_fTime;
	a_nHour = nTime / 3600;
	nTime %= 3600;

	a_nMin = nTime / 60;
	nTime %= 60;

	a_nSec = nTime;
}

void CMainPlayScene::createContainer()
{
	m_pMenuContainer = new CSpriteObject_Container("Resources/Textures/Scene/MainPlayScene/menuWindow", "png", 500, 500, 1);
	m_pMenuContainer->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
	m_pMenuContainer->setVisible(false);
	m_pMenuContainer->init(nullptr, nullptr, nullptr, nullptr);

	m_pMenuContainer->addChildSpriteObject("backTitle", CWindowType::BUTTON, m_pExitButton);
	m_pMenuContainer->addChildSpriteObject("option", CWindowType::BUTTON, m_pOptionButton);

	m_pSoundContainer = new CSpriteObject_Container("Resources/Textures/Scene/TitleScene/optionWindow", "png", 800, 640, 1);
	m_pSoundContainer->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
	m_pSoundContainer->setVisible(false);
	m_pSoundContainer->init(nullptr, nullptr, nullptr, nullptr);

	m_pSoundContainer->addChildSpriteObject("backMenu", CWindowType::BUTTON, m_pBackButton);
	m_pSoundContainer->addChildSpriteObject("BGM", CWindowType::SCROLLBAR, m_pScrollBar[0]);
	m_pSoundContainer->addChildSpriteObject("Effect", CWindowType::SCROLLBAR, m_pScrollBar[1]);
}

void CMainPlayScene::update(void)
{
	CScene::update();

	m_pStage->update();
	m_pCamCoderView->update();
	m_pMenuContainer->update();
	m_pSoundContainer->update();
	setTimer();
	this->setStateSound();
	this->setBGMSound();
	this->setVolume();
	pPlayer->update();
	if (m_bIsBGMPlay)
	{
		this->createStageSound();
		m_bIsBGMPlay = false;
	}
	if (IS_KEY_PRESSED(DIK_ESCAPE)) {
		m_pMenuContainer->setVisible(!m_pMenuContainer->getVisible());
		ShowCursor(m_pMenuContainer->getVisible());
	}

	m_fPlayTime += GET_DELTA_TIME();
}

void CMainPlayScene::draw(void)
{
	CScene::draw();
	D3DXMATRIXA16 stWorldMatrix;
	D3DXMatrixIdentity(&stWorldMatrix);

	/***************************************************/
	//StageRenderTarget에 draw
	/***************************************************/
	GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pTexSurf);
	GET_DEVICE()->SetDepthStencilSurface(FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pDepthStencil);
	GET_DEVICE()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0.0f);
	m_pStage->draw();
	pPlayer->draw();
	/***************************************************/
	//CamCoderRenderTarget에 draw
	/***************************************************/
	GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("CamCoderRenderTarget")->m_stRenderTarget.m_pTexSurf);
	GET_DEVICE()->SetDepthStencilSurface(FIND_RENDERTARGET("CamCoderRenderTarget")->m_stRenderTarget.m_pDepthStencil);
	GET_DEVICE()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0.0f);

	m_pCamCoderView->drawUI();

	char str[100];
	int nHour = 0, nMin = 0, nSec = 0;
	this->calcPlayTime(m_fPlayTime, nHour, nMin, nSec);

	sprintf(str, "%02d:%02d:%02d", nHour, nMin, nSec);
	m_pPlayTime->setString(str);
	m_pPlayTime->drawUI();


	FIND_RENDERTARGET("StageRenderTarget")->m_pCopyEffect->SetMatrix("g_stWorldMatrix",&stWorldMatrix);
	FIND_RENDERTARGET("StageRenderTarget")->m_pCopyEffect->SetTexture("g_pTexture",FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pTex);

	RunEffectLoop(FIND_RENDERTARGET("StageRenderTarget")->m_pCopyEffect, "CopyTexture", [=](int nPassNum)->void {
		FIND_RENDERTARGET("StageRenderTarget")->getPlaneMesh()->DrawSubset(0);
	});


	/***************************************************/
	//Back Buffer로 재설정
	/***************************************************/
	GET_RENDERTARGET_MANAGER()->resetRenderTarget();

	FIND_RENDERTARGET("CamCoderRenderTarget")->m_pCopyEffect->SetMatrix("g_stWorldMatrix", &stWorldMatrix);
	FIND_RENDERTARGET("CamCoderRenderTarget")->m_pCopyEffect->SetTexture("g_pTexture", FIND_RENDERTARGET("CamCoderRenderTarget")->m_stRenderTarget.m_pTex);

	RunEffectLoop(FIND_RENDERTARGET("CamCoderRenderTarget")->m_pCopyEffect, "CopyTexture", [=](int nPassNum)->void {
		FIND_RENDERTARGET("CamCoderRenderTarget")->getPlaneMesh()->DrawSubset(0);
	});


}

void CMainPlayScene::drawUI(void)
{
	CScene::drawUI();
	m_pMenuContainer->drawUI();
	m_pSoundContainer->drawUI();
}

LRESULT CMainPlayScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{
		
	}
	return S_OK;
}
