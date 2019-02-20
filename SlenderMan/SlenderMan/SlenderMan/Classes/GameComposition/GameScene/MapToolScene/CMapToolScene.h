#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"

#define MAX_LIST_BUTTON 5
#define MAX_TREE 9
#define MAX_HOUSE 4
#define MAX_OBJECT 4
#define MAX_SOUND 7
#define MAX_PAGE 9

class CSpriteObject;
class CSpriteObject_ScrollBar;
class CSpriteObject_Button;
class CSpriteObject_List;
class CSpriteObject_Container;
class CSpriteObject_ListSquare;
class CStage;
class CCameraObject;
class CRenderObject;
class CLabelObject;
class CMapToolScene :public CScene
{
private:
	struct MouseInfo
	{
		bool m_bDraw = false;
		EObjClasses	   m_eObjClasses = EObjClasses::NONE;
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

public:		//getter,setter
	/**************************************************/
	//getter
	/**************************************************/
	CStage*	getStage() { return m_pStage; }

	/**************************************************/
	//setter
	/**************************************************/

private:

	void createButtonUI();
	void createCameraObj();
	void createStage();

	void createLabel();

	void buttonUpdate();
	void listUpdate();
	void labelUpdate();

	// 버튼 만들기
	void createTreeButton(void);
	void createBuildingButton(void);
	void createObjectButton(void);
	void createSoundButton(void);
	void createPageButton(void);
	void createCheckBoxButton(void);

	// 버튼 그리기
	void buttonDrawUI();
	void labelDrawUI();

	// 키입력
	void inputKey(void);

	// 제거
	void removeUI(void);
	void removeList(void);

private:
	POINT m_stPrevMousePosition;

	CSpriteObject_Container* m_pSelectWindowContainer = nullptr;
	CSpriteObject_Button* m_pUpCover = nullptr;
	CSpriteObject_Button* m_pDownCover = nullptr;

	CSpriteObject_ScrollBar* m_pUpDownScrollBar = nullptr;
	CSpriteObject_Button*	 m_pScrollBarButton = nullptr;

	CSpriteObject_List*		 m_pSpriteList = nullptr;
	CSpriteObject_Button*	 m_pSpriteListButton[MAX_LIST_BUTTON] = { nullptr };

	CSpriteObject_Button* m_pOpenButton = nullptr;
	CSpriteObject_Button* m_pCloseButton = nullptr;
	CSpriteObject_Button* m_pBackButton = nullptr;
	CSpriteObject_Button* m_pGoTitleButton = nullptr;
	CSpriteObject_Button* m_pCollisionButton = nullptr;
	CSpriteObject_Button* m_pDebugButton = nullptr;

	CSpriteObject_Button* m_pTreeButton[MAX_TREE] = { nullptr };
	CSpriteObject_Button* m_pHouseButton[MAX_HOUSE] = { nullptr };
	CSpriteObject_Button* m_pObjectButton[MAX_OBJECT] = { nullptr };
	CSpriteObject_Button* m_pSoundButton[MAX_SOUND] = { nullptr };
	CSpriteObject_Button* m_pPageButton[MAX_PAGE] = { nullptr };

	CSpriteObject_Button* m_pSaveButton = nullptr;
	CSpriteObject_Button* m_pLoadButton = nullptr;

	CSpriteObject* m_pCurrent = nullptr;
	CStage*		   m_pStage = nullptr;

	CCameraObject* m_pCamera = nullptr;

	CSpriteObject_Button* m_pSquareUpCover = nullptr;
	CSpriteObject_ListSquare* m_pTreeListSquare = nullptr;
	CSpriteObject_ListSquare* m_pBuildingListSquare = nullptr;
	CSpriteObject_ListSquare* m_pObjectListSquare = nullptr;
	CSpriteObject_ListSquare* m_pSoundListSquare = nullptr;
	CSpriteObject_ListSquare* m_pPageListSquare = nullptr;

	CLabelObject*		m_pScaleLabel;
	CLabelObject*		m_pOffsetLabel;
	CLabelObject*		m_pRotateLabel;

	MouseInfo		m_stMouseInfo;



	FPTR* crashFptr = nullptr;
	FPTR* beginFptr = nullptr;
	FPTR* pressFptr = nullptr;
	FPTR* endFptr	= nullptr;

	bool isFirst			= true;
	bool isEnableClick		= false;
	bool m_bIsDebug			= false;
	bool m_bIsCollision		= false;

	float m_fAngleX = 0.0f;
	float m_fAngleY = 0.0f;
	float m_fAngleZ = 0.0f;

	float m_fScale = 0.2f;
	float m_fOffesetY = 0.0f;

};