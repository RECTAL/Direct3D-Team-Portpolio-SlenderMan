#include "player.h"

#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CInputManager.h"
#include "../../../Utility/Manager/CTimeManager.h"
#include "../../../Utility/Manager/CSceneManager.h"
#include "../../../Utility/Object/CameraObject/CCameraObject.h"
#include "../../../Utility/Object/LightObject/SpotLightObject/SpotLightObject.h"
#include "../../../Utility/Object/SkinnedObject/CSkinnedObject.h"
#include "../../../GameComposition/GameScene/MainPlayScene/CMainPlayScene.h"

player::player()
{

}

player::~player(void)
{

}

void player::init(void)
{
	cameraObj = new CCameraObject((float)GET_WINDOW_SIZE().cx / (float)GET_WINDOW_SIZE().cy);
	lightObj = new CSpotLightObject(0, 300.0f, D3DXToRadian(5.0f), D3DXToRadian(15.0f));
	lightObj->setCameraObj(cameraObj);
	this->addChildObject(cameraObj);
	this->addChildObject(lightObj);

}

void player::update(void)
{
	if (!mainScene->getIsMenu())
	{
		CCharactor::update();

		settingCamera();
		settingLight();
		// 마우스 화면 조절 
		mouseSenterPos();

		float fSpeed = 15.0f;

		if (IS_KEY_DOWN(DIK_W)) {
			playerState |= (int)EPlayerState::WALKGRASS;
			if (IS_KEY_DOWN(DIK_LSHIFT)) {
				fSpeed = 50.0f;
				playerState |= (int)EPlayerState::RUN;
			}
			else if (IS_KEY_RELEASED(DIK_LSHIFT)) {
				playerState = (int)EPlayerState::NONE;
			}
			this->moveByZAxis(fSpeed * GET_DELTA_TIME());
		}
		if (IS_KEY_RELEASED(DIK_W)) {
			playerState = (int)EPlayerState::NONE;
		}
		if (IS_KEY_DOWN(DIK_S)) {
			this->moveByZAxis(-fSpeed * GET_DELTA_TIME());
		}
		if (IS_KEY_DOWN(DIK_A)) {
			this->moveByXAxis(-fSpeed * GET_DELTA_TIME());
		}
		if (IS_KEY_DOWN(DIK_D)) {
			this->moveByXAxis(fSpeed * GET_DELTA_TIME());
		}
	}
}

void player::preDraw(void)
{
}

void player::doDraw(void)
{
	lightObj->draw();
}

void player::postDraw(void)
{
}

void player::mouseSenterPos()
{
	RECT rc;
	POINT pt = { 0 ,0 };
	GetClientRect(GET_WINDOW_HANDLE(), &rc);
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;

	int dPosX = pt.x - GET_MOUSE_POSITION().x;
	int dPosY = pt.y - GET_MOUSE_POSITION().y;

	this->rotateByYAxis(-dPosX / 5.0f, false);
	this->rotateByXAxis(-dPosY / 5.0f);
	ClientToScreen(GET_WINDOW_HANDLE(), &pt);
	SetCursorPos(pt.x, pt.y);
}

void player::settingCamera()
{
	D3DXVECTOR3	stRightVex3 = this->getRightDirection();
	D3DXVECTOR3	sUpVex3 = this->getUpDirection();
	D3DXVECTOR3	stForwardVex3 = this->getForwardDirection();
	D3DXVECTOR3	stPosVec3 = this->getPosition();

	cameraObj->setRightDirection(stRightVex3);
	cameraObj->setUpDirection(sUpVex3);
	cameraObj->setForwardDirection(stForwardVex3);
	cameraObj->setPosition(stPosVec3);

}

void player::settingLight()
{
	D3DXVECTOR3	stRightVex3 = this->getRightDirection();
	D3DXVECTOR3	sUpVex3 = this->getUpDirection();
	D3DXVECTOR3	stForwardVex3 = this->getForwardDirection();
	D3DXVECTOR3	stPosVec3 = this->getPosition() + stRightVex3 - sUpVex3;

	lightObj->setRightDirection(stRightVex3);
	lightObj->setUpDirection(sUpVex3);
	lightObj->setForwardDirection(stForwardVex3);
	lightObj->setPosition(stPosVec3);

	static float slowLight = 0.0f;
	static float speed = 5.0f;
	if (playerState & (int)EPlayerState::RUN)
	{
		slowLight += GET_DELTA_TIME() * speed;
		slowLight = min(slowLight, 30.0f);
		slowLight = max(slowLight, 0.0f);
		speed += 0.5f;
		speed = min(speed, 30.0f);
		speed = max(speed, 5.0f);
		lightObj->rotateByXAxis(slowLight);
	}
	else
	{
		lightObj->rotateByXAxis(slowLight);
		speed -= 0.3f;
		speed = min(speed, 30.0f);
		speed = max(speed, 5.0f);
		slowLight -= GET_DELTA_TIME() * speed;
		slowLight = min(slowLight, 30.0f);
		slowLight = max(slowLight, 0.0f);
	}
}

