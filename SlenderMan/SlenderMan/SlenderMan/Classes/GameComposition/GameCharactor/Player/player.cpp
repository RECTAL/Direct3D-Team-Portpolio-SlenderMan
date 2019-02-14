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
	cameraObject = new CCameraObject((float)GET_WINDOW_SIZE().cx / (float)GET_WINDOW_SIZE().cy);

	spotObj = new CSpotLightObject(0, 300.0f, D3DXToRadian(5.0f), D3DXToRadian(15.0f));
	spotObj->setPosition(D3DXVECTOR3(this->getPosition().x - 10, this->getPosition().y - 5, this->getPosition().z - 5));
	spotObj->setRotation(D3DXVECTOR3(D3DXToRadian(30), D3DXToRadian(-50), 0.0f));
	this->addChildObject(cameraObject);
	this->addChildObject(spotObj);
	//playerObject = this->createPlayer();
}

void player::update(void)
{
	CCharactor::update();
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
		for (auto iter : m_oChildObjectList) {
			iter->moveByZAxis(fSpeed * GET_DELTA_TIME());
		}
	}
	if (IS_KEY_RELEASED(DIK_W)){
		playerState = (int)EPlayerState::NONE;
	}
	if (IS_KEY_DOWN(DIK_S)) {
		this->moveByZAxis(-fSpeed * GET_DELTA_TIME());
		for (auto iter : m_oChildObjectList) {
			iter->moveByZAxis(-fSpeed * GET_DELTA_TIME());
		}
	}
	if (IS_KEY_DOWN(DIK_A)) {
		this->moveByXAxis(-fSpeed * GET_DELTA_TIME());
		for (auto iter : m_oChildObjectList) {
			iter->moveByXAxis(-fSpeed * GET_DELTA_TIME());
		}
	}
	if (IS_KEY_DOWN(DIK_D)) {
		this->moveByXAxis(fSpeed * GET_DELTA_TIME());
		for (auto iter : m_oChildObjectList) {
			iter->moveByXAxis(fSpeed * GET_DELTA_TIME());
		}
	}
	static float test = 0.0f;
	
	if (playerState & (int)EPlayerState::RUN)
	{
		test += GET_DELTA_TIME();
		test = min(test, 20);
		test = max(test, 0);
		//cameraObject->rotateByXAxis(-test);
		//spotObj->rotateByXAxis(test);
	}
	else
	{
		test -= GET_DELTA_TIME();
		test = min(test, 20);
		test = max(test, 0);
		//cameraObject->rotateByXAxis(test);
		//spotObj->rotateByXAxis(-test);
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
	for (auto iter : m_oChildObjectList) {
		iter->rotateByYAxis(-dPosX / 5.0f, false);
	}
	this->rotateByXAxis(-dPosY / 5.0f);
	for (auto iter : m_oChildObjectList) {
		iter->rotateByXAxis(-dPosY / 5.0f);
	}
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





