#include "player.h"
#include "../../../Function/GlobalFunction.h"
#include "../../../Utility/Base/CStage.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CInputManager.h"
#include "../../../Utility/Manager/CTimeManager.h"
#include "../../../Utility/Manager/CSceneManager.h"
#include "../../../Utility/Manager/CSoundManager.h"
#include "../../../Utility/Object/CameraObject/CCameraObject.h"
#include "../../../Utility/Object/LightObject/SpotLightObject/SpotLightObject.h"
#include "../../../Utility/Object/SkinnedObject/CSkinnedObject.h"
#include "../../../GameComposition/GameScene/MainPlayScene/CMainPlayScene.h"
#include "../../GameObject/Decorate/CDecorate_BillboardObj.h"

player::player()
{

}

player::~player(void)
{

}

void player::init(void)
{
	if (cameraObj == nullptr)
		cameraObj = new CCameraObject((float)GET_WINDOW_SIZE().cx / (float)GET_WINDOW_SIZE().cy);
	if (lightObj == nullptr)
		lightObj = new CSpotLightObject(0, 300.0f, D3DXToRadian(5.0f), D3DXToRadian(15.0f));

	this->setPosition(D3DXVECTOR3(190.0f, 50.0f, 20.0f));
	lightObj->setCameraObj(cameraObj);
	this->addChildObject(cameraObj);
	this->addChildObject(lightObj);

	if (m_pSkinnedObj == nullptr)
	{
		m_pSkinnedObj = this->createPlayer();
	}
	m_pSkinnedObj->setPosition(this->getPosition());
	m_pSkinnedObj->setVisible(true);
	m_pSkinnedObj->setScale(D3DXVECTOR3(0.045f, 0.045f, 0.045f));
	m_nPage = 0;
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

		m_bIsLeft = false;
		m_bIsRight = false;
		m_bIsFront = false;
		m_bIsBack = false;

		m_fTopLeft.x = this->getPosition().x - m_fCheckRange;
		m_fTopLeft.y = this->getPosition().z - m_fCheckRange;


		m_fBottomRight.x = this->getPosition().x + m_fCheckRange;
		m_fBottomRight.y = this->getPosition().z + m_fCheckRange;

		float fSpeed = 15.0f;
		auto aniList = m_pSkinnedObj->getAnimationNameList();


		if (IS_KEY_DOWN(DIK_W) && !checkCollisionTerrain(EDirection::FRONT)) {
			playerState |= (int)EPlayerState::WALKGRASS;
			m_bIsFront = true;
			if (!isPlay)
			{
				m_pSkinnedObj->playAnimation(aniList[1], true);
				isPlay = true;
			}
			if (IS_KEY_DOWN(DIK_LSHIFT)) {
				fSpeed = 25.0f;
				playerState |= (int)EPlayerState::RUN;
				if (isPlay && !isRun)
				{
					m_pSkinnedObj->playAnimation(aniList[2], true);
					isPlay = true;
					isRun = true;
				}
			}
			else if (IS_KEY_RELEASED(DIK_LSHIFT)) {
				playerState = (int)EPlayerState::NONE;
				m_pSkinnedObj->playAnimation(aniList[0], true);
				isPlay = false;
				isRun = false;
			}

			this->moveByZAxis(fSpeed * GET_DELTA_TIME());
		}
		else if (IS_KEY_RELEASED(DIK_W)) {
			playerState = (int)EPlayerState::NONE;
			m_pSkinnedObj->playAnimation(aniList[0], true);
			isPlay = false;
		}
		if (IS_KEY_DOWN(DIK_S) && !checkCollisionTerrain(EDirection::BACK)) {
			this->moveByZAxis(-fSpeed * GET_DELTA_TIME());
			m_bIsBack = true;
		}
		if (IS_KEY_DOWN(DIK_A) && !checkCollisionTerrain(EDirection::LEFT)) {
			this->moveByXAxis(-fSpeed * GET_DELTA_TIME());
			m_bIsLeft = true;
		}
		if (IS_KEY_DOWN(DIK_D) && !checkCollisionTerrain(EDirection::RIGHT)) {
			this->moveByXAxis(fSpeed * GET_DELTA_TIME());
			m_bIsRight = true;
		}
		if (IS_KEY_PRESSED(DIK_SPACE) && !m_bIsJump)
		{
			m_bIsJump = true;
			m_fJumpTime = 0.0f;
			m_fYVelocity = 14.0f;
		}

		checkPaperObj();
		adjustJump();

		m_fTopLeft.x = this->getPosition().x - m_fCheckRange;
		m_fTopLeft.y = this->getPosition().z - m_fCheckRange;


		m_fBottomRight.x = this->getPosition().x + m_fCheckRange;
		m_fBottomRight.y = this->getPosition().z + m_fCheckRange;

		adjustCollisionArea();

		if (this->checkCollisionArea())
		{
			if (m_bIsJump)m_fYVelocity = 0.0f;
			if (m_bIsFront)this->moveByZAxis(-fSpeed * GET_DELTA_TIME());
			if (m_bIsBack)this->moveByZAxis(fSpeed * GET_DELTA_TIME());
			if (m_bIsLeft)this->moveByXAxis(fSpeed * GET_DELTA_TIME());
			if (m_bIsRight)this->moveByXAxis(-fSpeed * GET_DELTA_TIME());
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
	static int fixed = 0;
	RECT rc;
	POINT pt = { 0 ,0 };
	GetClientRect(GET_WINDOW_HANDLE(), &rc);
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;

	int dPosX = pt.x - GET_MOUSE_POSITION().x;
	int dPosY = pt.y - GET_MOUSE_POSITION().y;

	static bool first = true;
	if (first)
	{
		dPosX = 0;
		dPosY = 0;
		first = false;
	}
	fixed += dPosY;
	if (fixed > 372)
	{
		fixed -= dPosY;
		dPosY = 0;
	}
	else if (fixed < -372)
	{
		fixed -= dPosY;
		dPosY = 0;
	}

	this->rotateByYAxis((float)-dPosX / 5.0f, false);
	this->rotateByXAxis((float)-dPosY / 5.0f);

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

	m_pSkinnedObj->setPosition(cameraObj->getPosition() + D3DXVECTOR3(0.0f, -3.0f, 0.0f) + m_stSkinnedForwardVec3 * 1.5f);

	m_pSkinnedObj->setRightDirection(m_stSkinnedRightVec3);
	m_pSkinnedObj->setUpDirection(m_stSkinnedUpVec3);
	m_pSkinnedObj->setForwardDirection(m_stSkinnedForwardVec3);
}

void player::adjustJump()
{
	if (!m_bIsJump)
	{
		float fTerrainHeight = m_pStage->getTerrainObj()->getHeight(this->getPosition());
		float fPlayerHeight = this->getPosition().y;
		if (fPlayerHeight - fTerrainHeight > 8.0f)
		{
			m_bIsJump = true;
			m_fJumpTime = 0.0f;
			m_fYVelocity = 0.0f;
		}
		else
		{
			D3DXVECTOR3 stPos = { this->getPosition().x,fTerrainHeight + 8.0f,this->getPosition().z };
			this->setPosition(stPos);
		}
	}

	if (m_bIsJump)
	{
		m_fYVelocity += -GRAVITY * m_fJumpTime;
		this->moveByYAxis(m_fYVelocity * GET_DELTA_TIME(), false);
		m_fJumpTime += GET_DELTA_TIME();

		float fTerrainHeight = m_pStage->getTerrainObj()->getHeight(this->getPosition());
		float fPlayerHeight = this->getPosition().y;
		if (m_fYVelocity < 0.0f&&fPlayerHeight - fTerrainHeight < 8.0f)
		{
			D3DXVECTOR3 stPos = { this->getPosition().x,fTerrainHeight + 8.0f,this->getPosition().z };
			this->setPosition(stPos);
			m_fJumpTime = 0.0f;
			m_fYVelocity = 0.0f;
			m_bIsJump = false;
		}
	}
}

void player::adjustCollisionArea()
{
	if (m_pStage->getTerrainObj() != nullptr)
	{
		int nWidth = m_pStage->getTerrainObj()->getCXTerrain();
		int nHeight = m_pStage->getTerrainObj()->getCZTerrain();

		if (m_fTopLeft.x < -nWidth / 2) m_fTopLeft.x = -nWidth / 2;
		if (m_fTopLeft.y < -nHeight / 2)m_fTopLeft.y = -nHeight / 2;


		if (m_fBottomRight.x > nWidth / 2) m_fBottomRight.x = nWidth / 2;
		if (m_fBottomRight.y > nHeight / 2)m_fBottomRight.y = nHeight / 2;

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
		int xTopLeftIndex = (int)stTopLeftPos.x;
		int zTopLeftIndex = (int)stTopLeftPos.z;
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

void player::checkPaperObj()
{
	STRay stRay;
	stRay.m_stDirection = cameraObj->getForwardDirection();
	stRay.m_stOrigin = cameraObj->getPosition();
	for (auto iter : m_pStage->getPaperObjList())
	{
		if (IsIntersectRaySphere(stRay, iter->getFinalBoundingSphere())&&!iter->getbIsGet())
		{
			D3DXVECTOR3 deltaVec = iter->getFinalBoundingSphere().m_stCenter - stRay.m_stOrigin;
			float deltaLength = D3DXVec3Length(&deltaVec);
			if (deltaLength < 60.0f)
				iter->getbOutLineDraw() = true;
			else
				iter->getbOutLineDraw() = false;
		}
		else
		{
			iter->getbOutLineDraw() = false;
		}
	}

	if (IS_MOUSE_BUTTON_PRESSED(EMouseInput::LEFT))
	{
		for (auto iter : m_pStage->getPaperObjList())
		{
			if (!iter->getbIsGet()&& iter->getbOutLineDraw())
			{
				
				static int nMaxPage;
				GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Pick.wav", false);
				iter->getbIsGet() = true;
				m_nPage++;
			}
		}
	}

}


bool player::checkCollisionTerrain(EDirection a_eDirection)
{
	bool isCollision = false;
	if (a_eDirection == EDirection::FRONT)
	{
		m_stSkinnedRay.m_stOrigin = cameraObj->getPosition();
		D3DXVec3Normalize(&m_stSkinnedRay.m_stDirection, &m_stSkinnedForwardVec3);
		//m_stSkinnedRay.m_stDirection = m_stSkinnedForwardVec3;
	}
	else if (a_eDirection == EDirection::BACK)
	{
		m_stSkinnedRay.m_stOrigin = cameraObj->getPosition();
		m_stSkinnedRay.m_stDirection = -m_stSkinnedForwardVec3;
	}
	else if (a_eDirection == EDirection::RIGHT)
	{
		m_stSkinnedRay.m_stOrigin = cameraObj->getPosition();
		m_stSkinnedRay.m_stDirection = -m_stSkinnedRightVec3;
	}
	else if (a_eDirection == EDirection::LEFT)
	{
		m_stSkinnedRay.m_stOrigin = cameraObj->getPosition();
		m_stSkinnedRay.m_stDirection = m_stSkinnedRightVec3;
	}

	int nTerrainWidth = m_pStage->getTerrainObj()->getCXTerrain();
	int nTerrainHeight = m_pStage->getTerrainObj()->getCZTerrain();
	int nWidth = m_pStage->getTerrainObj()->getCXDIB();
	int nScaleX = m_pStage->getTerrainObj()->getScale().x;
	int nScaleZ = m_pStage->getTerrainObj()->getScale().z;


	D3DXVECTOR3 stTopLeftPos = D3DXVECTOR3((m_fTopLeft.x + nTerrainWidth / 2) / nScaleX, 0, (m_fTopLeft.y + nTerrainHeight / 2) / nScaleZ);
	D3DXVECTOR3 stBottomRightPos = D3DXVECTOR3((m_fBottomRight.x + nTerrainWidth / 2) / nScaleX, 0, (m_fBottomRight.y + nTerrainHeight / 2) / nScaleZ);
	int xTopLeftIndex = (int)stTopLeftPos.x;
	int zTopLeftIndex = (int)stTopLeftPos.z;
	int xBottomRightIndex = (int)stBottomRightPos.x;
	int zBottomRightIndex = (int)stBottomRightPos.z;

	int dxIndex = xBottomRightIndex - xTopLeftIndex;
	int dyIndex = zBottomRightIndex - zTopLeftIndex;

	D3DXVECTOR3* pHeightMap = m_pStage->getTerrainObj()->getHeightMap();
	D3DXVECTOR3 pPos[3];
	for (int i = 0; i <= dyIndex; i++)
	{
		for (int j = 0; j <= dxIndex; j++)
		{
			int nIndex = (zTopLeftIndex + i)*nWidth + (xTopLeftIndex + j);
			pPos[0] = D3DXVECTOR3(pHeightMap[nIndex]);
			pPos[1] = D3DXVECTOR3(pHeightMap[nIndex + 1]);
			pPos[2] = D3DXVECTOR3(pHeightMap[nIndex + nWidth]);
			float fDistance = 0.0f;
			if (D3DXIntersectTri(&pPos[0], &pPos[1], &pPos[2], &m_stSkinnedRay.m_stOrigin, &m_stSkinnedRay.m_stDirection, NULL, NULL, &fDistance))
			{
				if (fDistance < 60.0f)
				{
					isCollision = true;
					break;
				}
			}
			pPos[0] = D3DXVECTOR3(pHeightMap[nIndex + nWidth + 1]);
			pPos[1] = D3DXVECTOR3(pHeightMap[nIndex + nWidth]);
			pPos[2] = D3DXVECTOR3(pHeightMap[nIndex + 1]);
			if (D3DXIntersectTri(&pPos[0], &pPos[1], &pPos[2], &m_stSkinnedRay.m_stOrigin, &m_stSkinnedRay.m_stDirection, NULL, NULL, &fDistance))
			{
				if (fDistance < 60.0f)
				{
					isCollision = true;
					break;
				}
			}
		}
		if (isCollision)break;
	}
	ZeroMemory(&m_stSkinnedRay, sizeof(m_stSkinnedRay));
	return isCollision;
}