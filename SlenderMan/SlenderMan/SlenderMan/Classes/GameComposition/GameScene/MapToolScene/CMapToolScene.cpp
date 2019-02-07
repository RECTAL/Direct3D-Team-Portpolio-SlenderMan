#include "CMapToolScene.h"

#include "../../../Utility/Base/CStage.h"
#include "../../../Utility/Manager/CTimeManager.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CInputManager.h"
#include "../../../Utility/Object/CameraObject/CCameraObject.h"
#include "../../../Utility/Object/LightObject/CLightObject.h"
#include "../../../Utility/Object/LightObject/SpotLightObject/SpotLightObject.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Button.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_ScrollBar.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_List.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Container.h"
#include "../../../Utility/Manager/CSceneManager.h"

CMapToolScene::CMapToolScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CMapToolScene::~CMapToolScene()
{
	SAFE_DELETE(scrollBarButton);
	SAFE_DELETE(UpDownScrollBar);

	SAFE_DELETE(openButton);
	SAFE_DELETE(closeButton);
	SAFE_DELETE(buildingButton);
	SAFE_DELETE(terrainButton);
	SAFE_DELETE(goTitleButton);

	if (backButton != nullptr)
	{
		SAFE_DELETE(backButton);
	}
		for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE(m_pSpriteListButton[i]);
	}
	SAFE_DELETE(m_pSpriteList);
	SAFE_DELETE(selectWindowContainer);

	SAFE_DELETE(m_pStage);
	SAFE_DELETE(m_pCamera);
}

void CMapToolScene::init()
{
	CScene::init();
	if (isFirst)
	{
		crashFptr = new std::function<void(void)>;
		beginFptr = new std::function<void(void)>;
		pressFptr = new std::function<void(void)>;
		endFptr = new std::function<void(void)>;


		this->createCameraObj();
		this->createStage();
		this->createWindowUI();
		this->createButtonUI();

		isFirst = false;
	}
}

void CMapToolScene::createWindowUI()
{
	/***************************************************/
	//컨테이너 만들기
	/***************************************************/
	selectWindowContainer = new CSpriteObject_Container("Resources/Textures/Scene/MapToolScene/blackCover", "png", 150, 500, 1);
	selectWindowContainer->setPosition(D3DXVECTOR3(75, GET_WINDOW_SIZE().cy / 2 - 50, 0.0f));
	selectWindowContainer->setVisible(false);
	selectWindowContainer->init(nullptr, nullptr, nullptr, nullptr);

	upCover = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/blackCover", "png", 100, 100, 1);
	upCover->init(nullptr, nullptr, nullptr, nullptr,true,D3DXVECTOR3(0, -190, 0));

	downCover = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/blackCover", "png", 150, 140, 1);
	downCover->init(nullptr, nullptr, nullptr, nullptr, true, D3DXVECTOR3(0, 350, 0));
		/***************************************************/
		//스크롤 바 만들기
		/***************************************************/
		UpDownScrollBar = new CSpriteObject_ScrollBar("Resources/Textures/Scene/MapToolScene/scrollBarBack", "png", 20, 400, 1);
		UpDownScrollBar->setPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			/**********************/
			//스크롤 바 버튼 만들기
			/**********************/
			scrollBarButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/scrollBarFront", "png", 20, 100, 1);
			scrollBarButton->init(nullptr, nullptr, nullptr, nullptr, true);

			UpDownScrollBar->init(
				nullptr, nullptr, nullptr, nullptr, 0, 400, scrollBarButton, true, D3DXVECTOR3(80, 60, 0.0f)
			);


		/***************************************************/
		//리스트 만들기
		/***************************************************/
		m_pSpriteList = new CSpriteObject_List("Resources/Textures/Scene/MapToolScene/blackCover", "png", 150, 500, 1);
		m_pSpriteList->init(nullptr, nullptr, nullptr, nullptr,true,D3DXVECTOR3(0, 110, 0.0f));
			/*************************/
			//리스트 내 버튼 만들기
			/*************************/
			m_pSpriteListButton[0] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/building", "png", 100, 80, 1);
			m_pSpriteListButton[0]->setPosition(m_pSpriteList->getPosition());
			(*endFptr) = [=](void) -> void
			{
				m_pSpriteList->setVisible(false);
				backButton->setVisible(true);
			};
			m_pSpriteListButton[0]->init(nullptr, nullptr, nullptr, endFptr, true);

			m_pSpriteListButton[1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/building", "png", 100, 80, 1);
			m_pSpriteListButton[1]->setPosition(m_pSpriteList->getPosition());
			m_pSpriteListButton[1]->init(nullptr, nullptr, nullptr, nullptr, true);

			m_pSpriteListButton[2] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/building", "png", 100, 80, 1);
			m_pSpriteListButton[2]->setPosition(m_pSpriteList->getPosition());
			m_pSpriteListButton[2]->init(nullptr, nullptr, nullptr, nullptr, true);

			m_pSpriteListButton[3] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/building", "png", 100, 80, 1);
			m_pSpriteListButton[3]->setPosition(m_pSpriteList->getPosition());
			m_pSpriteListButton[3]->init(nullptr, nullptr, nullptr, nullptr, true);

			m_pSpriteListButton[4] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/building", "png", 100, 80, 1);
			m_pSpriteListButton[4]->setPosition(m_pSpriteList->getPosition());
			m_pSpriteListButton[4]->init(nullptr, nullptr, nullptr, nullptr, true);

			for (int i = 0; i < 5; i++)
			{
					char name[100];
					sprintf(name, "uiButton_%d", i);
					m_pSpriteList->addChildSpriteObject(name, CWindowType::BUTTON, m_pSpriteListButton[i]);
			}

		selectWindowContainer->addChildSpriteObject("ButtonList", CWindowType::LIST, m_pSpriteList);
		selectWindowContainer->addChildSpriteObject("upCover", CWindowType::BUTTON, upCover);
		selectWindowContainer->addChildSpriteObject("downCover", CWindowType::BUTTON, downCover);
		selectWindowContainer->addChildSpriteObject("ScrollBar", CWindowType::SCROLLBAR, UpDownScrollBar);

}

void CMapToolScene::createButtonUI()
{
	goTitleButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/back", "png", 200, 100, 2);
	goTitleButton->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx - 100, GET_WINDOW_SIZE().cy - 50, 0));
	goTitleButton->setVisible(true);
	(*crashFptr) = [=](void) -> void
	{
		goTitleButton->getTextureOffset() = 1;
	};

	(*endFptr) = [=](void) -> void
	{
		CHANGE_SCENE_LOADING(GAMESCENE_TITLE, FALSE);

	};
	goTitleButton->init(crashFptr, nullptr, nullptr, endFptr);

	openButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/open", "png", 75, 75, 2);
	openButton->setPosition(D3DXVECTOR3(0, 0, 0));
	openButton->setVisible(true);

	closeButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/close", "png", 75, 75, 2);
	closeButton->setPosition(D3DXVECTOR3(0, 0, 0));
	closeButton->setVisible(false);

	buildingButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/building", "png", 100, 100, 1);
	buildingButton->setPosition(D3DXVECTOR3(50, 300, 0));
	buildingButton->setVisible(false);

	terrainButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/terrain", "png", 100, 100, 1);
	terrainButton->setPosition(D3DXVECTOR3(50, 450, 0));
	terrainButton->setVisible(false);

	(*crashFptr) = [=](void) -> void
	{
		openButton->getTextureOffset() = 1;
	};
	(*endFptr) = [=](void) -> void
	{
		openButton->setVisible(false);
		goTitleButton->setVisible(false);

		m_pCurrent = openButton;

		selectWindowContainer->setVisible(true);
		closeButton->setVisible(true);
		buildingButton->setVisible(true);
		terrainButton->setVisible(true);
	};
	openButton->init(crashFptr, nullptr, nullptr, endFptr);

	(*crashFptr) = [=](void) -> void
	{
		closeButton->getTextureOffset() = 1;
	};
	(*endFptr) = [=](void) -> void
	{
		openButton->setVisible(true);
		goTitleButton->setVisible(true);

		m_pCurrent = nullptr;

		selectWindowContainer->setVisible(false);
		closeButton->setVisible(false);
		buildingButton->setVisible(false);
		terrainButton->setVisible(false);
	};
	closeButton->init(crashFptr, nullptr, nullptr, endFptr);

	buildingButton->init(nullptr, nullptr, nullptr, nullptr);
	terrainButton->init(nullptr, nullptr, nullptr, nullptr);

	backButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/close", "png", 50, 50, 2);
	backButton->setVisible(false);
	(*endFptr) = [=](void) -> void
	{
		backButton->setVisible(false);

	};
	backButton->init(nullptr, nullptr, nullptr, endFptr);
}

void CMapToolScene::createCameraObj()
{
	m_pCamera = new CCameraObject((float)GET_WINDOW_SIZE().cx / (float)GET_WINDOW_SIZE().cy);
}

void CMapToolScene::createStage()
{
	m_pStage = new CStage();

	CTerrainObject::STParameters stParameters;
	stParameters.m_pCamera = m_pCamera;
	stParameters.m_vfScale = D3DXVECTOR3(1.0f, 0.007f, 1.0f);
	stParameters.m_oHeightFilepath = "Resources/Datas/HeightMap.raw";
	stParameters.m_oSplatFilepath = "Resources/Textures/Terrain/HeightMap.bmp";
	stParameters.m_oEffectFilepath = "Resources/Effects/DefaultTerrain.fx";

	stParameters.m_stMapSize.cx = 257;
	stParameters.m_stMapSize.cy = 257;

	stParameters.m_nSmoothLevel = 1;

	stParameters.m_nNumSpotLight = 0;
	stParameters.m_pSpotLight = NULL;

	stParameters.m_nNumPointLight = 0;
	stParameters.m_pPointLight = NULL;


	for (int i = 0; i < CTerrainObject::MAX_TERRAIN_TEX; ++i) {
		char szFilepath[MAX_PATH] = "";
		sprintf(szFilepath, "Resources/Textures/Terrain/Terrain_%02d.jpg", i + 1);

		stParameters.m_oTextureFilepathList.push_back(szFilepath);
	}

	m_pStage->init(stParameters, "");
	m_pStage->setCameraObj(m_pCamera);
	m_pStage->getbIsMaptool() = TRUE;
}

void CMapToolScene::update(void)
{
	CScene::update();
	selectWindowContainer->update();
	this->buttonUpdate();

	printf("%f\n", UpDownScrollBar->getSetValue());
	m_pCamera->update();
	m_pStage->update();


	m_pSpriteList->getMoveOffset() = D3DXVECTOR3(0, -UpDownScrollBar->getSetValue(), 0);

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
		

		m_pCamera->rotateByYAxis(fDeltaX / 5.0f, false);
	


		

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

	}
}

void CMapToolScene::buttonUpdate()
{
	goTitleButton->update();

	openButton->getWindow()->setAbsolutePosition(
		(D3DXVECTOR3(selectWindowContainer->getWindow()->getAbsolutePosition().x - 30,
			selectWindowContainer->getWindow()->getAbsolutePosition().y - 300, 0)));
	openButton->update();

	closeButton->getWindow()->setAbsolutePosition(
		(D3DXVECTOR3(selectWindowContainer->getWindow()->getAbsolutePosition().x + 40,
			selectWindowContainer->getWindow()->getAbsolutePosition().y - 300, 0)));
	closeButton->update();

	backButton->getWindow()->setAbsolutePosition(
		(D3DXVECTOR3(selectWindowContainer->getWindow()->getAbsolutePosition().x - 50,
			selectWindowContainer->getWindow()->getAbsolutePosition().y + 350, 0)));
	backButton->update();
}

void CMapToolScene::draw(void)
{
	CScene::draw();
	m_pStage->draw();
}

void CMapToolScene::drawUI(void)
{
	CScene::drawUI();
	selectWindowContainer->drawUI();

	this->buttonDrawUI();

}

void CMapToolScene::buttonDrawUI()
{
	goTitleButton->drawUI();
	openButton->drawUI();
	closeButton->drawUI();
	backButton->drawUI();

}

LRESULT CMapToolScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
