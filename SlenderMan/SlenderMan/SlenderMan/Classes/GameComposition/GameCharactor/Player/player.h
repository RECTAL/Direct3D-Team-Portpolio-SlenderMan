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
private:
	void mouseSenterPos();
	CSkinnedObject* createPlayer();
private:
	void settingCamera();
	void settingLight();


	void adjustCollisionArea();
	

private:
	CCameraObject* cameraObj = nullptr;
	CSpotLightObject* lightObj = nullptr;
	CMainPlayScene* mainScene = nullptr;
	CSkinnedObject*		m_pSkinnedObj = nullptr;
	CStage*		m_pStage = nullptr;

	int playerState = (int)EPlayerState::NONE;
	bool isEsc = false;

	fPOINT		m_fTopLeft;
	fPOINT		m_fBottomRight;
	float		m_fCheckRange = 20.0f;
};