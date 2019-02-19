#include "CMapToolScene.h"

#include "../../GameObject/Decorate/CDecorate_SoundObj.h"
#include "../../GameObject/Decorate/CDecorate_BillboardObj.h"
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
	SAFE_DELETE(m_pStage);
	SAFE_DELETE(m_pCamera);

	SAFE_DELETE(m_stMouseInfo.m_pRenderObj);

	this->removeUI();
	this->removeList();
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
		this->createLabel();
		this->createCheckBoxButton();

		m_stMouseInfo.m_eObjType = EObjType::NONE;
		m_stMouseInfo.m_pRenderObj = (CRenderObject*) new CObject();
		m_stMouseInfo.m_pRenderObj->setScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));
		m_pStage->setCameraObjMain(m_pCamera);

		isFirst = false;
	}

	m_bIsDebug = false;

	m_pCamera->setPosition(D3DXVECTOR3(100, 100, 100));
	m_pStage->setCameraObjMain(m_pCamera);
	m_pStage->getTerrainObj()->getSTParameters().m_nNumSpotLight = max(m_pStage->getTerrainObj()->getSTParameters().m_nNumSpotLight - 1, 0);
	int nNumSpot = m_pStage->getTerrainObj()->getSTParameters().m_nNumSpotLight;
	m_pStage->getTerrainObj()->getSTParameters().m_pSpotLight[nNumSpot] = nullptr;
	m_pStage->setObjDebugMode(m_bIsDebug, EDebugDrawType::BOX);
	m_pStage->delSpotLightObj();
	m_pStage->getbIsMaptool() = TRUE;
}

void CMapToolScene::createWindowUI()
{
	/***************************************************/
	//컨테이너 만들기
	/***************************************************/

	m_pSelectWindowContainer = new CSpriteObject_Container("Resources/Textures/Scene/MapToolScene/blackCover", "png", 150, 500, 1);
	m_pSelectWindowContainer->setPosition(D3DXVECTOR3(75, GET_WINDOW_SIZE().cy / 2 - 50, 0.0f));
	m_pSelectWindowContainer->setVisible(false);
	m_pSelectWindowContainer->init(nullptr, nullptr, nullptr, nullptr);

	m_pUpCover = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/blackCover", "png", 100, 100, 1);
	m_pUpCover->init(nullptr, nullptr, nullptr, nullptr, true, D3DXVECTOR3(0, -190, 0));

	m_pDownCover = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/blackCover", "png", 150, 140, 1);
	m_pDownCover->init(nullptr, nullptr, nullptr, nullptr, true, D3DXVECTOR3(0, 350, 0));
	/***************************************************/
	//스크롤 바 만들기
	/***************************************************/
	m_pUpDownScrollBar = new CSpriteObject_ScrollBar("Resources/Textures/Scene/MapToolScene/scrollBarBack", "png", 20, 400, 1);
	m_pUpDownScrollBar->setPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	/**********************/
	//스크롤 바 버튼 만들기
	/**********************/
	m_pScrollBarButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/scrollBarFront", "png", 20, 100, 1);
	m_pScrollBarButton->init(nullptr, nullptr, nullptr, nullptr, true);

	m_pUpDownScrollBar->init(
		nullptr, nullptr, nullptr, nullptr, 0, 400, m_pScrollBarButton, true, D3DXVECTOR3(80, 60, 0.0f)
	);


	/***************************************************/
	//리스트 만들기
	/***************************************************/
	m_pSpriteList = new CSpriteObject_List("Resources/Textures/Scene/MapToolScene/blackCover", "png", 150, 500, 1);
	m_pSpriteList->init(nullptr, nullptr, nullptr, nullptr, true, D3DXVECTOR3(0, 110, 0.0f));
	/*************************/
	//리스트 내 버튼 만들기
	/*************************/
	// 나무
	// {
	m_pSpriteListButton[0] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListIcon/tree", "png", 100, 80, 1);
	m_pSpriteListButton[0]->setPosition(m_pSpriteList->getPosition());
	(*endFptr) = [=](void) -> void
	{
		m_pTreeListSquare->setVisible(true);
		m_pSquareUpCover->setVisible(true);
		m_pBackButton->setVisible(true);

		m_pBuildingListSquare->setVisible(false);
		m_pObjectListSquare->setVisible(false);
		m_pSoundListSquare->setVisible(false);
		m_pPageListSquare->setVisible(false);
	};
	m_pSpriteListButton[0]->init(nullptr, nullptr, nullptr, endFptr, true);
	// }

	// 집
	// {
	m_pSpriteListButton[1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListIcon/building", "png", 100, 80, 1);
	m_pSpriteListButton[1]->setPosition(m_pSpriteList->getPosition());
	(*endFptr) = [=](void) -> void
	{
		m_pBuildingListSquare->setVisible(true);
		m_pSquareUpCover->setVisible(true);
		m_pBackButton->setVisible(true);

		m_pTreeListSquare->setVisible(false);
		m_pObjectListSquare->setVisible(false);
		m_pSoundListSquare->setVisible(false);
		m_pPageListSquare->setVisible(false);
	};
	m_pSpriteListButton[1]->init(nullptr, nullptr, nullptr, endFptr, true);
	// }

			//오브젝트
			// {
	m_pSpriteListButton[2] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListIcon/object", "png", 100, 80, 1);
	m_pSpriteListButton[2]->setPosition(m_pSpriteList->getPosition());
	(*endFptr) = [=](void) -> void
	{
		m_pObjectListSquare->setVisible(true);
		m_pSquareUpCover->setVisible(true);
		m_pBackButton->setVisible(true);

		m_pBuildingListSquare->setVisible(false);
		m_pTreeListSquare->setVisible(false);
		m_pSoundListSquare->setVisible(false);
		m_pPageListSquare->setVisible(false);
	};
	m_pSpriteListButton[2]->init(nullptr, nullptr, nullptr, endFptr, true);
	// }

	//사운드
	// {
	m_pSpriteListButton[3] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListIcon/sound", "png", 100, 80, 1);
	m_pSpriteListButton[3]->setPosition(m_pSpriteList->getPosition());
	(*endFptr) = [=](void) -> void
	{
		m_pSoundListSquare->setVisible(true);
		m_pSquareUpCover->setVisible(true);
		m_pBackButton->setVisible(true);

		m_pBuildingListSquare->setVisible(false);
		m_pTreeListSquare->setVisible(false);
		m_pObjectListSquare->setVisible(false);
		m_pPageListSquare->setVisible(false);
	};
	m_pSpriteListButton[3]->init(nullptr, nullptr, nullptr, endFptr, true);
	// }

	// 페이지
	// {
	m_pSpriteListButton[4] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListIcon/page", "png", 100, 80, 1);
	m_pSpriteListButton[4]->setPosition(m_pSpriteList->getPosition());
	(*endFptr) = [=](void) -> void
	{
		m_pPageListSquare->setVisible(true);
		m_pSquareUpCover->setVisible(true);
		m_pBackButton->setVisible(true);

		m_pBuildingListSquare->setVisible(false);
		m_pTreeListSquare->setVisible(false);
		m_pObjectListSquare->setVisible(false);
		m_pSoundListSquare->setVisible(false);
	};
	m_pSpriteListButton[4]->init(nullptr, nullptr, nullptr, endFptr, true);
	// }

	for (int i = 0; i < MAX_LIST_BUTTON; i++)
	{
		char name[100];
		sprintf(name, "uiButton_%d", i);
		m_pSpriteList->addChildSpriteObject(name, CWindowType::BUTTON, m_pSpriteListButton[i]);
	}

	m_pSelectWindowContainer->addChildSpriteObject("ButtonList", CWindowType::LIST, m_pSpriteList);
	m_pSelectWindowContainer->addChildSpriteObject("m_pUpCover", CWindowType::BUTTON, m_pUpCover);
	m_pSelectWindowContainer->addChildSpriteObject("m_pDownCover", CWindowType::BUTTON, m_pDownCover);
	m_pSelectWindowContainer->addChildSpriteObject("ScrollBar", CWindowType::SCROLLBAR, m_pUpDownScrollBar);

	this->createButtonUI();
}

void CMapToolScene::createButtonUI()
{
	m_pGoTitleButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ButtonIcon/back", "png", 200, 100, 2);
	m_pGoTitleButton->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx - 100, GET_WINDOW_SIZE().cy - 50, 0));
	m_pGoTitleButton->setVisible(true);
	(*crashFptr) = [=](void) -> void
	{
		m_pGoTitleButton->getTextureOffset() = 1;
	};

	(*endFptr) = [=](void) -> void
	{
		CHANGE_SCENE_LOADING(GAMESCENE_TITLE, TRUE);

	};
	m_pGoTitleButton->init(crashFptr, nullptr, nullptr, endFptr);

	m_pOpenButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ButtonIcon/open", "png", 75, 75, 2);
	m_pOpenButton->setPosition(D3DXVECTOR3(0, 0, 0));
	m_pOpenButton->setVisible(true);

	m_pCloseButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ButtonIcon/close", "png", 75, 75, 2);
	m_pCloseButton->setPosition(D3DXVECTOR3(0, 0, 0));
	m_pCloseButton->setVisible(false);

	(*crashFptr) = [=](void) -> void
	{
		m_pOpenButton->getTextureOffset() = 1;
	};
	(*endFptr) = [=](void) -> void
	{
		m_pOpenButton->setVisible(false);
		m_pGoTitleButton->setVisible(false);
		m_pCollisionButton->setVisible(false);
		m_pDebugButton->setVisible(false);

		m_pCurrent = m_pOpenButton;

		m_pSelectWindowContainer->setVisible(true);
		m_pCloseButton->setVisible(true);
	};
	m_pOpenButton->init(crashFptr, nullptr, nullptr, endFptr);

	(*crashFptr) = [=](void) -> void
	{
		m_pCloseButton->getTextureOffset() = 1;
	};
	(*endFptr) = [=](void) -> void
	{
		m_pOpenButton->setVisible(true);
		m_pGoTitleButton->setVisible(true);
		m_pCollisionButton->setVisible(true);
		m_pDebugButton->setVisible(true);

		m_pCurrent = nullptr;

		m_pSelectWindowContainer->setVisible(false);
		m_pCloseButton->setVisible(false);
		m_pSoundListSquare->setVisible(false);
	};
	m_pCloseButton->init(crashFptr, nullptr, nullptr, endFptr);

	m_pBackButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ButtonIcon/listSquareClose", "png", 100, 100, 1);
	m_pBackButton->setVisible(false);
	(*endFptr) = [=](void) -> void
	{
		m_pBackButton->setVisible(false);
		m_pTreeListSquare->setVisible(false);
		m_pObjectListSquare->setVisible(false);
		m_pBuildingListSquare->setVisible(false);
		m_pSoundListSquare->setVisible(false);
		m_pPageListSquare->setVisible(false);
		m_pSquareUpCover->setVisible(false);
	};
	m_pBackButton->init(nullptr, nullptr, nullptr, endFptr);

	m_pSquareUpCover = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/blackCover", "png", 300, 100, 1);
	m_pSquareUpCover->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx - 200, 50, 0));
	m_pSquareUpCover->setVisible(false);
	m_pSquareUpCover->init(nullptr, nullptr, nullptr, nullptr);

	m_pSaveButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ButtonIcon/save", "png", 50, 50, 1);
	m_pSaveButton->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy - 25, 0));
	(*endFptr) = [=](void)->void
	{
		MessageBox(GET_WINDOW_HANDLE(), _T("Save"), "", S_OK);
		m_pStage->save("Resources/Datas/ObjPacket.map");
		MessageBox(GET_WINDOW_HANDLE(), _T("Save Complete"), "", S_OK);
	};
	m_pSaveButton->init(nullptr, nullptr, nullptr, endFptr);

	m_pLoadButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ButtonIcon/load", "png", 50, 50, 1);
	m_pLoadButton->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2 + 55, GET_WINDOW_SIZE().cy - 25, 0));
	(*endFptr) = [=](void)->void
	{
		MessageBox(GET_WINDOW_HANDLE(), _T("Load"), "", S_OK);

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

		m_pStage->init(stParameters, "");
		m_pStage->setCameraObj(m_pCamera);
		m_pStage->getbIsMaptool() = TRUE;
		m_pStage->load(stParameters, "Resources/Datas/ObjPacket.map");

		MessageBox(GET_WINDOW_HANDLE(), _T("Load Complete"), "", S_OK);
	};
	m_pLoadButton->init(nullptr, nullptr, nullptr, endFptr);

	this->createTreeButton();
	this->createBuildingButton();
	this->createObjectButton();
	this->createSoundButton();
	this->createPageButton();
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

	m_pStage->init(stParameters, "");
	m_pStage->setCameraObj(m_pCamera);
	m_pStage->getbIsMaptool() = TRUE;
}

void CMapToolScene::createLabel()
{
	m_pScaleLabel = new CLabelObject("", 20);
	m_pOffsetLabel = new CLabelObject("", 20);
	m_pRotateLabel = new CLabelObject("", 20);
}

void CMapToolScene::update(void)
{
	CScene::update();
	m_pSelectWindowContainer->update();
	this->listUpdate();
	this->buttonUpdate();
	this->labelUpdate();

	printf("%f\n", m_pUpDownScrollBar->getSetValue());
	m_pCamera->update();
	m_pStage->update();
	this->inputKey();

	m_pSpriteList->getMoveOffset() = D3DXVECTOR3(0, -m_pUpDownScrollBar->getSetValue(), 0);


	D3DXVECTOR3 pos;
	m_stMouseInfo.m_bDraw = false;
	if (m_pStage->getPickingPosWithTerrain(pos))
	{
		m_stMouseInfo.m_bDraw = true;
		if (m_stMouseInfo.m_pRenderObj != nullptr)m_stMouseInfo.m_pRenderObj->setPosition(pos + D3DXVECTOR3(0, m_fOffesetY, 0));
		m_stMouseInfo.m_pRenderObj->update();
	}
}

void CMapToolScene::buttonUpdate()
{
	m_pGoTitleButton->update();

	m_pOpenButton->getWindow()->setAbsolutePosition(
		(D3DXVECTOR3(m_pSelectWindowContainer->getWindow()->getAbsolutePosition().x - 30,
			m_pSelectWindowContainer->getWindow()->getAbsolutePosition().y - 300, 0)));
	m_pOpenButton->update();

	m_pCloseButton->getWindow()->setAbsolutePosition(
		(D3DXVECTOR3(m_pSelectWindowContainer->getWindow()->getAbsolutePosition().x + 40,
			m_pSelectWindowContainer->getWindow()->getAbsolutePosition().y - 300, 0)));
	m_pCloseButton->update();

	m_pBackButton->getWindow()->setAbsolutePosition(
		(D3DXVECTOR3(m_pSelectWindowContainer->getWindow()->getAbsolutePosition().x + GET_WINDOW_SIZE().cx - 175,
			m_pSelectWindowContainer->getWindow()->getAbsolutePosition().y - 275, 0)));
	m_pBackButton->update();

	m_pSquareUpCover->update();
	m_pSaveButton->update();
	m_pLoadButton->update();
	m_pCollisionButton->update();
	m_pDebugButton->update();
	
}

void CMapToolScene::listUpdate()
{
	m_pTreeListSquare->update();
	m_pBuildingListSquare->update();
	m_pObjectListSquare->update();
	m_pSoundListSquare->update();
	m_pPageListSquare->update();
}

void CMapToolScene::labelUpdate()
{
	m_pScaleLabel->setPosition(D3DXVECTOR3(100, 10, 0));
	m_pOffsetLabel->setPosition(D3DXVECTOR3(100, 40, 0));
	m_pRotateLabel->setPosition(D3DXVECTOR3(500, 10, 0));

	m_pScaleLabel->update();
	m_pOffsetLabel->update();
	m_pRotateLabel->update();
}

void CMapToolScene::createTreeButton(void)
{
	m_pTreeListSquare = new CSpriteObject_ListSquare("Resources/Textures/Scene/MapToolScene/blackCover", "png", 300, 300, 1);
	m_pTreeListSquare->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx - 200, 250, 0));
	m_pTreeListSquare->setVisible(false);
	m_pTreeListSquare->init(nullptr, nullptr, nullptr, nullptr);
	
	for (int i = 0; i < MAX_TREE - 1; i++) {
		char cTreeIconIndex[MAX_PATH];
		char cTreeMeshIndex[MAX_PATH];
		sprintf(cTreeIconIndex, "Resources/Textures/Scene/MapToolScene/ListSquareIcon/tree%d", i + 1);
		sprintf(cTreeMeshIndex, "Resources/Meshes/tree%d/tree%d.X", i + 1, i + 1);
		std::string str = cTreeMeshIndex;
		m_pTreeButton[i] = new CSpriteObject_Button(cTreeIconIndex, "png", 100, 100, 1);
		(*endFptr) = [=](void)->void {
			m_stMouseInfo.m_eObjClasses = EObjClasses::STATIC;
			m_stMouseInfo.m_eObjType = (EObjType)((int)EObjType::TREE_1 + i);

			if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
			CStaticObject::STParameters stParameters =
			{
				m_pCamera,m_pStage->getDirectionalLightObj(),
				0,NULL,
				0,NULL,
				str.c_str(),
				"Resources/Effects/DefaultStaticMesh.fx"
			};


			m_stMouseInfo.m_pRenderObj = new CStaticObject(stParameters);
			m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
			m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
		};
		m_pTreeButton[i]->init(nullptr, nullptr, nullptr, endFptr, true);
	}
	m_pTreeButton[MAX_TREE - 1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/none", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::STATIC;
		m_stMouseInfo.m_eObjType = EObjType::NONE;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);

		m_stMouseInfo.m_pRenderObj = (CRenderObject*) new CObject();
		m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
		m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
	};
	m_pTreeButton[MAX_TREE - 1]->init(nullptr, nullptr, nullptr, endFptr, true);

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

	m_pHouseButton[0] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/oldHouse", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::STATIC;
		m_stMouseInfo.m_eObjType = EObjType::OLDHOUSE;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
		CStaticObject::STParameters stParameters =
		{
			m_pCamera,m_pStage->getDirectionalLightObj(),
			0,NULL,
			0,NULL,
			"Resources/Meshes/oldHouse/oldHouse.X",
			"Resources/Effects/DefaultStaticMesh.fx"
		};
		m_stMouseInfo.m_pRenderObj = new CStaticObject(stParameters);
		m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
		m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
	};
	m_pHouseButton[0]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pHouseButton[1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/woodHouse", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::STATIC;
		m_stMouseInfo.m_eObjType = EObjType::WOODHOUSE;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
		CStaticObject::STParameters stParameters =
		{
			m_pCamera,m_pStage->getDirectionalLightObj(),
			0,NULL,
			0,NULL,
			"Resources/Meshes/woodHouse/woodHouse.X",
			"Resources/Effects/DefaultStaticMesh.fx"
		};
		m_stMouseInfo.m_pRenderObj = new CStaticObject(stParameters);
		m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
		m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
	};
	m_pHouseButton[1]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pHouseButton[2] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/oldWoodDock", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::STATIC;
		m_stMouseInfo.m_eObjType = EObjType::OLDWOODDOCK;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
		CStaticObject::STParameters stParameters =
		{
			m_pCamera,m_pStage->getDirectionalLightObj(),
			0,NULL,
			0,NULL,
			"Resources/Meshes/oldWoodDock/oldWoodDock.X",
			"Resources/Effects/DefaultStaticMesh.fx"
		};
		m_stMouseInfo.m_pRenderObj = new CStaticObject(stParameters);
		m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
		m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
	};
	m_pHouseButton[2]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pHouseButton[MAX_HOUSE - 1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/none", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::STATIC;
		m_stMouseInfo.m_eObjType = EObjType::NONE;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
		
		m_stMouseInfo.m_pRenderObj = (CRenderObject*) new CObject();
		m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
		m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
	};
	m_pHouseButton[MAX_HOUSE - 1]->init(nullptr, nullptr, nullptr, endFptr, true);

	for (int i = 0; i < MAX_HOUSE; i++)
	{
		char path[MAX_PATH];
		sprintf(path, "test%1d", i);
		m_pBuildingListSquare->addChildSpriteObject(path, CWindowType::BUTTON, m_pHouseButton[i]);
	}
}

void CMapToolScene::createObjectButton(void)
{
	m_pObjectListSquare = new CSpriteObject_ListSquare("Resources/Textures/Scene/MapToolScene/blackCover", "png", 300, 300, 1);
	m_pObjectListSquare->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx - 200, 250, 0));
	m_pObjectListSquare->setVisible(false);
	m_pObjectListSquare->init(nullptr, nullptr, nullptr, nullptr);

	m_pObjectButton[0] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/urbanDebris", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::STATIC;
		m_stMouseInfo.m_eObjType = EObjType::URBANDEBRIS;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
		CStaticObject::STParameters stParameters =
		{
			m_pCamera,m_pStage->getDirectionalLightObj(),
			0,NULL,
			0,NULL,
			"Resources/Meshes/urbanDebris/urbanDebris.X",
			"Resources/Effects/DefaultStaticMesh.fx"
		};
		m_stMouseInfo.m_pRenderObj = new CStaticObject(stParameters);
		m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
		m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
	};
	m_pObjectButton[0]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pObjectButton[1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/plants", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::STATIC;
		m_stMouseInfo.m_eObjType = EObjType::PLANTS;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
		CStaticObject::STParameters stParameters =
		{
			m_pCamera,m_pStage->getDirectionalLightObj(),
			0,NULL,
			0,NULL,
			"Resources/Meshes/plants/plants.X",
			"Resources/Effects/DefaultStaticMesh.fx"
		};
		m_stMouseInfo.m_pRenderObj = new CStaticObject(stParameters);
		m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
		m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
	};
	m_pObjectButton[1]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pObjectButton[2] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/roundWood", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::STATIC;
		m_stMouseInfo.m_eObjType = EObjType::ROUNDWOOD;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
		CStaticObject::STParameters stParameters =
		{
			m_pCamera,m_pStage->getDirectionalLightObj(),
			0,NULL,
			0,NULL,
			"Resources/Meshes/roundwood/roundwood.X",
			"Resources/Effects/DefaultStaticMesh.fx"
		};
		m_stMouseInfo.m_pRenderObj = new CStaticObject(stParameters);
		m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
		m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
	};
	m_pObjectButton[2]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pObjectButton[MAX_OBJECT - 1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/none", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::STATIC;
		m_stMouseInfo.m_eObjType = EObjType::NONE;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);

		m_stMouseInfo.m_pRenderObj = (CRenderObject*) new CObject();
		m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
		m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
	};
	m_pObjectButton[MAX_OBJECT - 1]->init(nullptr, nullptr, nullptr, endFptr, true);

	for (int i = 0; i < MAX_OBJECT; i++)
	{
		char path[MAX_PATH];
		sprintf(path, "test%1d", i);
		m_pObjectListSquare->addChildSpriteObject(path, CWindowType::BUTTON, m_pObjectButton[i]);
	}
}

void CMapToolScene::createSoundButton(void)
{
	m_pSoundListSquare = new CSpriteObject_ListSquare("Resources/Textures/Scene/MapToolScene/blackCover", "png", 300, 300, 1);
	m_pSoundListSquare->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx - 200, 250, 0));
	m_pSoundListSquare->setVisible(false);
	m_pSoundListSquare->init(nullptr, nullptr, nullptr, nullptr);

	m_pSoundButton[0] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/criket", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::DECORATE_SOUND;
		m_stMouseInfo.m_eObjType = EObjType::CRIKET;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
		CDecorate_SoundObj::STParameters stParameters = {
		m_pCamera,m_pStage->getDirectionalLightObj()
		};

		m_stMouseInfo.m_pRenderObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::CRIKET);
		m_stMouseInfo.m_pRenderObj->setScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	};
	m_pSoundButton[0]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pSoundButton[1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/crow", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::DECORATE_SOUND;
		m_stMouseInfo.m_eObjType = EObjType::CROW;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
		CDecorate_SoundObj::STParameters stParameters = {
		m_pCamera,m_pStage->getDirectionalLightObj()
		};

		m_stMouseInfo.m_pRenderObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::CROW);
		m_stMouseInfo.m_pRenderObj->setScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	};
	m_pSoundButton[1]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pSoundButton[2] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/fire", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::DECORATE_SOUND;
		m_stMouseInfo.m_eObjType = EObjType::FIRE;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
		CDecorate_SoundObj::STParameters stParameters = {
		m_pCamera,m_pStage->getDirectionalLightObj()
		};

		m_stMouseInfo.m_pRenderObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::FIRE);
		m_stMouseInfo.m_pRenderObj->setScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	};
	m_pSoundButton[2]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pSoundButton[3] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/owl", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::DECORATE_SOUND;
		m_stMouseInfo.m_eObjType = EObjType::OWL;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
		CDecorate_SoundObj::STParameters stParameters = {
		m_pCamera,m_pStage->getDirectionalLightObj()
		};

		m_stMouseInfo.m_pRenderObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::OWL);
		m_stMouseInfo.m_pRenderObj->setScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	};
	m_pSoundButton[3]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pSoundButton[4] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/rain", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::DECORATE_SOUND;
		m_stMouseInfo.m_eObjType = EObjType::RAIN;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);

		CDecorate_SoundObj::STParameters stParameters = {
			m_pCamera,m_pStage->getDirectionalLightObj()
		};

		m_stMouseInfo.m_pRenderObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::RAIN);
		m_stMouseInfo.m_pRenderObj->setScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	};
	m_pSoundButton[4]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pSoundButton[5] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/wind", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::DECORATE_SOUND;
		m_stMouseInfo.m_eObjType = EObjType::WIND;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
		CDecorate_SoundObj::STParameters stParameters = {
			m_pCamera,m_pStage->getDirectionalLightObj()
		};

		m_stMouseInfo.m_pRenderObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::WIND);
		m_stMouseInfo.m_pRenderObj->setScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	};
	m_pSoundButton[5]->init(nullptr, nullptr, nullptr, endFptr, true);

	m_pSoundButton[MAX_SOUND - 1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/none", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::DECORATE_SOUND;
		m_stMouseInfo.m_eObjType = EObjType::NONE;
		m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);

		CDecorate_SoundObj::STParameters stParameters = {
			m_pCamera,m_pStage->getDirectionalLightObj()
		};

		m_stMouseInfo.m_pRenderObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::NONE);
		m_stMouseInfo.m_pRenderObj->setScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	};
	m_pSoundButton[MAX_SOUND - 1]->init(nullptr, nullptr, nullptr, endFptr, true);

	for (int i = 0; i < MAX_SOUND; i++)
	{
		char path[MAX_PATH];
		sprintf(path, "test%1d", i);
		m_pSoundListSquare->addChildSpriteObject(path, CWindowType::BUTTON, m_pSoundButton[i]);
	}
}

void CMapToolScene::createPageButton(void)
{
	m_pPageListSquare= new CSpriteObject_ListSquare("Resources/Textures/Scene/MapToolScene/blackCover", "png", 300, 300, 1);
	m_pPageListSquare->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx - 200, 250, 0));
	m_pPageListSquare->setVisible(false);
	m_pPageListSquare->init(nullptr, nullptr, nullptr, nullptr);
	for (int i = 0; i < MAX_PAGE - 1; i++) {
		char cPageIconIndex[MAX_PATH];
		char cPageMeshIndex[MAX_PATH];
		sprintf(cPageIconIndex, "Resources/Textures/Scene/MapToolScene/ListSquareIcon/page%d", i + 1);
		sprintf(cPageMeshIndex, "Resources/Textures/object/page%d", i + 1);
		std::string str = cPageMeshIndex;
		m_pPageButton[i] = new CSpriteObject_Button(cPageIconIndex, "png", 100, 100, 1);
		(*endFptr) = [=](void)->void {
			m_stMouseInfo.m_eObjClasses = EObjClasses::STATIC;
			m_stMouseInfo.m_eObjType = ((EObjType)((int)EObjType::PAGE_1 + i));
			if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);
			CDecorate_BillboardObj::STParameters stParameters =
			{
				m_pCamera,m_pStage->getDirectionalLightObj(),
				0,NULL,
				0,NULL,
				str.c_str(),"png",1,
				"Resources/Effects/DefaultStaticMesh.fx"
			};
			m_stMouseInfo.m_pRenderObj = new CDecorate_BillboardObj(stParameters);
			m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
			m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
		};
		m_pPageButton[i]->init(nullptr, nullptr, nullptr, endFptr, true);
	}

	m_pPageButton[MAX_PAGE - 1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ListSquareIcon/none", "png", 100, 100, 1);
	(*endFptr) = [=](void)->void {
		m_stMouseInfo.m_eObjClasses = EObjClasses::STATIC;
		m_stMouseInfo.m_eObjType = EObjType::NONE;
		if (m_stMouseInfo.m_pRenderObj != nullptr) SAFE_DELETE(m_stMouseInfo.m_pRenderObj);

		m_stMouseInfo.m_pRenderObj = (CRenderObject*) new CObject();
		m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
		m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
	};
	m_pPageButton[MAX_PAGE - 1]->init(nullptr, nullptr, nullptr, endFptr, true);

	for (int i = 0; i < MAX_PAGE; i++)
	{
		char path[MAX_PATH];
		sprintf(path, "test%1d", i);
		m_pPageListSquare->addChildSpriteObject(path, CWindowType::BUTTON, m_pPageButton[i]);
	}

}

void CMapToolScene::createCheckBoxButton(void)
{
	m_pCollisionButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ButtonIcon/collision", "png", 250, 60, 2, true);
	m_pCollisionButton->setPosition(D3DXVECTOR3(125, GET_WINDOW_SIZE().cy - 120, 0));
	(*endFptr) = [=](void) -> void
	{
		if (m_pCollisionButton->getTextureOffset() == 0) {
			m_pCollisionButton->getTextureOffset() = 1;

			m_bIsCollision = true;
			m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
			if (m_stMouseInfo.m_eObjClasses != EObjClasses::DECORATE_SOUND)
			{
				m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
			}
		}
		else {
			m_pCollisionButton->getTextureOffset() = 0;
			m_bIsCollision = false;
			m_stMouseInfo.m_pRenderObj->getbIsCollision() = m_bIsCollision;
			if (m_stMouseInfo.m_eObjClasses != EObjClasses::DECORATE_SOUND)
			{
				m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
			}
		}
	};
	m_pCollisionButton->init(nullptr, nullptr, nullptr, endFptr);

	m_pDebugButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/ButtonIcon/debug", "png", 160, 60, 2, true);
	m_pDebugButton->setPosition(D3DXVECTOR3(80, GET_WINDOW_SIZE().cy - 60, 0));
	(*endFptr) = [=](void) -> void
	{
		if (m_pDebugButton->getTextureOffset() == 0) {
			m_pDebugButton->getTextureOffset() = 1;
		
			m_bIsDebug = true;
			if (m_stMouseInfo.m_eObjClasses != EObjClasses::DECORATE_SOUND)
			{
				m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
			}
			m_pStage->setObjDebugMode(m_bIsDebug, EDebugDrawType::BOX);
		}
		else {
			m_pDebugButton->getTextureOffset() = 0;
			m_bIsDebug = false;
			if (m_stMouseInfo.m_eObjClasses != EObjClasses::DECORATE_SOUND)
			{
				m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
			}
			m_pStage->setObjDebugMode(m_bIsDebug, EDebugDrawType::BOX);
		}
	};
	m_pDebugButton->init(nullptr, nullptr, nullptr, endFptr);


}

void CMapToolScene::draw(void)
{
	CScene::draw();
	m_pStage->draw();
	if (m_stMouseInfo.m_eObjType != EObjType::NONE&&m_stMouseInfo.m_bDraw)
	{
		if(m_stMouseInfo.m_eObjClasses != EObjClasses::DECORATE_SOUND)
			m_stMouseInfo.m_pRenderObj->setDebugEnable(m_bIsDebug, EDebugDrawType::BOX);
		m_stMouseInfo.m_pRenderObj->draw();
	}
}

void CMapToolScene::drawUI(void)
{
	CScene::drawUI();
	
	m_pSelectWindowContainer->drawUI();
	this->buttonDrawUI();
	m_pTreeListSquare->drawUI();
	m_pBuildingListSquare->drawUI();
	m_pObjectListSquare->drawUI();
	m_pSoundListSquare->drawUI();
	m_pPageListSquare->drawUI();

	this->labelDrawUI();
}

void CMapToolScene::buttonDrawUI()
{
	m_pGoTitleButton->drawUI();
	m_pSquareUpCover->drawUI();
	m_pOpenButton->drawUI();
	m_pCloseButton->drawUI();
	m_pBackButton->drawUI();
	m_pSaveButton->drawUI();
	m_pLoadButton->drawUI();
	m_pCollisionButton->drawUI();
	m_pDebugButton->drawUI();
}

void CMapToolScene::labelDrawUI()
{
	char cScaleValue[100];
	sprintf(cScaleValue, "scale X:%06.2f ,scale Y:%06.2f ,scale Z:%06.2f", m_stMouseInfo.m_pRenderObj->getScale().x, m_stMouseInfo.m_pRenderObj->getScale().y, m_stMouseInfo.m_pRenderObj->getScale().z);
	m_pScaleLabel->setString(cScaleValue);
	m_pScaleLabel->drawUI();

	char cOffsetValue[100];
	sprintf(cOffsetValue, "offset Y:%06.2f", m_fOffesetY);
	m_pOffsetLabel->setString(cOffsetValue);
	m_pOffsetLabel->drawUI();

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
								if (IsIntersectRayBox(ray, iter->getFinalBoundingBox()))
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
						oRenderObjList.clear();
						pIndexBuffer->Unlock();
					}
				}
			}
			else
			{
				D3DXVECTOR3 stPos;

				if (m_pStage->getPickingPosWithTerrain(stPos))
				{
					stPos += D3DXVECTOR3(0, m_fOffesetY, 0);

					CStage::OBJPACKET objPacket;
					objPacket.m_bIsCollision = m_bIsCollision;
					objPacket.m_EClasses	 = m_stMouseInfo.m_eObjClasses;
					objPacket.m_EObjType	 = m_stMouseInfo.m_eObjType;
					objPacket.m_stPosition   = stPos;
					objPacket.m_stScale      = m_stMouseInfo.m_pRenderObj->getScale();
					objPacket.m_stForwordVec = m_stMouseInfo.m_pRenderObj->getForwardDirection();
					objPacket.m_stUpVec      = m_stMouseInfo.m_pRenderObj->getUpDirection();
					objPacket.m_stRightVec   = m_stMouseInfo.m_pRenderObj->getRightDirection();

					m_pStage->addObj(objPacket, stPos, m_bIsDebug);
				}
			}
		}

		if (IS_KEY_DOWN(DIK_Q)) {
			m_fScale += 0.1f * GET_DELTA_TIME();
		}
		else if (IS_KEY_DOWN(DIK_E)) {
			m_fScale -= 0.1f * GET_DELTA_TIME();
		}
		if (IS_KEY_DOWN(DIK_Z)) {
			m_fOffesetY += GET_DELTA_TIME() * 10.0f;
		}
		else if (IS_KEY_DOWN(DIK_X)) {
			m_fOffesetY -= GET_DELTA_TIME() * 10.0f;
		}
		
		if (IS_KEY_DOWN(DIK_LEFT))		 m_fAngleY += 30.0f * GET_DELTA_TIME();
		else if (IS_KEY_DOWN(DIK_RIGHT)) m_fAngleY -= 30.0f * GET_DELTA_TIME();
		else if (IS_KEY_DOWN(DIK_UP))	 m_fAngleX += 30.0f * GET_DELTA_TIME();
		else if (IS_KEY_DOWN(DIK_DOWN))	 m_fAngleX -= 30.0f * GET_DELTA_TIME();

	}
	m_stMouseInfo.m_pRenderObj->setRotation(D3DXVECTOR3(m_fAngleX, m_fAngleY, m_fAngleZ));
	m_stMouseInfo.m_pRenderObj->setScale(D3DXVECTOR3(m_fScale, m_fScale, m_fScale));


	
}

void CMapToolScene::removeUI(void)
{
	SAFE_DELETE(m_pUpDownScrollBar);

	SAFE_DELETE(m_pSelectWindowContainer);

	SAFE_DELETE(m_pScrollBarButton);
	SAFE_DELETE(m_pSaveButton);
	SAFE_DELETE(m_pLoadButton);
	SAFE_DELETE(m_pOpenButton);
	SAFE_DELETE(m_pCloseButton);
	SAFE_DELETE(m_pGoTitleButton);
	SAFE_DELETE(m_pCollisionButton);
	SAFE_DELETE(m_pDebugButton);
	SAFE_DELETE(m_pSquareUpCover);

	if (m_pBackButton != nullptr)
	{
		SAFE_DELETE(m_pBackButton);
	}
	for (int i = 0; i < MAX_LIST_BUTTON; i++)
	{
		SAFE_DELETE(m_pSpriteListButton[i]);
	}
}

void CMapToolScene::removeList(void)
{
	SAFE_DELETE(m_pSpriteList);

	SAFE_DELETE(m_pTreeListSquare);
	SAFE_DELETE(m_pBuildingListSquare);
	SAFE_DELETE(m_pObjectListSquare);
	SAFE_DELETE(m_pSoundListSquare);
	SAFE_DELETE(m_pPageListSquare);
}

LRESULT CMapToolScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
