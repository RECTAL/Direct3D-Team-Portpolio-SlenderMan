#include "CMapToolScene.h"

#include "../../../Utility/../Function/GlobalFunction.h"
#include "../../../Utility/Base/CStage.h"
#include"../../../Utility/Base/CObject.h"
#include "../../../Utility/Manager/CTimeManager.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CInputManager.h"
#include "../../../Utility/Manager/CDeviceManager.h"
#include "../../../Utility/Object/LabelObject/CLabelObject.h"
#include "../../../Utility/Object/CameraObject/CCameraObject.h"
#include "../../../Utility/Object/LightObject/CLightObject.h"
#include "../../../Utility/Object/LightObject/SpotLightObject/SpotLightObject.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Button.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_ScrollBar.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_List.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Container.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_ListSquare.h"
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
	SAFE_DELETE(squareUpCover);
	SAFE_DELETE(m_pTreeListSquare);
	SAFE_DELETE(m_pBuildingListSquare);
	SAFE_DELETE(m_pObjectListSquare);
	SAFE_DELETE(saveButton);
	SAFE_DELETE(loadButton);

	if (backButton != nullptr)
	{
		SAFE_DELETE(backButton);
	}
	for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE(m_pSpriteListButton[i]);
	}
	

	SAFE_DELETE(m_pScaleLabel);
	SAFE_DELETE(m_pRotateLabel);
	

	SAFE_DELETE(m_pSpriteList);
	SAFE_DELETE(selectWindowContainer);

	SAFE_DELETE(m_pStage);
	SAFE_DELETE(m_pCamera);

	SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
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
		this->createLabel();

		m_stMouseInfo.m_eObjType = EObjType::NONE;
		m_stMouseInfo.m_pRenderObj = (CRenderObject*) new CObject();
		m_stMouseInfo.m_pRenderObj->setScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));
		m_pStage->setCameraObjMain(m_pCamera);

		isFirst = false;
	}
	m_pCamera->setPosition(D3DXVECTOR3(100, 100, 100));
}

void CMapToolScene::createWindowUI()
{
	createTreeButton();
	createBuildingButton();
	createObjectButton();
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
		m_pSpriteList->init(nullptr, nullptr, nullptr, nullptr, true, D3DXVECTOR3(0, 110, 0.0f));
			/*************************/
			//리스트 내 버튼 만들기
			/*************************/
			m_pSpriteListButton[0] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/tree", "png", 100, 80, 1);
			m_pSpriteListButton[0]->setPosition(m_pSpriteList->getPosition());
			(*endFptr) = [=](void) -> void
			{
				m_pTreeListSquare->setVisible(true);
				squareUpCover->setVisible(true);
				backButton->setVisible(true); // 나중에 x버튼으로 바꾸기
			};
			m_pSpriteListButton[0]->init(nullptr, nullptr, nullptr, endFptr, true);

			m_pSpriteListButton[1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/building", "png", 100, 80, 1);
			m_pSpriteListButton[1]->setPosition(m_pSpriteList->getPosition());
			(*endFptr) = [=](void) -> void
			{
				m_pBuildingListSquare->setVisible(true);
				squareUpCover->setVisible(true);
				backButton->setVisible(true); // 나중에 x버튼으로 바꾸기
			};
			m_pSpriteListButton[1]->init(nullptr, nullptr, nullptr, endFptr, true);

			m_pSpriteListButton[2] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/object", "png", 100, 80, 1);
			m_pSpriteListButton[2]->setPosition(m_pSpriteList->getPosition());
			(*endFptr) = [=](void) -> void
			{
				m_pObjectListSquare->setVisible(true);
				squareUpCover->setVisible(true);
				backButton->setVisible(true); // 나중에 x버튼으로 바꾸기
			};
			m_pSpriteListButton[2]->init(nullptr, nullptr, nullptr, endFptr, true);


			for (int i = 0; i < 3; i++)
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
		CHANGE_SCENE_LOADING(GAMESCENE_TITLE, TRUE);

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
		m_pTreeListSquare->setVisible(false);
		m_pObjectListSquare->setVisible(false);
		m_pBuildingListSquare->setVisible(false);
		squareUpCover->setVisible(false);

	};
	backButton->init(nullptr, nullptr, nullptr, endFptr);

	squareUpCover = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/blackCover", "png", 300, 100, 1);
	squareUpCover->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx - 200, 50, 0));
	squareUpCover->setVisible(false);
	squareUpCover->init(nullptr, nullptr, nullptr, nullptr);

	saveButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/save", "png", 50, 50, 1);
	saveButton->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy - 25, 0));
	(*endFptr) = [=](void)->void
	{
		MessageBox(GET_WINDOW_HANDLE(), _T("Save"), "", S_OK);
		m_pStage->save("Resources/Datas/ObjPacket.map");
		MessageBox(GET_WINDOW_HANDLE(), _T("Save Complete"), "", S_OK);
		
	};
	saveButton->init(nullptr, nullptr, nullptr, endFptr);

	loadButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/load", "png", 50, 50, 1);
	loadButton->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2 + 55, GET_WINDOW_SIZE().cy - 25, 0));
	(*endFptr) = [=](void)->void
	{
		MessageBox(GET_WINDOW_HANDLE(), _T("Load"), "", S_OK);

		CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
		CLightObject** ppPointLightObj = new CLightObject*[10];

		CTerrainObject::STParameters stParameters;
		stParameters.m_pCamera = m_pCamera;
		stParameters.m_vfScale = D3DXVECTOR3(1.0f, 0.010f, 1.0f);
		stParameters.m_oHeightFilepath = "Resources/Datas/realterrain.raw";
		stParameters.m_oSplatFilepath = "Resources/Textures/Terrain/SplatMap2.png";
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

		m_pStage->init(stParameters, "");
		m_pStage->setCameraObj(m_pCamera);
		m_pStage->getbIsMaptool() = TRUE;
		m_pStage->load(stParameters, "Resources/Datas/ObjPacket.map");

		MessageBox(GET_WINDOW_HANDLE(), _T("Load Complete"), "", S_OK);
	};
	loadButton->init(nullptr, nullptr, nullptr, endFptr);
}

void CMapToolScene::createCameraObj()
{
	m_pCamera = new CCameraObject((float)GET_WINDOW_SIZE().cx / (float)GET_WINDOW_SIZE().cy);
}

void CMapToolScene::createStage()
{
	m_pStage = new CStage();

	CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
	CLightObject** ppPointLightObj = new CLightObject*[10];

	CTerrainObject::STParameters stParameters;
	stParameters.m_pCamera = m_pCamera;
	stParameters.m_vfScale = D3DXVECTOR3(1.0f, 0.010f, 1.0f);
	stParameters.m_oHeightFilepath = "Resources/Datas/realterrain.raw";
	stParameters.m_oSplatFilepath = "Resources/Textures/Terrain/SplatMap2.png";
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

	m_pStage->init(stParameters, "");
	m_pStage->setCameraObj(m_pCamera);
	m_pStage->getbIsMaptool() = TRUE;
}

void CMapToolScene::createLabel()
{
	m_pScaleLabel = new CLabelObject("", 20);
	m_pRotateLabel = new CLabelObject("", 20);
}

void CMapToolScene::update(void)
{
	CScene::update();
	selectWindowContainer->update();
	m_pTreeListSquare->update();
	m_pBuildingListSquare->update();
	m_pObjectListSquare->update();
	this->buttonUpdate();
	this->labelUpdate();

	printf("%f\n", UpDownScrollBar->getSetValue());
	m_pCamera->update();
	m_pStage->update();
	
	inputKey();

	m_pSpriteList->getMoveOffset() = D3DXVECTOR3(0, -UpDownScrollBar->getSetValue(), 0);
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

	squareUpCover->update();
	saveButton->update();
	loadButton->update();
}

void CMapToolScene::labelUpdate()
{
	m_pScaleLabel->setPosition(D3DXVECTOR3(100, 10, 0));
	m_pRotateLabel->setPosition(D3DXVECTOR3(500, 10, 0));

	m_pScaleLabel->update();
	m_pRotateLabel->update();
}

void CMapToolScene::createTreeButton(void)
{
	m_pTreeListSquare = new CSpriteObject_ListSquare("Resources/Textures/Scene/MapToolScene/blackCover", "png", 300, 300, 1);
	m_pTreeListSquare->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx - 200, 250, 0));
	m_pTreeListSquare->setVisible(false);
	m_pTreeListSquare->init(nullptr, nullptr, nullptr, nullptr);

	m_pTreeButton[0] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/tree1", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_bIsSkinned = false;
		m_stMouseInfo.m_eObjType = EObjType::TREE_1;
	};
	m_pTreeButton[0]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pTreeButton[1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/tree2", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_bIsSkinned = false;
		m_stMouseInfo.m_eObjType = EObjType::TREE_2;
	};
	m_pTreeButton[1]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pTreeButton[2] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/tree3", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_bIsSkinned = false;
		m_stMouseInfo.m_eObjType = EObjType::TREE_3;
	};
	m_pTreeButton[2]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pTreeButton[3] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/tree4", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_bIsSkinned = false;
		m_stMouseInfo.m_eObjType = EObjType::TREE_4;
	};
	m_pTreeButton[3]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pTreeButton[4] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/tree5", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_bIsSkinned = false;
		m_stMouseInfo.m_eObjType = EObjType::TREE_5;
	};
	m_pTreeButton[4]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pTreeButton[5] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/tree6", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_bIsSkinned = false;
		m_stMouseInfo.m_eObjType = EObjType::TREE_6;
	};
	m_pTreeButton[5]->init(nullptr, nullptr, nullptr, endFptr, true);

	for (int i = 0; i < MAX_TREE; i++)
	{
		char path[MAX_PATH];
		sprintf(path, "test%1d", i);
		m_pTreeListSquare->addChildSpriteObject(path, CWindowType::BUTTON, m_pTreeButton[i]);
	}
}

void CMapToolScene::createBuildingButton(void)
{
	m_pBuildingListSquare = new CSpriteObject_ListSquare("Resources/Textures/Scene/MapToolScene/blackCover", "png", 300, 300, 1);
	m_pBuildingListSquare->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx - 200, 250, 0));
	m_pBuildingListSquare->setVisible(false);
	m_pBuildingListSquare->init(nullptr, nullptr, nullptr, nullptr);

	m_pBuildingButton[0] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/oldHouse", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_bIsSkinned = false;
		m_stMouseInfo.m_eObjType = EObjType::OLDHOUSE;
	};
	m_pBuildingButton[0]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pBuildingButton[1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/woodHouse", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_bIsSkinned = false;
		m_stMouseInfo.m_eObjType = EObjType::WOODHOUSE;
	};
	m_pBuildingButton[1]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pBuildingButton[2] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/oldWoodDock", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_bIsSkinned = false;
		m_stMouseInfo.m_eObjType = EObjType::OLDWOODDOCK;
	};
	m_pBuildingButton[2]->init(nullptr, nullptr, nullptr, endFptr, true);

	for (int i = 0; i < MAX_BUILDING; i++)
	{
		char path[MAX_PATH];
		sprintf(path, "test%1d", i);
		m_pBuildingListSquare->addChildSpriteObject(path, CWindowType::BUTTON, m_pBuildingButton[i]);
	}

}

void CMapToolScene::createObjectButton(void)
{
	m_pObjectListSquare = new CSpriteObject_ListSquare("Resources/Textures/Scene/MapToolScene/blackCover", "png", 300, 300, 1);
	m_pObjectListSquare->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx - 200, 250, 0));
	m_pObjectListSquare->setVisible(false);
	m_pObjectListSquare->init(nullptr, nullptr, nullptr, nullptr);

	m_pObjectButton[0] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/urbanDebris", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_bIsSkinned = false;
		m_stMouseInfo.m_eObjType = EObjType::URBANDEBRIS;
	};
	m_pObjectButton[0]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pObjectButton[1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/plants", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_bIsSkinned = false;
		m_stMouseInfo.m_eObjType = EObjType::PLANTS;
	};
	m_pObjectButton[1]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pObjectButton[2] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/roundWood", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_bIsSkinned = false;
		m_stMouseInfo.m_eObjType = EObjType::ROUNDWOOD;
	};
	m_pObjectButton[2]->init(nullptr, nullptr, nullptr, endFptr, true);

	for (int i = 0; i < MAX_BUILDING; i++)
	{
		char path[MAX_PATH];
		sprintf(path, "test%1d", i);
		m_pObjectListSquare->addChildSpriteObject(path, CWindowType::BUTTON, m_pObjectButton[i]);
	}
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
	m_pTreeListSquare->drawUI();
	m_pBuildingListSquare->drawUI();
	m_pObjectListSquare->drawUI();

	this->buttonDrawUI();
	this->labelDrawUI();

}

void CMapToolScene::buttonDrawUI()
{
	goTitleButton->drawUI();
	openButton->drawUI();
	closeButton->drawUI();
	backButton->drawUI();
	squareUpCover->drawUI();
	saveButton->drawUI();
	loadButton->drawUI();
}

void CMapToolScene::labelDrawUI()
{
	char cScaleValue[100];
	sprintf(cScaleValue, "scale X:%06.2f ,scale Y:%06.2f ,scale Z:%06.2f", m_stMouseInfo.m_pRenderObj->getScale().x, m_stMouseInfo.m_pRenderObj->getScale().y, m_stMouseInfo.m_pRenderObj->getScale().z);
	m_pScaleLabel->setString(cScaleValue);
	m_pScaleLabel->drawUI();


	char cRotateValue[300];
	sprintf(cRotateValue, "RightDirection X:%01.2f ,Y:%01.2f ,Z:%01.2f \n UpDirection X:%01.2f ,Y:%01.2f ,Z:%01.2f \n ForwardDirection X:%01.2f ,Y:%01.2f ,Z:%01.2f"
		, m_stMouseInfo.m_pRenderObj->getRightDirection().x, m_stMouseInfo.m_pRenderObj->getRightDirection().y, m_stMouseInfo.m_pRenderObj->getRightDirection().z
		, m_stMouseInfo.m_pRenderObj->getUpDirection().x, m_stMouseInfo.m_pRenderObj->getUpDirection().y, m_stMouseInfo.m_pRenderObj->getUpDirection().z
		, m_stMouseInfo.m_pRenderObj->getForwardDirection().x, m_stMouseInfo.m_pRenderObj->getForwardDirection().y, m_stMouseInfo.m_pRenderObj->getForwardDirection().z);
	
	m_pRotateLabel->setString(cRotateValue);
	m_pRotateLabel->drawUI();

}

void CMapToolScene::inputKey(void)
{
	if (IS_KEY_PRESSED(DIK_F1))
	{
		isEnableClick = !isEnableClick;
	}

	if (isEnableClick)
	{
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

		if (IS_MOUSE_BUTTON_PRESSED(EMouseInput::LEFT))
		{
			if (IS_KEY_DOWN(DIK_LCONTROL))
			{
				D3DXVECTOR3 stPos;
				if (m_pStage->getPickingPosWithTerrain(stPos))
				{
					STRay	ray;
					LPDWORD			pIndices = nullptr;
					LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
					m_pStage->getTerrainObj()->getTerrainMesh()->GetIndexBuffer(&pIndexBuffer);

					ray = CreateRay(GET_MOUSE_POSITION());

					int width = m_pStage->getTerrainObj()->getCXDIB();
					int height = m_pStage->getTerrainObj()->getCZDIB();
					int nTriangles = m_pStage->getTerrainObj()->getTriangles();

					if (SUCCEEDED(pIndexBuffer->Lock(0, (width - 1)*(height - 1) * 2 * sizeof(DWORD) * 3, (void**)&pIndices, 0)))
					{
						float fMinLength = 100000.0f;
						std::vector<CRenderObject*> oRenderObjList;
						CRenderObject* pRenderObj = nullptr;
						for (int i = 0; i < nTriangles * 3; i += 3)
						{
							for (auto iter : m_pStage->getObjList()[pIndices[i]])
							{
								if (IsCreshWithBoundingSphere(ray, iter->getFinalBoundingSphere()))
								{
									oRenderObjList.push_back(iter);
								}
							}
						}
						for (auto iter : oRenderObjList)
						{
							D3DXVECTOR3 vec = iter->getPosition() - ray.m_stOrigin;
							float flength = D3DXVec3Length(&vec);
							if (fMinLength > flength)
							{
								fMinLength = flength;
								pRenderObj = iter;
							}
						}

						if (pRenderObj != nullptr)
							m_pStage->delObj(pRenderObj, pRenderObj->getPosition());
						pIndexBuffer->Unlock();
					}
				}
			}
			else
			{
				D3DXVECTOR3 stPos;

				if (m_pStage->getPickingPosWithTerrain(stPos))
				{
					CStage::OBJPACKET objPacket;
					objPacket.m_bIsSkinned = m_stMouseInfo.m_bIsSkinned;
					objPacket.m_EObjType = m_stMouseInfo.m_eObjType;
					objPacket.m_stPosition = stPos;
					objPacket.m_stScale = m_stMouseInfo.m_pRenderObj->getScale();
					objPacket.m_stForwordVec = m_stMouseInfo.m_pRenderObj->getForwardDirection();
					objPacket.m_stUpVec = m_stMouseInfo.m_pRenderObj->getUpDirection();
					objPacket.m_stRightVec = m_stMouseInfo.m_pRenderObj->getRightDirection();

					m_pStage->addObj(objPacket, stPos);
				}
			}
		}

		if (IS_KEY_DOWN(DIK_Q)) {
			m_fScale += 0.01f * GET_DELTA_TIME();
			m_stMouseInfo.m_pRenderObj->setScale(D3DXVECTOR3(m_fScale, m_fScale, m_fScale));
		}
		else if (IS_KEY_DOWN(DIK_E)) {
			m_fScale -= 0.01f * GET_DELTA_TIME();
			m_stMouseInfo.m_pRenderObj->setScale(D3DXVECTOR3(m_fScale, m_fScale, m_fScale));
		}

		if (IS_KEY_PRESSED(DIK_LEFT))		m_fAngleX += 30.0f * GET_DELTA_TIME();
		else if (IS_KEY_PRESSED(DIK_RIGHT)) m_fAngleX -= 30.0f * GET_DELTA_TIME();
		else if (IS_KEY_PRESSED(DIK_UP))	m_fAngleY += 30.0f * GET_DELTA_TIME();
		else if (IS_KEY_PRESSED(DIK_DOWN))	m_fAngleY -= 30.0f * GET_DELTA_TIME();

		m_stMouseInfo.m_pRenderObj->setRotation(D3DXVECTOR3(m_fAngleX, m_fAngleY, m_fAngleZ));
	}
}

LRESULT CMapToolScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
