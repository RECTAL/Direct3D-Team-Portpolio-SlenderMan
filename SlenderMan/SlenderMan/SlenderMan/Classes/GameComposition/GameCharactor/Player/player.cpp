#include "player.h"

#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CInputManager.h"
#include "../../../Utility/Manager/CTimeManager.h"
#include "../../../Utility/Object/CameraObject/CCameraObject.h"
#include "../../../Utility/Object/LightObject/SpotLightObject/SpotLightObject.h"
#include "../../../Utility/Object/SkinnedObject/CSkinnedObject.h"

player::player()
{

}

player::~player(void)
{
	//SAFE_DELETE(playerObject);
	SAFE_DELETE(cameraObject);
	SAFE_DELETE(spotObj);
}

void player::init(void)
{

	cameraObject = new CCameraObject((float)GET_WINDOW_SIZE().cx / (float)GET_WINDOW_SIZE().cy);
	cameraObject->setPosition(D3DXVECTOR3(0.0f, 0.0f, -5.0f));

	spotObj = new CSpotLightObject(0, 300.0f, D3DXToRadian(5.0f), D3DXToRadian(15.0f));
	//playerObject = this->createPlayer();
}

void player::update(void)
{
	cameraObject->update();
	spotObj->update();
	spotObj->setPosition(cameraObject->getPosition());

	// 마우스 화면 조절 

	mouseSenterPos();


	float fSpeed = 15.0f;

	if (IS_KEY_DOWN(DIK_LSHIFT)) {
		fSpeed = 50.0f;
	}
	if (IS_KEY_DOWN(DIK_W)) {
		cameraObject->moveByZAxis(fSpeed * GET_DELTA_TIME());
		spotObj->moveByZAxis(fSpeed * GET_DELTA_TIME());
		playerState = EPlayerState::WALKGRASS;
	}
	if (IS_KEY_RELEASED(DIK_W))
	{
		playerState = EPlayerState::NONE;
	}
	if (IS_KEY_DOWN(DIK_S)) {
		cameraObject->moveByZAxis(-fSpeed * GET_DELTA_TIME());
		spotObj->moveByZAxis(-fSpeed * GET_DELTA_TIME());
	}

	if (IS_KEY_DOWN(DIK_A)) {
		cameraObject->moveByXAxis(-fSpeed * GET_DELTA_TIME());
		spotObj->moveByXAxis(-fSpeed * GET_DELTA_TIME());
	}
	else if (IS_KEY_DOWN(DIK_D)) {
		cameraObject->moveByXAxis(fSpeed * GET_DELTA_TIME());
		spotObj->moveByXAxis(fSpeed * GET_DELTA_TIME());
	}

	//playerObject->setPosition(D3DXVECTOR3(cameraObject->getPosition().x, cameraObject->getPosition().y, cameraObject->getPosition().z - 100));
	//playerObject->update();
}

void player::preDraw(void)
{
}

void player::doDraw(void)
{
	//playerObject->draw();
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

	cameraObject->rotateByYAxis(-dPosX / 5.0f, false);
	spotObj->rotateByYAxis(-dPosX / 5.0f, false);

	cameraObject->rotateByXAxis(-dPosY / 5.0f);
	spotObj->rotateByXAxis(-dPosY / 5.0f);

	ClientToScreen(GET_WINDOW_HANDLE(), &pt);
	SetCursorPos(pt.x, pt.y);
}

CSkinnedObject * player::createPlayer()
{
	CSkinnedObject::STParameters stParameters = {
		"Resources/Meshes/player/player.x",
		"Resources/Effects/DefaultSkinnedMesh.fx"
	};
	return createSkinnedMesh(stParameters);
}



