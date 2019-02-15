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
	//SAFE_DELETE(playerObject);
	SAFE_DELETE(cameraObject);
	SAFE_DELETE(spotObj);
	SAFE_DELETE(m_pMainScene);
}

void player::init(void)
{
	cameraObj = new CCameraObject((float)GET_WINDOW_SIZE().cx / (float)GET_WINDOW_SIZE().cy);

	lightObj = new CSpotLightObject(0, 300.0f, D3DXToRadian(5.0f), D3DXToRadian(15.0f));
	lightObj->setCameraObj(cameraObj);
	this->addChildObject(cameraObj);
	this->addChildObject(lightObj);
	//playerObject = this->createPlayer();

	m_pMainScene = dynamic_cast<CMainPlayScene*> (FIND_SCENE(GAMESCENE_MAINPLAY));
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
	lightObj->setRightDirection(this->getRightDirection());
	lightObj->setUpDirection(this->getUpDirection());
	lightObj->setForwardDirection(this->getForwardDirection());
	lightObj->setPosition(this->getPosition());

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
	D3DXVECTOR3	stRightVex3 = this->getRightDirection();
	D3DXVECTOR3	sUpVex3 = this->getUpDirection();
	D3DXVECTOR3	stForwardVex3 = this->getForwardDirection();
	D3DXVECTOR3	stPosVec3 = this->getPosition() + stRightVex3*6.0f + sUpVex3*-5.0f + stForwardVex3*5.0f;
	D3DXVECTOR3 stNewRightVec3;
	D3DXVECTOR3 stNewUpVec3;
	D3DXVECTOR3 stNewForwardVec3;

	D3DXMATRIXA16 stRotateMatrix;
	D3DXMatrixRotationYawPitchRoll(&stRotateMatrix, D3DXToRadian(-30.0f), D3DXToRadian(-25.0f), D3DXToRadian(0.0f));

	D3DXVec3TransformNormal(&stNewRightVec3, &stRightVex3, &stRotateMatrix);
	D3DXVec3TransformNormal(&stNewUpVec3, &sUpVex3, &stRotateMatrix);
	D3DXVec3TransformNormal(&stNewForwardVec3, &stForwardVex3, &stRotateMatrix);

	D3DXVec3Normalize(&stNewRightVec3, &stNewRightVec3);
	D3DXVec3Normalize(&stNewUpVec3, &stNewUpVec3);
	D3DXVec3Normalize(&stNewForwardVec3, &stNewForwardVec3);


	lightObj->setRightDirection(stNewRightVec3);
	lightObj->setUpDirection(stNewUpVec3);
	lightObj->setForwardDirection(stNewForwardVec3);
	lightObj->setPosition(stPosVec3);
}
