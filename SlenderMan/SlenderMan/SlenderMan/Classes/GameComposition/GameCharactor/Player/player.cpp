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
	//SAFE_DELETE(cameraObject);
	//SAFE_DELETE(spotObj);
}

void player::init(void)
{
	cameraObj = new CCameraObject((float)GET_WINDOW_SIZE().cx / (float)GET_WINDOW_SIZE().cy);

	lightObj = new CSpotLightObject(0, 300.0f, D3DXToRadian(5.0f), D3DXToRadian(15.0f));
	lightObj->setCameraObj(cameraObj);
	this->addChildObject(cameraObj);
	this->addChildObject(lightObj);
	//playerObject = this->createPlayer();
}

void player::update(void)
{
	CCharactor::update();
	settingCamera();
	settingLight();
	// 마우스 화면 조절 
	static bool isEsc = false;
	if (!isEsc)
		mouseSenterPos();
	if (IS_KEY_DOWN(DIK_ESCAPE))
		isEsc = !isEsc;

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
	if (IS_KEY_RELEASED(DIK_W)){
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
	static float test = 0.0f;
	static float test2 = 0.0f;
	if (playerState & (int)EPlayerState::RUN)
	{
		test2 += GET_DELTA_TIME() * 5;
		test += test2;
		test = min(test, 30.0f);
		test = max(test, 0.0f);
		lightObj->rotateByXAxis(test);
	}
	else
	{
		lightObj->rotateByXAxis(test);
		test2 += GET_DELTA_TIME() * 5;
		test -= test2;
		test = min(test, 30.0f);
		test = max(test, 0.0f);
	}
	
	printf("test : %f\n", test);
	
	
	//playerObject->setPosition(D3DXVECTOR3(cameraObject->getPosition().x, cameraObject->getPosition().y, cameraObject->getPosition().z - 100));
	//playerObject->update();
}

void player::preDraw(void)
{
}

void player::doDraw(void)
{
	//playerObject->draw();
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

CSkinnedObject * player::createPlayer()
{
	CSkinnedObject::STParameters stParameters = {
		"Resources/Meshes/player/player.x",
		"Resources/Effects/DefaultSkinnedMesh.fx"
	};
	return createSkinnedMesh(stParameters);
}

void player::settingCamera()
{
	cameraObj->setRightDirection(this->getRightDirection());
	cameraObj->setUpDirection(this->getUpDirection());
	cameraObj->setForwardDirection(this->getForwardDirection());
	cameraObj->setPosition(this->getPosition());
}

void player::settingLight()
{
	lightObj->setRightDirection(this->getRightDirection());
	lightObj->setUpDirection(this->getUpDirection());
	lightObj->setForwardDirection(this->getForwardDirection());
	lightObj->setPosition(this->getPosition());
}





