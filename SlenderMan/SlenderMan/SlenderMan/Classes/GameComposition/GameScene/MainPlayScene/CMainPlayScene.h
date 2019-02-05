#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"



class CCameraObject;
class CTerrainObject;
class CStaticObject;
class CLightObject;
class CSpotLightObject;
class CMainPlayScene :public CScene
{
public:		//constructor , destructor
	CMainPlayScene(std::string a_stSceneName);
	virtual ~CMainPlayScene();

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
	void	createRenderTarget();
	void	createMesh();
	void	createCamera();
	void	createSound();

public:
	CTerrainObject* createTerrain();
	CStaticObject*	createStaticMesh();
	CLightObject*	createDirectionalLight();
	CSpotLightObject* createSpotObj();
private:
	POINT m_stPrevMousePosition;

	CCameraObject* m_pCamera = nullptr;
	CTerrainObject* m_pTerrain = nullptr;
	CStaticObject*	m_pStaticMesh = nullptr;
	CStaticObject*	m_pStaticMesh1 = nullptr;
	CStaticObject*	m_pStaticMesh2 = nullptr;
	CLightObject*	m_pLightObj = nullptr;
	CSpotLightObject* m_pSpotObj = nullptr;

	LPD3DXMESH m_pSphere = nullptr;

	bool isFirst = true;
};