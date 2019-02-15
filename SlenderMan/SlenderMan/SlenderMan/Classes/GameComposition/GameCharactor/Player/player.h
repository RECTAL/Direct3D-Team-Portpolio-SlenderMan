#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../CCharactor.h"


class CCameraObject;
class SpotLightObject;
class CSkinnedObject;
class CMainPlayScene;

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

public:		//getter
	CCameraObject* getCamera(void) { return cameraObject; }
	CSpotLightObject* getLightObj(void) { return spotObj; }
	EPlayerState &getPlayerState(void) { return playerState; }
private:
	void mouseSenterPos();
	CSkinnedObject* createPlayer();
private:
	CCameraObject* cameraObject;
	CSpotLightObject* spotObj = nullptr;
	CMainPlayScene* m_pMainScene = nullptr;
	EPlayerState playerState = EPlayerState::NONE;

	//CSkinnedObject* playerObject;

};