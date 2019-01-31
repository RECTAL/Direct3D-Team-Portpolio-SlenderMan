#include "CMapToolScene.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Button.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_ScrollBar.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_List.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Default.h"

CMapToolScene::CMapToolScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CMapToolScene::~CMapToolScene()
{
	SAFE_DELETE(m_pSpriteButton);
	SAFE_DELETE(m_pSpriteScrollBar);
	SAFE_DELETE(openButton);
	SAFE_DELETE(closeButton);
	SAFE_DELETE(selectionWindow);
	SAFE_DELETE(buildingButton);
	SAFE_DELETE(terrainButton);

	for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE(m_pSpriteListButton[i]);
	}
	SAFE_DELETE(m_pSpriteList);
}

void CMapToolScene::init()
{
	CScene::init();

	crashFptr = new std::function<void(void)>;
	beginFptr = new std::function<void(void)>;
	pressFptr = new std::function<void(void)>;
	endFptr = new std::function<void(void)>;


	this->createButtonUI();
	this->createWindowUI();
}

void CMapToolScene::createWindowUI()
{
	selectionWindow = new CSpriteObject_Default("Resources/Textures/Scene/MapToolScene/mapToolWindow", "png", 300, 700, 1);
	selectionWindow->setPosition(D3DXVECTOR3(0.0f, GET_WINDOW_SIZE().cy / 2, 0));
	selectionWindow->setVisible(false);
	/***************************************************/
	//스크롤 바 만들기
	/***************************************************/
	m_pSpriteScrollBar = new CSpriteObject_ScrollBar("Resources/Textures/Scene/MapToolScene/scrollBarEX", "png", 50, 300, 1);
	m_pSpriteScrollBar->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0.0f));
		/**********************/
		//스크롤 바 버튼 만들기
		/**********************/
		m_pSpriteButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/buttonEx", "png", 70, 30, 1);
		m_pSpriteButton->setPosition(D3DXVECTOR3(m_pSpriteScrollBar->getPosition().x, m_pSpriteScrollBar->getPosition().y - 150, 0.0f));
		m_pSpriteButton->init(nullptr, nullptr, nullptr, nullptr, true);

		m_pSpriteScrollBar->init(
			nullptr, nullptr, nullptr, nullptr, 0, 300, m_pSpriteButton
		);

	/***************************************************/
	//리스트 만들기
	/***************************************************/
	m_pSpriteList = new CSpriteObject_List("Resources/Textures/Scene/MapToolScene/mapToolWindow", "png", 300, 310, 1);
	m_pSpriteList->setPosition(D3DXVECTOR3(300.0f, 200.0f, 0.0f));
	m_pSpriteList->init(nullptr, nullptr, nullptr, nullptr);
		/*************************/
		//리스트 내 버튼 만들기
		/*************************/
		for (int i = 0; i < 5; i++)
		{
			m_pSpriteListButton[i] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/terrain", "png", 100, 100, 1);
			m_pSpriteListButton[i]->setPosition(m_pSpriteList->getPosition());
			m_pSpriteListButton[i]->init(nullptr, nullptr, nullptr, nullptr, true);
	
			char name[100];
			sprintf(name, "uiButton_%d", i);
			m_pSpriteList->addChildSpriteObject(name, CWindowType::BUTTON, m_pSpriteListButton[i]);
		}

}

void CMapToolScene::createButtonUI()
{
	openButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/open", "png", 100, 100, 2);
	openButton->setPosition(D3DXVECTOR3(50, GET_WINDOW_SIZE().cy / 2, 0));
	openButton->setVisible(true);

	closeButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/close", "png", 100, 100, 2);
	closeButton->setPosition(D3DXVECTOR3(200, GET_WINDOW_SIZE().cy / 2, 0));
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

		selectionWindow->setVisible(true);
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

		selectionWindow->setVisible(false);
		closeButton->setVisible(false);
		buildingButton->setVisible(false);
		terrainButton->setVisible(false);
	};
	closeButton->init(crashFptr, nullptr, nullptr, endFptr);

	buildingButton->init(nullptr, nullptr, nullptr, nullptr);
	terrainButton->init(nullptr, nullptr, nullptr, nullptr);
	
}

void CMapToolScene::update(void)
{
	CScene::update();
	selectionWindow->update();

	m_pSpriteScrollBar->update();
	m_pSpriteList->update();
	this->buttonUpdate();


	m_pSpriteList->getMoveOffset() = D3DXVECTOR3(0.0f, -(290 - m_pSpriteScrollBar->getSetValue()), 0.0f);
	printf("%f\n", m_pSpriteScrollBar->getSetValue());
}

void CMapToolScene::buttonUpdate()
{
	openButton->update();
	closeButton->update();
	buildingButton->update();
	terrainButton->update();
}

void CMapToolScene::draw(void)
{
	CScene::draw();
}

void CMapToolScene::drawUI(void)
{
	CScene::drawUI();
	selectionWindow->drawUI();

	m_pSpriteScrollBar->drawUI();

	m_pSpriteList->drawUI();
	this->buttonDrawUI();
}

void CMapToolScene::buttonDrawUI()
{
	openButton->drawUI();
	closeButton->drawUI();
	buildingButton->drawUI();
	terrainButton->drawUI();
}

LRESULT CMapToolScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
