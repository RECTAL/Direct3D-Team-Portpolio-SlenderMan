#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"

class CSpriteObject;
class CSpriteObject_ScrollBar;
class CSpriteObject_Button;
class CSpriteObject_List;
class CSpriteObject_Container;
class CStage;
class CCameraObject;
class CMapToolScene :public CScene
{
public:		//constructor , destructor
	CMapToolScene(std::string a_stSceneName);
	virtual ~CMapToolScene();

public:		//interface
	virtual void update(void)override;				//interface : IUpdateable
	virtual void draw(void)override;				//interface : IRenderable
	virtual void drawUI(void)override;				//interface : IRenderable

	virtual LRESULT handleWindowMessage(HWND a_hWindow,
		UINT a_nMessage,
		WPARAM a_wParam,
		LPARAM a_lParam) override;					//interface : IWindowMessageHandler

public:		//public override function
	virtual void init()override;					//override	: CScene
	virtual void createWindowUI()override;
private:

	void createButtonUI();
	void createCameraObj();
	void createStage();
	void buttonUpdate();

	void buttonDrawUI();
private:
	POINT m_stPrevMousePosition;


	CSpriteObject_Container* selectWindowContainer = nullptr;
	CSpriteObject_Button* upCover = nullptr;
	CSpriteObject_Button* downCover = nullptr;

	CSpriteObject_ScrollBar* UpDownScrollBar = nullptr;
	CSpriteObject_Button*	 scrollBarButton = nullptr;

	CSpriteObject_List*		 m_pSpriteList = nullptr;
	CSpriteObject_Button*	 m_pSpriteListButton[5] = { nullptr };

	CSpriteObject_Button* openButton = nullptr;
	CSpriteObject_Button* closeButton = nullptr;
	CSpriteObject_Button* buildingButton = nullptr;
	CSpriteObject_Button* terrainButton = nullptr;
	CSpriteObject_Button* backButton = nullptr;
	CSpriteObject_Button* goTitleButton = nullptr;

	CSpriteObject* m_pCurrent = nullptr;
	CStage*		   m_pStage = nullptr;

	CCameraObject* m_pCamera = nullptr;




	FPTR* crashFptr = nullptr;
	FPTR* beginFptr = nullptr;
	FPTR* pressFptr = nullptr;
	FPTR* endFptr = nullptr;

	bool isFirst = true;
};