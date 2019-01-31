#include "CMapToolScene.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CInputManager.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Button.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_ScrollBar.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_List.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Container.h"

CMapToolScene::CMapToolScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CMapToolScene::~CMapToolScene()
{
	SAFE_DELETE(m_pSpriteButton);
	SAFE_DELETE(m_pSpriteScrollBar);

	for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE(m_pSpriteListButton[i]);
	}
	SAFE_DELETE(m_pSpriteList);
	SAFE_DELETE(m_pSpriteContainer);
}

void CMapToolScene::init()
{
	CScene::init();
	this->createWindowUI();
}

void CMapToolScene::createWindowUI()
{
	/***************************************************/
	//컨테이너 만들기
	/***************************************************/
	m_pSpriteContainer = new CSpriteObject_Container("Resources/Textures/Scene/MapToolScene/ExWindow", "png", 300, 310, 1);
	m_pSpriteContainer->setPosition(D3DXVECTOR3(300.0f, 200.0f, 0.0f));
	m_pSpriteContainer->init(nullptr, nullptr, nullptr, nullptr);
	
	
		/***************************************************/
		//스크롤 바 만들기
		/***************************************************/
		m_pSpriteScrollBar = new CSpriteObject_ScrollBar("Resources/Textures/Scene/MapToolScene/scrollBarEX", "png", 20, 100, 1);
		m_pSpriteScrollBar->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0.0f));
			/**********************/
			//스크롤 바 버튼 만들기
			/**********************/
			m_pSpriteButton = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/buttonEx", "png", 30, 10, 1);
			m_pSpriteButton->init(nullptr, nullptr, nullptr, nullptr, true);


		m_pSpriteScrollBar->init(
				nullptr,nullptr,nullptr,nullptr,0,300,m_pSpriteButton,true, D3DXVECTOR3(150.0f, 0.0f, 0.0f)
			);


		/***************************************************/
		//리스트 만들기
		/***************************************************/
		m_pSpriteList = new CSpriteObject_List("Resources/Textures/Scene/MapToolScene/ExWindow", "png", 150, 160, 1);
		m_pSpriteList->init(nullptr, nullptr, nullptr, nullptr,true,D3DXVECTOR3(-50.0f,0.0f,0.0f));
			/*************************/
			//리스트 내 버튼 만들기
			/*************************/
			for (int i = 0; i < 5; i++)
			{
				m_pSpriteListButton[i] = new CSpriteObject_Button("Resources/Textures/Scene/MapToolScene/gameStart", "png", 150, 50, 1);
				m_pSpriteListButton[i]->setPosition(m_pSpriteList->getPosition());
				m_pSpriteListButton[i]->init(nullptr, nullptr, nullptr, nullptr, true);
		
				char name[100];
				sprintf(name, "uiButton_%d", i);
				m_pSpriteList->addChildSpriteObject(name, CWindowType::BUTTON, m_pSpriteListButton[i]);
			}

		m_pSpriteContainer->addChildSpriteObject("ButtonList", CWindowType::LIST, m_pSpriteList);
		m_pSpriteContainer->addChildSpriteObject("ScrollBar", CWindowType::SCROLLBAR, m_pSpriteScrollBar);

}


void CMapToolScene::update(void)
{
	CScene::update();
	m_pSpriteContainer->update();

	m_pSpriteList->getMoveOffset() = D3DXVECTOR3(0.0f,-(290 -m_pSpriteScrollBar->getSetValue()), 0.0f);
	printf("%f\n", m_pSpriteScrollBar->getSetValue());

	if (IS_KEY_PRESSED(DIK_F1))
	{
		m_pSpriteContainer->setVisible(!m_pSpriteContainer->getVisible());
	}
}

void CMapToolScene::draw(void)
{
	CScene::draw();
}

void CMapToolScene::drawUI(void)
{
	CScene::drawUI();
	m_pSpriteContainer->drawUI();
}

LRESULT CMapToolScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
