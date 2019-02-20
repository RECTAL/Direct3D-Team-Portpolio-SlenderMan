#include "AI_SlenderMan.h"
#include "./../../../../Function/GlobalFunction.h"
#include "../../../../Utility/Object/StaticObject/CStaticObject.h"
#include "../../../../Utility/Manager/CTimeManager.h"
#include "../../../../Utility/Manager/CDeviceManager.h"
#include "../../../GameCharactor/Player/player.h"
#include "../../../../Utility/Base/CStage.h"

slenderman::slenderman(CStaticObject::STParameters a_stParameters)
{
	m_pStaticObj = this->createStaticMesh(a_stParameters);
	
	this->createSphereMesh();

	auto stBoundingBox = CreateBoundingBox(m_pSphere);
	auto stBoundingSphere = CreateBoundingSphere(m_pSphere);

	this->setBoundingBox(stBoundingBox);
	this->setBoundingSphere(stBoundingSphere);
	this->setScale(D3DXVECTOR3(5.0f, 5.0f, 5.0f));

	m_pStaticObj->setScale(D3DXVECTOR3(7.0f, 7.0f, 7.0f));
}

slenderman::~slenderman(void)
{
	SAFE_DELETE(m_pStaticObj);
}

void slenderman::init(void)
{
	m_bIsSpawn = false;
	m_fSpawnTime = 0.0f;
}

void slenderman::update(void)
{
	CCharactor::update();
	m_pStaticObj->setPosition(this->getPosition());
	setStaticRotate();
	m_pStaticObj->update();
	if (m_bIsSpawn)
	{
		m_fSpawnTime -= GET_DELTA_TIME();
		if (m_fSpawnTime < 0.0f)
		{
			m_fSpawnTime = 0.0f;
			m_bIsSpawn = false;
		}
	}
}

void slenderman::preDraw(void)
{
	CCharactor::preDraw();
}

void slenderman::doDraw(void)
{
	CCharactor::doDraw();
	if(m_bIsSpawn)
		m_pStaticObj->draw();
}

void slenderman::postDraw(void)
{
	CCharactor::postDraw();
}

void slenderman::setStaticRotate()
{
	if (m_pPlayer != nullptr)
	{
		m_stStaticForwardVec3 = m_pPlayer->getPosition() - this->getPosition();
		D3DXVec3Normalize(&m_stStaticForwardVec3, &m_stStaticForwardVec3);

		m_stStaticUpVec3 = WORLD_UP_DIRECTION;
		D3DXVec3Normalize(&m_stStaticUpVec3, &m_stStaticUpVec3);

		D3DXVec3Cross(&m_stStaticRightVec3, &m_stStaticUpVec3, &m_stStaticForwardVec3);
		D3DXVec3Cross(&m_stStaticForwardVec3, &m_stStaticRightVec3, &m_stStaticUpVec3);

		D3DXVec3Normalize(&m_stStaticForwardVec3, &m_stStaticForwardVec3);
		D3DXVec3Normalize(&m_stStaticUpVec3, &m_stStaticUpVec3);
		D3DXVec3Normalize(&m_stStaticRightVec3, &m_stStaticRightVec3);


		m_pStaticObj->setForwardDirection(m_stStaticForwardVec3);
		m_pStaticObj->setUpDirection(m_stStaticUpVec3);
		m_pStaticObj->setRightDirection(m_stStaticRightVec3);
	}
}

void slenderman::spawnSlenderMan()
{
	if (!m_bIsSpawn)
	{
		m_bIsSpawn = true;
		m_fSpawnTime = RandomFloatValue(8.0f, 12.0f);

		if (m_pPlayer != nullptr)
		{
			D3DXVECTOR3 stPosition = m_pPlayer->getPosition();
			int rXVal = RandomIntValue(1, 2);
			int rZVal = RandomIntValue(1, 2);
			if (rXVal == 1)
			{
				stPosition.x += RandomFloatValue(50.0f, 70.0f);
			}
			else
			{
				stPosition.x -= RandomFloatValue(50.0f, 70.0f);
			}
			
			if (rZVal == 1)
			{
				stPosition.z += RandomFloatValue(50.0f, 70.0f);
			}
			else
			{
				stPosition.z -= RandomFloatValue(50.0f, 70.0f);
			}
			stPosition.y = m_pStage->getTerrainObj()->getHeight(stPosition);
			this->setPosition(stPosition);
		}
	}
}

void slenderman::addSpotLight(CSpotLightObject * a_pSpotLight)
{
	int nNumSpot = ++m_pStaticObj->getSTParameters().m_nNumSpotLight;
	if (nNumSpot < 10)
		m_pStaticObj->getSTParameters().m_pSpotLight[nNumSpot - 1] = a_pSpotLight;
}

void slenderman::delSpotLight()
{
	m_pStaticObj->getSTParameters().m_nNumSpotLight = max(m_pStaticObj->getSTParameters().m_nNumSpotLight - 1, 0);
	int nNumSpot = m_pStaticObj->getSTParameters().m_nNumSpotLight;
	m_pStaticObj->getSTParameters().m_pSpotLight[nNumSpot] = nullptr;
}

void slenderman::createSphereMesh()
{
	if (m_pSphere != nullptr)SAFE_RELEASE(m_pSphere);
	D3DXCreateSphere(GET_DEVICE(), 3.0f, 50.0f, 50.0f, &m_pSphere, NULL);
}