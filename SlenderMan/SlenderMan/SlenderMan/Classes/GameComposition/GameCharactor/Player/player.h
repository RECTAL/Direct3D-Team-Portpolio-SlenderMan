#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../CCharactor.h"


class CCameraObject;
class SpotLightObject;
class CSkinnedObject;

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
private:
	void mouseSenterPos();
	CSkinnedObject* createPlayer();
protected:
	void settingCamera();
	void settingLight();
private:
	CCameraObject* cameraObj;
	CSpotLightObject* lightObj = nullptr;
	int playerState = (int)EPlayerState::NONE;

	//CSkinnedObject* playerObject;

};