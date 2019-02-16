#include "player.h"
#include "../../../Function/GlobalFunction.h"
#include "../../../Utility/Base/CStage.h"
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
	if(cameraObj == nullptr)
		cameraObj = new CCameraObject((float)GET_WINDOW_SIZE().cx / (float)GET_WINDOW_SIZE().cy);
	if (lightObj == nullptr)
		lightObj = new CSpotLightObject(0, 300.0f, D3DXToRadian(5.0f), D3DXToRadian(15.0f));

	this->setPosition(D3DXVECTOR3(203.0f, 39.0f, 43.0f));
	lightObj->setCameraObj(cameraObj);
	this->addChildObject(cameraObj);
	this->addChildObject(lightObj);

	if (m_pSkinnedObj == nullptr)
	{
		m_pSkinnedObj = this->createPlayer();
	}
	m_pSkinnedObj->setPosition(this->getPosition());
	m_pSkinnedObj->setVisible(true);
	m_pSkinnedObj->setScale(D3DXVECTOR3(0.2f, 0.2f, 0.2f));
}

void player::update(void)
{
	if (!mainScene->getIsMenu())
	{
		CCharactor::update();
		
		settingCamera();
		settingLight();
		settingSkinnedObj();

		m_pSkinnedObj->update();
		
		// 마우스 화면 조절 
		static bool isEsc = false;
		if (!isEsc)
			mouseSenterPos();

		float fSpeed = 15.0f;

		m_bIsLeft = false;
		m_bIsRight = false;
		m_bIsFront = false;
		m_bIsBack = false;

		if (IS_KEY_DOWN(DIK_W)) {
			playerState |= (int)EPlayerState::WALKGRASS;
			m_bIsFront = true;
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
			m_bIsBack = true;
		}
		if (IS_KEY_DOWN(DIK_A)) {
			this->moveByXAxis(-fSpeed * GET_DELTA_TIME());
			m_bIsLeft = true;
		}
		if (IS_KEY_DOWN(DIK_D)) {
			this->moveByXAxis(fSpeed * GET_DELTA_TIME());
			m_bIsRight = true;
		}

		m_fTopLeft.x = this->getPosition().x - m_fCheckRange;
		m_fTopLeft.y = this->getPosition().z - m_fCheckRange;


		m_fBottomRight.x = this->getPosition().x + m_fCheckRange;
		m_fBottomRight.y = this->getPosition().z + m_fCheckRange;

		adjustCollisionArea();
		
		if (this->checkCollisionArea())
		{
			if(m_bIsFront)this->moveByZAxis(-fSpeed * GET_DELTA_TIME());
			if(m_bIsBack)this->moveByZAxis(fSpeed * GET_DELTA_TIME());
			if(m_bIsLeft)this->moveByXAxis(fSpeed * GET_DELTA_TIME());
			if(m_bIsRight)this->moveByXAxis(-fSpeed * GET_DELTA_TIME());
		}

	}
}

void player::preDraw(void)
{
}

void player::doDraw(void)
{
	lightObj->draw();
	m_pSkinnedObj->draw();
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
		cameraObj,
		m_pStage->getDirectionalLightObj(),
		"Resources/Meshes/player/player.X",
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

void player::settingSkinnedObj()
{
	m_stSkinnedRightVec3 = -cameraObj->getRightDirection();
	m_stSkinnedUpVec3 = WORLD_UP_DIRECTION;
	m_stSkinnedForwardVec3;
	D3DXVec3Cross(&m_stSkinnedForwardVec3, &m_stSkinnedRightVec3, &m_stSkinnedUpVec3);
	D3DXVec3Cross(&m_stSkinnedRightVec3, &m_stSkinnedUpVec3, &m_stSkinnedForwardVec3);
	D3DXVec3Normalize(&m_stSkinnedRightVec3, &m_stSkinnedRightVec3);
	D3DXVec3Normalize(&m_stSkinnedUpVec3, &m_stSkinnedUpVec3);
	D3DXVec3Normalize(&m_stSkinnedForwardVec3, &m_stSkinnedForwardVec3);

	m_pSkinnedObj->setPosition(cameraObj->getPosition() + D3DXVECTOR3(0.0f, -13.0f, 0.0f) + m_stSkinnedForwardVec3 * 1.5f);

	m_pSkinnedObj->setRightDirection(m_stSkinnedRightVec3);
	m_pSkinnedObj->setUpDirection(m_stSkinnedUpVec3);
	m_pSkinnedObj->setForwardDirection(m_stSkinnedForwardVec3);
}

void player::adjustCollisionArea()
{
	if (m_pStage->getTerrainObj() != nullptr)
	{
		int nWidth = m_pStage->getTerrainObj()->getCXTerrain();
		int nHeight = m_pStage->getTerrainObj()->getCZTerrain();
		
		if (m_fTopLeft.x < -nWidth / 2) m_fTopLeft.x = -nWidth / 2;
		if (m_fTopLeft.y < -nHeight / 2)m_fTopLeft.y = -nHeight/2;


		if (m_fBottomRight.x > nWidth / 2) m_fBottomRight.x = nWidth / 2;
		if (m_fBottomRight.y > nHeight / 2)m_fBottomRight.y = nHeight/2;

	}
}

bool player::checkCollisionArea()
{
	bool isCollision = false;
	if (m_pStage->getTerrainObj() != nullptr)
	{
		int nTerrainWidth = m_pStage->getTerrainObj()->getCXTerrain();
		int nTerrainHeight = m_pStage->getTerrainObj()->getCZTerrain();
		int nWidth = m_pStage->getTerrainObj()->getCXDIB();
		int nScaleX = m_pStage->getTerrainObj()->getScale().x;
		int nScaleZ = m_pStage->getTerrainObj()->getScale().z;


		D3DXVECTOR3 stTopLeftPos = D3DXVECTOR3((m_fTopLeft.x + nTerrainWidth / 2) / nScaleX, 0, (m_fTopLeft.y + nTerrainHeight / 2) / nScaleZ);
		D3DXVECTOR3 stBottomRightPos = D3DXVECTOR3((m_fBottomRight.x + nTerrainWidth / 2) / nScaleX, 0, (m_fBottomRight.y + nTerrainHeight / 2) / nScaleZ);
		int xTopLeftIndex	  = (int)stTopLeftPos.x;
		int zTopLeftIndex	  = (int)stTopLeftPos.z;
		int xBottomRightIndex = (int)stBottomRightPos.x;
		int zBottomRightIndex = (int)stBottomRightPos.z;

		int dxIndex = xBottomRightIndex - xTopLeftIndex;
		int dyIndex = zBottomRightIndex - zTopLeftIndex;

		std::vector<CRenderObject*> oCollionObjList;
		for (int i = 0; i <= dyIndex; i++)
		{
			for (int j = 0; j <= dxIndex; j++)
			{
				int nIndex = (zTopLeftIndex + i)*nWidth + (xTopLeftIndex + j);
				for (auto iter : m_pStage->getObjList()[nIndex])
				{
					if (iter->getbIsCollision())oCollionObjList.push_back(iter);
				}
			}
		}

		for (auto iter : oCollionObjList)
		{
			if (IsIntersectBox(m_pSkinnedObj->getBoundingBox(), iter->getBoundingBox()))
			{
				isCollision = true;
				break;
			}
		}
	}
	return isCollision;
}

