#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../CCharactor.h"


class CCameraObject;
class SpotLightObject;
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
	CCameraObject* getCamera(void) { return cameraObj; }
	CSpotLightObject* getLightObj(void) { return lightObj; }
	int &getPlayerState(void) { return playerState; }
	void mainSceneAddress(CMainPlayScene* address) { mainScene = address; }
private:
	void mouseSenterPos();
protected:
	void settingCamera();
	void settingLight();
private:
	CCameraObject* cameraObj = nullptr;
	CSpotLightObject* lightObj = nullptr;
	CMainPlayScene* mainScene = nullptr;
	int playerState = (int)EPlayerState::NONE;
	bool isEsc = false;


};