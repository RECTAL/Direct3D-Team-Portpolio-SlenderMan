#include "CMapToolScene.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CInputManager.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Button.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_ScrollBar.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_List.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Container.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Default.h"

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
}

void CMapToolScene::init()
{
	CScene::init();
	crashFptr = new std::function<void(void)>;
	beginFptr = new std::function<void(void)>;
	pressFptr = new std::function<void(void)>;
	endFptr = new std::function<void(void)>;


	this->createWindowUI();
	this->createButtonUI();
}

void CMapToolScene::createWindowUI()
{
	/***************************************************/
	//컨테이너 만들기
	/***************************************************/
	selectWindowContainer = new CSpriteObject_Container("Resources/Textures/Scene/MapToolScene/mapToolWindow", "png", 150, 600, 1);
	selectWindowContainer->setPosition(D3DXVECTOR3(75, GET_WINDOW_SIZE().cy / 2, 0.0f));
	selectWindowContainer->setVisible(false);
	selectWindowContainer->init(nullptr, nullptr, nullptr, nullptr);

		/***************************************************/
		//스크롤 바 만들기
		/***************************************************/
		UpDownScrollBar = new CSpriteObject_ScrollBar("Resources/Textures/Scene/MapToolScene/scrollBarEX", "png", 20, 490, 1);
		UpDownScrollBar->setPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			/**********************/
			//스크롤 바 버튼 만들기
			/**********************/
			scrollBarButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/buttonEx", "png", 30, 10, 1);
			scrollBarButton->init(nullptr, nullptr, nullptr, nullptr, true);

			UpDownScrollBar->init(
				nullptr, nullptr, nullptr, nullptr, 0, 300, scrollBarButton, true, D3DXVECTOR3(80, 60, 0.0f)
			);


		/***************************************************/
		//리스트 만들기
		/***************************************************/
		m_pSpriteList = new CSpriteObject_List("Resources/Textures/Scene/MapToolScene/mapToolWindow", "png", 150, 500, 1);
		m_pSpriteList->init(nullptr, nullptr, nullptr, nullptr,true,D3DXVECTOR3(0, selectWindowContainer->getPosition().y - 330,0.0f));
			/*************************/
			//리스트 내 버튼 만들기
			/*************************/
			m_pSpriteListButton[0] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/building", "png", 100, 100, 1);
			m_pSpriteListButton[0]->setPosition(m_pSpriteList->getPosition());
			(*endFptr) = [=](void) -> void
			{
				m_pSpriteList->setVisible(false);
				backButton->setVisible(true);
			};
			m_pSpriteListButton[0]->init(nullptr, nullptr, nullptr, endFptr, true);

			m_pSpriteListButton[1] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/building", "png", 100, 100, 1);
			m_pSpriteListButton[1]->setPosition(m_pSpriteList->getPosition());
			m_pSpriteListButton[1]->init(nullptr, nullptr, nullptr, nullptr, true);

			m_pSpriteListButton[2] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/building", "png", 100, 100, 1);
			m_pSpriteListButton[2]->setPosition(m_pSpriteList->getPosition());
			m_pSpriteListButton[2]->init(nullptr, nullptr, nullptr, nullptr, true);

			m_pSpriteListButton[3] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/building", "png", 100, 100, 1);
			m_pSpriteListButton[3]->setPosition(m_pSpriteList->getPosition());
			m_pSpriteListButton[3]->init(nullptr, nullptr, nullptr, nullptr, true);

			m_pSpriteListButton[4] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/building", "png", 100, 100, 1);
			m_pSpriteListButton[4]->setPosition(m_pSpriteList->getPosition());
			m_pSpriteListButton[4]->init(nullptr, nullptr, nullptr, nullptr, true);

			for (int i = 0; i < 5; i++)
			{
					char name[100];
					sprintf(name, "uiButton_%d", i);
					m_pSpriteList->addChildSpriteObject(name, CWindowType::BUTTON, m_pSpriteListButton[i]);
			}

		selectWindowContainer->addChildSpriteObject("ButtonList", CWindowType::LIST, m_pSpriteList);
		selectWindowContainer->addChildSpriteObject("ScrollBar", CWindowType::SCROLLBAR, UpDownScrollBar);

}

void CMapToolScene::createButtonUI()
{
	openButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/open", "png", 100, 100, 2);
	openButton->setPosition(D3DXVECTOR3(0, 0, 0));
	openButton->setVisible(true);

	closeButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/close", "png", 100, 100, 2);
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
		//열었을때 리스트 닫기
		m_pSpriteList->setVisible(true);
		backButton->setVisible(false);

	};
	backButton->init(nullptr, nullptr, nullptr, endFptr, true);
}

void CMapToolScene::update(void)
{
	CScene::update();
	this->buttonUpdate();
	selectWindowContainer->update();

	m_pSpriteList->getMoveOffset() = D3DXVECTOR3(0.0f, -(290 - UpDownScrollBar->getSetValue()), 0.0f);
	printf("%f\n", UpDownScrollBar->getSetValue());

}

void CMapToolScene::buttonUpdate()
{
	openButton->getWindow()->setAbsolutePosition(
		(D3DXVECTOR3(selectWindowContainer->getWindow()->getAbsolutePosition().x-25,
			selectWindowContainer->getWindow()->getAbsolutePosition().y - 300, 0)));
	openButton->update();

	closeButton->getWindow()->setAbsolutePosition(
		(D3DXVECTOR3(selectWindowContainer->getWindow()->getAbsolutePosition().x+150,
			selectWindowContainer->getWindow()->getAbsolutePosition().y - 300, 0)));
	closeButton->update();
	
	backButton->getWindow()->setAbsolutePosition(
		(D3DXVECTOR3(selectWindowContainer->getWindow()->getAbsolutePosition().x - 50,
			selectWindowContainer->getWindow()->getAbsolutePosition().y + 330, 0)));
	backButton->update();
}

void CMapToolScene::draw(void)
{
	CScene::draw();
}

void CMapToolScene::drawUI(void)
{
	CScene::drawUI();
	selectWindowContainer->drawUI();

	this->buttonDrawUI();

}

void CMapToolScene::buttonDrawUI()
{
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
