#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../CCharactor.h"


class CCameraObject;
class SpotLightObject;
class CMainPlayScene;
class CSkinnedObject;
class CStage;
class player : public CCharactor
{
public:
	player();
	virtual ~player(void);
public:
	virtual void init(void) override;
	virtual void update(void) override;

	virtual void preDraw(void) override;
	virtual void doDraw(void) override;
	virtual void postDraw(void) override;

public:		//getter,setter
	/**********************************************/
	//getter
	/**********************************************/
	CCameraObject* getCamera(void) { return cameraObj; }
	CSpotLightObject* getLightObj(void) { return lightObj; }
	int &getPlayerState(void) { return playerState; }



	/**********************************************/
	//setter
	/**********************************************/
	void	setStage(CStage* a_pStage) { m_pStage = a_pStage; }
	void	setMainSceneAddress(CMainPlayScene* address) { mainScene = address; }
private:
	void mouseSenterPos();
	CSkinnedObject* createPlayer();
private:
	void settingCamera();
	void settingLight();
	void settingSkinnedObj();


	void adjustCollisionArea();
	bool checkCollisionArea();
	

private:
	CCameraObject* cameraObj = nullptr;
	CSpotLightObject* lightObj = nullptr;
	CMainPlayScene* mainScene = nullptr;
	CSkinnedObject*		m_pSkinnedObj = nullptr;
	CStage*		m_pStage = nullptr;

	int playerState = (int)EPlayerState::NONE;
	bool isEsc = false;



	bool m_bIsLeft	= false;
	bool m_bIsRight	= false;
	bool m_bIsFront = false;
	bool m_bIsBack	= false;

	D3DXVECTOR3 m_stSkinnedRightVec3;
	D3DXVECTOR3 m_stSkinnedUpVec3;
	D3DXVECTOR3	m_stSkinnedForwardVec3;

	fPOINT		m_fTopLeft;
	fPOINT		m_fBottomRight;
	float		m_fCheckRange = 3.0f;
};