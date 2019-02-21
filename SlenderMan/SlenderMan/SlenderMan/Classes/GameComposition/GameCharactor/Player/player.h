#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../CCharactor.h"

#define GRAVITY			1.8f

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
	CSkinnedObject*	getSkinnedObj(void) { return m_pSkinnedObj; }
	int &getPlayerState(void) { return playerState; }
	int getPage() { return m_nPage; }



	/**********************************************/
	//setter
	/**********************************************/
	void	setStage(CStage* a_pStage) { m_pStage = a_pStage; }
	void setMainSceneAddress(CMainPlayScene* address) { mainScene = address; }
private:
	void mouseSenterPos();
	CSkinnedObject* createPlayer();
private:
	void settingCamera();
	void settingLight();
	void settingSkinnedObj();

	void adjustJump();
	void adjustCollisionArea();
	bool checkCollisionArea();
	bool checkCollisionTerrain(EDirection a_eDirection);
	void checkPaperObj();

private:
	CCameraObject* cameraObj = nullptr;
	CSpotLightObject* lightObj = nullptr;
	CMainPlayScene* mainScene = nullptr;
	CSkinnedObject*		m_pSkinnedObj = nullptr;
	CStage*		m_pStage = nullptr;

	int playerState = (int)EPlayerState::NONE;
	bool isEsc = false;



	bool m_bIsLeft = false;
	bool m_bIsRight = false;
	bool m_bIsFront = false;
	bool m_bIsBack = false;

	D3DXVECTOR3 m_stSkinnedRightVec3;
	D3DXVECTOR3 m_stSkinnedUpVec3;
	D3DXVECTOR3	m_stSkinnedForwardVec3;

	bool		m_bIsJump = false;
	float		m_fJumpTime = 0.0f;
	float		m_fYVelocity = 0.0f;

	fPOINT		m_fTopLeft;
	fPOINT		m_fBottomRight;
	STRay		m_stCameraForwardRay;
	STRay		m_stSkinnedRay;


	float		m_fCheckRange = 6.0f;

	bool isPlay = false;
	bool isRun = false;
	int m_nPage;

};