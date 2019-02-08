#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"

#define MAX_TREE_NUM 7
#define MAX_BUILDING 3
#define MAX_OBJECT 3

class CSpriteObject;
class CSpriteObject_ScrollBar;
class CSpriteObject_Button;
class CSpriteObject_List;
class CSpriteObject_Container;
class CSpriteObject_ListSquare;
class CStage;
class CCameraObject;
class CRenderObject;
class CMapToolScene :public CScene
{
private:
	struct MouseInfo
	{
		bool m_bIsSkinned = false;
		EObjType	   m_eObjType = EObjType::NONE;
		CRenderObject* m_pRenderObj = nullptr;
	};
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

	void createTreeButton(void);
	void createBuildingButton(void);
	void createObjectButton(void);

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

	CSpriteObject_Button* squareUpCover = nullptr;
	CSpriteObject_ListSquare* treeListSquare = nullptr;
	CSpriteObject_ListSquare* m_pBuildingListSquare = nullptr;
	CSpriteObject_ListSquare* m_pObjectListSquare = nullptr;
	CSpriteObject_Button* treeButton[MAX_TREE_NUM] = { nullptr };
	CSpriteObject_Button* m_pBuildingButton[MAX_BUILDING] = { nullptr };
	CSpriteObject_Button* m_pObjectButton[MAX_OBJECT] = { nullptr };

	CSpriteObject_Button* saveButton = nullptr;
	CSpriteObject_Button* loadButton = nullptr;


	MouseInfo		m_stMouseInfo;



	FPTR* crashFptr = nullptr;
	FPTR* beginFptr = nullptr;
	FPTR* pressFptr = nullptr;
	FPTR* endFptr = nullptr;

	bool isFirst = true;
	bool isEnableClick = false;

	float m_fAngleX = 0.0f;
	float m_fAngleY = 0.0f;
	float m_fAngleZ = 0.0f;

	float m_fScale = 0.0f;
};