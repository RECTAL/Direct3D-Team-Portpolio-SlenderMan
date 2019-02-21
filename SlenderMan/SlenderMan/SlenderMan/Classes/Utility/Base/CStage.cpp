#include "CStage.h"
#include "../Base/CRenderObject.h"
#include "../System/CollisionSystem/CollisionSystem_CFrustum.h"
#include "../Object/SkinnedObject/CSkinnedObject.h"
#include "../Object/StaticObject/CStaticObject.h"
#include "../Object/CameraObject/CCameraObject.h"
#include "../Object/LightObject/CLightObject.h"
#include "../../GameComposition/GameObject/Decorate/CDecorate_SoundObj.h"
#include "../../GameComposition//GameObject/Decorate/CDecorate_BillboardObj.h"
#include "../../Function/GlobalFunction.h"
#include "../Manager/CWindowManager.h"
#include "../Manager/CDeviceManager.h"
#include "../Manager/CRendertargetManager.h"

CStage::~CStage()
{
	this->release();
	SAFE_DELETE(m_pDirectionLightObj);
}

void CStage::init(CTerrainObject::STParameters a_stParameters, std::string m_oObjPacketListFilePath)
{
	m_pDirectionLightObj = new CLightObject(0);
	m_pDirectionLightObj->setPosition(D3DXVECTOR3(0.0f, 0.0f, -50.0f));
	m_pDirectionLightObj->rotateByXAxis(D3DXToRadian(-45.0f));
	m_pDirectionLightObj->update();
	a_stParameters.m_pLight = m_pDirectionLightObj;


	this->createRenderTarget();
	this->load(a_stParameters, m_oObjPacketListFilePath);
}



void CStage::release()
{
	if (m_pTerrainObj != nullptr)
	{
		int fWidth = m_pTerrainObj->getCXDIB();
		int fHeight = m_pTerrainObj->getCZDIB();
		if (m_pObjPacketList != nullptr)
		{
			delete[] m_pObjPacketList;
			m_pObjPacketList = nullptr;
		}
		if (m_pObjList != nullptr)
		{
			for (int i = 0; i < fWidth*fHeight; i++)
			{
				for (auto iter : m_pObjList[i])
				{
					SAFE_DELETE(iter);
				}
				m_pObjList[i].clear();
			}
			delete[] m_pObjList;
			m_pObjList = nullptr;
		}
		m_oSoundObjList.clear();
		m_oPaperObjList.clear();
		m_oRenderObjList.clear();
		SAFE_DELETE(m_pTerrainObj);
	}

}


void CStage::load(CTerrainObject::STParameters a_stParameters, std::string m_oObjPacketListFilePath)
{
	this->release();
	a_stParameters.m_pLight = m_pDirectionLightObj;
	m_pTerrainObj = new CTerrainObject(a_stParameters);

	int fWidth = m_pTerrainObj->getCXDIB();
	int fHeight = m_pTerrainObj->getCZDIB();

	m_pObjPacketList = new OBJCONTAINER[fWidth*fHeight];
	for (int i = 0; i < fWidth*fHeight; i++)
	{
		for (int j = 0; j < MAX_OBJ_CAPACITY; j++)
		{
			m_pObjPacketList[i].m_nPivot = 0;
			m_pObjPacketList[i].m_nObjCapacity[j] = GOUST_VALUE;
			ZeroMemory(&m_pObjPacketList[i].m_stObjPacket[j], sizeof(m_pObjPacketList[i].m_stObjPacket[j]));
		}
	}

	m_pObjList = new std::vector<CRenderObject*>[fWidth*fHeight];

	if (m_oObjPacketListFilePath != "")
	{
		HANDLE	hFile = CreateFile(
			_T(m_oObjPacketListFilePath.c_str()),
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		DWORD dwRead;
		if (!ReadFile(hFile, m_pObjPacketList, sizeof(OBJCONTAINER)*fWidth*fHeight, &dwRead, NULL))assert("Fail");
		for (int i = 0; i < fWidth*fHeight; i++)
		{
			for (int j = 0; j < MAX_OBJ_CAPACITY; j++)
			{
				if (m_pObjPacketList[i].m_nObjCapacity[j] == GOUST_VALUE)break;
				else
				{
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::DESK)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/desk/desk.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stStaticObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stStaticObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stStaticObj->getObjClasses() = EObjClasses::STATIC;
						m_pObjList[i].push_back(stStaticObj);
						m_oRenderObjList.push_back(stStaticObj);
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::GRASS)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/grass/grass.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stStaticObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stStaticObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stStaticObj->getObjClasses() = EObjClasses::STATIC;
						m_pObjList[i].push_back(stStaticObj);
						m_oRenderObjList.push_back(stStaticObj);
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::MOUNTAIN)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/mountain/mountain.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stStaticObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stStaticObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stStaticObj->getObjClasses() = EObjClasses::STATIC;
						m_pObjList[i].push_back(stStaticObj);
						m_oRenderObjList.push_back(stStaticObj);
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::OLDHOUSE)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/oldHouse/oldHouse.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stStaticObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stStaticObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stStaticObj->getObjClasses() = EObjClasses::STATIC;
						m_pObjList[i].push_back(stStaticObj);
						m_oRenderObjList.push_back(stStaticObj);

					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::OLDWOODDOCK)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/oldWoodDock/oldWoodDock.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stStaticObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stStaticObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stStaticObj->getObjClasses() = EObjClasses::STATIC;
						m_pObjList[i].push_back(stStaticObj);
						m_oRenderObjList.push_back(stStaticObj);

					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::PLANTS)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/plants/plants.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stStaticObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stStaticObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stStaticObj->getObjClasses() = EObjClasses::STATIC;
						m_pObjList[i].push_back(stStaticObj);
						m_oRenderObjList.push_back(stStaticObj);

					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::ROUNDWOOD)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/roundwood/roundwood.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stStaticObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stStaticObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stStaticObj->getObjClasses() = EObjClasses::STATIC;
						m_pObjList[i].push_back(stStaticObj);
						m_oRenderObjList.push_back(stStaticObj);
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::LOG)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/log/log.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stStaticObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stStaticObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stStaticObj->getObjClasses() = EObjClasses::STATIC;
						m_pObjList[i].push_back(stStaticObj);
						m_oRenderObjList.push_back(stStaticObj);
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::WOODHOUSE)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/woodHouse/woodHouse.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stStaticObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stStaticObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stStaticObj->getObjClasses() = EObjClasses::STATIC;
						m_pObjList[i].push_back(stStaticObj);
						m_oRenderObjList.push_back(stStaticObj);
					}
				
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::CRIKET)
					{
						CDecorate_SoundObj::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj
						};
						CDecorate_SoundObj* stSoundObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::CRIKET);
						stSoundObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stSoundObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stSoundObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stSoundObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stSoundObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stSoundObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stSoundObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stSoundObj->getObjClasses() = EObjClasses::DECORATE_SOUND;
						m_pObjList[i].push_back(stSoundObj);
						m_oSoundObjList.push_back(stSoundObj);
						m_oRenderObjList.push_back(stSoundObj);
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::CROW)
					{
						CDecorate_SoundObj::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj
						};
						CDecorate_SoundObj* stSoundObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::CROW);
						stSoundObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stSoundObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stSoundObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stSoundObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stSoundObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stSoundObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stSoundObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stSoundObj->getObjClasses() = EObjClasses::DECORATE_SOUND;
						m_pObjList[i].push_back(stSoundObj);
						m_oSoundObjList.push_back(stSoundObj);
						m_oRenderObjList.push_back(stSoundObj);
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::FIRE)
					{
						CDecorate_SoundObj::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj
						};
						CDecorate_SoundObj* stSoundObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::FIRE);
						stSoundObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stSoundObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stSoundObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stSoundObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stSoundObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stSoundObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stSoundObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stSoundObj->getObjClasses() = EObjClasses::DECORATE_SOUND;
						m_pObjList[i].push_back(stSoundObj);
						m_oSoundObjList.push_back(stSoundObj);
						m_oRenderObjList.push_back(stSoundObj);
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::OWL)
					{
						CDecorate_SoundObj::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj
						};
						CDecorate_SoundObj* stSoundObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::OWL);
						stSoundObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stSoundObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stSoundObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stSoundObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stSoundObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stSoundObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stSoundObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stSoundObj->getObjClasses() = EObjClasses::DECORATE_SOUND;
						m_pObjList[i].push_back(stSoundObj);
						m_oSoundObjList.push_back(stSoundObj);
						m_oRenderObjList.push_back(stSoundObj);
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::WIND)
					{
						CDecorate_SoundObj::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj
						};
						CDecorate_SoundObj* stSoundObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::WIND);
						stSoundObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stSoundObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stSoundObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stSoundObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stSoundObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stSoundObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stSoundObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stSoundObj->getObjClasses() = EObjClasses::DECORATE_SOUND;
						m_pObjList[i].push_back(stSoundObj);
						m_oSoundObjList.push_back(stSoundObj);
						m_oRenderObjList.push_back(stSoundObj);
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::RAIN)
					{
						CDecorate_SoundObj::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj
						};
						CDecorate_SoundObj* stSoundObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::RAIN);
						stSoundObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stSoundObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stSoundObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stSoundObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stSoundObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
						stSoundObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
						stSoundObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
						stSoundObj->getObjClasses() = EObjClasses::DECORATE_SOUND;
						m_pObjList[i].push_back(stSoundObj);
						m_oSoundObjList.push_back(stSoundObj);
						m_oRenderObjList.push_back(stSoundObj);
					}
					for (int k = 0; k < 8; k++) {
						char cTreeMeshIndex[MAX_PATH];
						sprintf(cTreeMeshIndex, "Resources/Meshes/tree%d/tree%d.X", k + 1, k + 1);
						if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == ((EObjType)((int)EObjType::TREE_1 + k)))
						{
							CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
							CLightObject** ppPointLightObj = new CLightObject*[10];
							CStaticObject::STParameters stParameters = {
								m_pCameraObj,m_pDirectionLightObj,
								0,ppSpotLightObj,
								0,ppPointLightObj,
								cTreeMeshIndex,
								"Resources/Effects/DefaultStaticMesh.fx"
							};
							CStaticObject* stStaticObj = new CStaticObject(stParameters);
							stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
							stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
							stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
							stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
							stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
							stStaticObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
							stStaticObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
							stStaticObj->getObjClasses() = EObjClasses::STATIC;
							m_pObjList[i].push_back(stStaticObj);
							m_oRenderObjList.push_back(stStaticObj);
						}
					}
					for (int k = 0; k < 8; k++) {
						char TextureIndex[MAX_PATH];
						sprintf(TextureIndex, "Resources/Textures/object/page%d", k + 1);
						if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == ((EObjType)((int)EObjType::PAGE_1 + k)))
						{
							CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
							CLightObject** ppPointLightObj = new CLightObject*[10];
							CDecorate_BillboardObj::STParameters stParameters = {
								m_pCameraObj,m_pDirectionLightObj,
								0,ppSpotLightObj,
								0,ppPointLightObj,
								TextureIndex,"png",1,
								"Resources/Effects/DefaultStaticMesh.fx"
							};
							CDecorate_BillboardObj* stBillboardObj = new CDecorate_BillboardObj(stParameters);
							stBillboardObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
							stBillboardObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
							stBillboardObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
							stBillboardObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
							stBillboardObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);
							stBillboardObj->getbIsCollision() = m_pObjPacketList[i].m_stObjPacket[j].m_bIsCollision;
							stBillboardObj->getObjType() = m_pObjPacketList[i].m_stObjPacket[j].m_EObjType;
							stBillboardObj->getObjClasses() = EObjClasses::DECORATE_BILLBOARD;
							m_pObjList[i].push_back(stBillboardObj);
							m_oPaperObjList.push_back(stBillboardObj);
							m_oRenderObjList.push_back(stBillboardObj);
						}
					}
				}
			}
		}
		CloseHandle(hFile);
	}
}

void CStage::addObj(OBJPACKET& a_stPacket, D3DXVECTOR3 a_stPosition, bool a_bIsDebug)
{
	int nIndex = m_pTerrainObj->findIndex(a_stPosition);
	for (int i = 0; i < MAX_OBJ_CAPACITY; i++)
	{
		if (m_pObjPacketList[nIndex].m_nObjCapacity[i] == GOUST_VALUE)
		{
			m_pObjPacketList[nIndex].m_nObjCapacity[i] = (int)a_stPacket.m_EObjType;
			m_pObjPacketList[nIndex].m_stObjPacket[i].m_EClasses = a_stPacket.m_EClasses;
			m_pObjPacketList[nIndex].m_stObjPacket[i] = a_stPacket;

			if (a_stPacket.m_EObjType == EObjType::DESK)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/desk/desk.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->getbIsCollision() = a_stPacket.m_bIsCollision;
				stStaticObj->getObjType() = a_stPacket.m_EObjType;
				stStaticObj->getObjClasses() = EObjClasses::STATIC;
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
				m_oRenderObjList.push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::GRASS)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/grass/grass.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->getbIsCollision() = a_stPacket.m_bIsCollision;
				stStaticObj->getObjType() = a_stPacket.m_EObjType;
				stStaticObj->getObjClasses() = EObjClasses::STATIC;
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
				m_oRenderObjList.push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::MOUNTAIN)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/mountain/mountain.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->getbIsCollision() = a_stPacket.m_bIsCollision;
				stStaticObj->getObjType() = a_stPacket.m_EObjType;
				stStaticObj->getObjClasses() = EObjClasses::STATIC;
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
				m_oRenderObjList.push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::OLDHOUSE)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/oldHouse/oldHouse.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->getbIsCollision() = a_stPacket.m_bIsCollision;
				stStaticObj->getObjType() = a_stPacket.m_EObjType;
				stStaticObj->getObjClasses() = EObjClasses::STATIC;
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
				m_oRenderObjList.push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::OLDWOODDOCK)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/oldWoodDock/oldWoodDock.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->getbIsCollision() = a_stPacket.m_bIsCollision;
				stStaticObj->getObjType() = a_stPacket.m_EObjType;
				stStaticObj->getObjClasses() = EObjClasses::STATIC;
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
				m_oRenderObjList.push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::PLANTS)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/plants/plants.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->getbIsCollision() = a_stPacket.m_bIsCollision;
				stStaticObj->getObjType() = a_stPacket.m_EObjType;
				stStaticObj->getObjClasses() = EObjClasses::STATIC;
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
				m_oRenderObjList.push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::ROUNDWOOD)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/roundwood/roundwood.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->getbIsCollision() = a_stPacket.m_bIsCollision;
				stStaticObj->getObjType() = a_stPacket.m_EObjType;
				stStaticObj->getObjClasses() = EObjClasses::STATIC;
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
				m_oRenderObjList.push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::LOG)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/log/log.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->getbIsCollision() = a_stPacket.m_bIsCollision;
				stStaticObj->getObjType() = a_stPacket.m_EObjType;
				stStaticObj->getObjClasses() = EObjClasses::STATIC;
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
				m_oRenderObjList.push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::WOODHOUSE)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/woodHouse/woodHouse.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->getbIsCollision() = a_stPacket.m_bIsCollision;
				stStaticObj->getObjType() = a_stPacket.m_EObjType;
				stStaticObj->getObjClasses() = EObjClasses::STATIC;
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
				m_oRenderObjList.push_back(stStaticObj);
			}

			else if (a_stPacket.m_EObjType == EObjType::CRIKET)
			{
				CDecorate_SoundObj::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj
				};
				CDecorate_SoundObj* stSoundObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::CRIKET);
				stSoundObj->setPosition(a_stPacket.m_stPosition);
				stSoundObj->setScale(a_stPacket.m_stScale);
				stSoundObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stSoundObj->setUpDirection(a_stPacket.m_stUpVec);
				stSoundObj->setRightDirection(a_stPacket.m_stRightVec);
				stSoundObj->getbIsCollision() = a_stPacket.m_bIsCollision & 0;
				stSoundObj->getObjType() = a_stPacket.m_EObjType;
				stSoundObj->getObjClasses() = EObjClasses::DECORATE_SOUND;
				stSoundObj->setVisible(false);
				m_pObjList[nIndex].push_back(stSoundObj);
				m_oSoundObjList.push_back(stSoundObj);
				m_oRenderObjList.push_back(stSoundObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::CROW)
			{
				CDecorate_SoundObj::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj
				};
				CDecorate_SoundObj* stSoundObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::CROW);
				stSoundObj->setPosition(a_stPacket.m_stPosition);
				stSoundObj->setScale(a_stPacket.m_stScale);
				stSoundObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stSoundObj->setUpDirection(a_stPacket.m_stUpVec);
				stSoundObj->setRightDirection(a_stPacket.m_stRightVec);
				stSoundObj->getbIsCollision() = a_stPacket.m_bIsCollision & 0;
				stSoundObj->getObjType() = a_stPacket.m_EObjType;
				stSoundObj->getObjClasses() = EObjClasses::DECORATE_SOUND;
				stSoundObj->setVisible(false);
				m_pObjList[nIndex].push_back(stSoundObj);
				m_oSoundObjList.push_back(stSoundObj);
				m_oRenderObjList.push_back(stSoundObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::FIRE)
			{
				CDecorate_SoundObj::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj
				};
				CDecorate_SoundObj* stSoundObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::FIRE);
				stSoundObj->setPosition(a_stPacket.m_stPosition);
				stSoundObj->setScale(a_stPacket.m_stScale);
				stSoundObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stSoundObj->setUpDirection(a_stPacket.m_stUpVec);
				stSoundObj->setRightDirection(a_stPacket.m_stRightVec);
				stSoundObj->getbIsCollision() = a_stPacket.m_bIsCollision & 0;
				stSoundObj->getObjType() = a_stPacket.m_EObjType;
				stSoundObj->getObjClasses() = EObjClasses::DECORATE_SOUND;
				stSoundObj->setVisible(false);
				m_pObjList[nIndex].push_back(stSoundObj);
				m_oSoundObjList.push_back(stSoundObj);
				m_oRenderObjList.push_back(stSoundObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::OWL)
			{
				CDecorate_SoundObj::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj
				};
				CDecorate_SoundObj* stSoundObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::OWL);
				stSoundObj->setPosition(a_stPacket.m_stPosition);
				stSoundObj->setScale(a_stPacket.m_stScale);
				stSoundObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stSoundObj->setUpDirection(a_stPacket.m_stUpVec);
				stSoundObj->setRightDirection(a_stPacket.m_stRightVec);
				stSoundObj->getbIsCollision() = a_stPacket.m_bIsCollision & 0;
				stSoundObj->getObjType() = a_stPacket.m_EObjType;
				stSoundObj->getObjClasses() = EObjClasses::DECORATE_SOUND;
				stSoundObj->setVisible(false);
				m_pObjList[nIndex].push_back(stSoundObj);
				m_oSoundObjList.push_back(stSoundObj);
				m_oRenderObjList.push_back(stSoundObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::RAIN)
			{
				CDecorate_SoundObj::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj
				};
				CDecorate_SoundObj* stSoundObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::RAIN);
				stSoundObj->setPosition(a_stPacket.m_stPosition);
				stSoundObj->setScale(a_stPacket.m_stScale);
				stSoundObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stSoundObj->setUpDirection(a_stPacket.m_stUpVec);
				stSoundObj->setRightDirection(a_stPacket.m_stRightVec);
				stSoundObj->getbIsCollision() = a_stPacket.m_bIsCollision & 0;
				stSoundObj->getObjType() = a_stPacket.m_EObjType;
				stSoundObj->getObjClasses() = EObjClasses::DECORATE_SOUND;
				stSoundObj->setVisible(false);
				m_pObjList[nIndex].push_back(stSoundObj);
				m_oSoundObjList.push_back(stSoundObj);
				m_oRenderObjList.push_back(stSoundObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::WIND)
			{
				CDecorate_SoundObj::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj
				};
				CDecorate_SoundObj* stSoundObj = new CDecorate_SoundObj(stParameters, EPlayingBGM::WIND);
				stSoundObj->setPosition(a_stPacket.m_stPosition);
				stSoundObj->setScale(a_stPacket.m_stScale);
				stSoundObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stSoundObj->setUpDirection(a_stPacket.m_stUpVec);
				stSoundObj->setRightDirection(a_stPacket.m_stRightVec);
				stSoundObj->getbIsCollision() = a_stPacket.m_bIsCollision & 0;
				stSoundObj->getObjType() = a_stPacket.m_EObjType;
				stSoundObj->getObjClasses() = EObjClasses::DECORATE_SOUND;
				stSoundObj->setVisible(false);
				m_pObjList[nIndex].push_back(stSoundObj);
				m_oSoundObjList.push_back(stSoundObj);
				m_oRenderObjList.push_back(stSoundObj);
			}
			for (int i = 0; i < 8; i++) {
				char cTreeMeshIndex[MAX_PATH];
				sprintf(cTreeMeshIndex, "Resources/Meshes/tree%d/tree%d.X", i + 1, i + 1);
				if (a_stPacket.m_EObjType == ((EObjType)((int)EObjType::TREE_1 + i)))
				{
					CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
					CLightObject** ppPointLightObj = new CLightObject*[10];
					CStaticObject::STParameters stParameters =
					{
						m_pCameraObj,m_pDirectionLightObj,
						0,ppSpotLightObj,
						0,ppPointLightObj,
						cTreeMeshIndex,
						"Resources/Effects/DefaultStaticMesh.fx"
					};
					CStaticObject* stStaticObj = new CStaticObject(stParameters);
					stStaticObj->setPosition(a_stPacket.m_stPosition);
					stStaticObj->setScale(a_stPacket.m_stScale);
					stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
					stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
					stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
					stStaticObj->getbIsCollision() = a_stPacket.m_bIsCollision;
					stStaticObj->getObjType() = a_stPacket.m_EObjType;
					stStaticObj->getObjClasses() = EObjClasses::STATIC;
					stStaticObj->setVisible(false);
					stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
					m_pObjList[nIndex].push_back(stStaticObj);
					m_oRenderObjList.push_back(stStaticObj);
				}
			}
			for (int i = 0; i < 8; i++) {
				char cTextureIndex[MAX_PATH];
				sprintf(cTextureIndex, "Resources/Textures/object/page%d", i + 1);
				if (a_stPacket.m_EObjType == ((EObjType)((int)EObjType::PAGE_1 + i)))
				{
					CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
					CLightObject** ppPointLightObj = new CLightObject*[10];
					CDecorate_BillboardObj::STParameters stParameters =
					{
						m_pCameraObj, m_pDirectionLightObj,
						0,ppSpotLightObj,
						0,ppPointLightObj,
						cTextureIndex,"png",1,
						"Resources/Effects/DefaultStaticMesh.fx"
					};
					CDecorate_BillboardObj* stBillboardObj = new CDecorate_BillboardObj(stParameters);
					stBillboardObj->setPosition(a_stPacket.m_stPosition);
					stBillboardObj->setScale(a_stPacket.m_stScale);
					stBillboardObj->setForwardDirection(a_stPacket.m_stForwordVec);
					stBillboardObj->setUpDirection(a_stPacket.m_stUpVec);
					stBillboardObj->setRightDirection(a_stPacket.m_stRightVec);
					stBillboardObj->getbIsCollision() = a_stPacket.m_bIsCollision;
					stBillboardObj->getObjType() = a_stPacket.m_EObjType;
					stBillboardObj->getObjClasses() = EObjClasses::DECORATE_BILLBOARD;
					stBillboardObj->setVisible(false);
					m_pObjList[nIndex].push_back(stBillboardObj);
					m_oPaperObjList.push_back(stBillboardObj);
					m_oRenderObjList.push_back(stBillboardObj);
				}
			}

			m_pObjPacketList[nIndex].m_nPivot++;
			break;
		}
	}
}

void CStage::delObj(CRenderObject * a_pRenderObj, D3DXVECTOR3 a_stPosition)
{
	int nIndex = m_pTerrainObj->findIndex(a_stPosition);
	for (int i = 0; i < m_pObjList[nIndex].size(); i++)
	{
		if (m_pObjList[nIndex][i] == a_pRenderObj)
		{
			if (m_pObjList[nIndex][i]->getObjClasses() == EObjClasses::DECORATE_SOUND){
				std::vector<CDecorate_SoundObj*>::iterator iter = std::find_if(m_oSoundObjList.begin(), m_oSoundObjList.end(), [=](CDecorate_SoundObj* a_pSoundObj)->bool {
					if (a_pSoundObj == m_pObjList[nIndex][i])return true;
					else return false;
				});

				if (iter != m_oSoundObjList.end())
				{
					m_oSoundObjList.erase(iter);
				}
			}

			if (m_pObjList[nIndex][i]->getObjClasses() == EObjClasses::DECORATE_BILLBOARD) {
				std::vector<CDecorate_BillboardObj*>::iterator iter = std::find_if(m_oPaperObjList.begin(), m_oPaperObjList.end(), [=](CDecorate_BillboardObj* a_pBillboardObj)->bool {
					if (a_pBillboardObj == m_pObjList[nIndex][i])return true;
					else return false;
				});

				if (iter != m_oPaperObjList.end())
				{
					m_oPaperObjList.erase(iter);
				}
			}
			std::vector<CRenderObject*>::iterator iter = std::find_if(m_oRenderObjList.begin(), m_oRenderObjList.end(), [=](CRenderObject* a_pRenderObj)->bool {
				if (a_pRenderObj == m_pObjList[nIndex][i])return true;
				else return false;
			});

			if (iter != m_oRenderObjList.end())
			{
				m_oRenderObjList.erase(iter);
			}
			for (int j = i; j < MAX_OBJ_CAPACITY - 1; j++)
			{
				m_pObjPacketList[nIndex].m_nObjCapacity[j] = m_pObjPacketList[nIndex].m_nObjCapacity[j + 1];
				m_pObjPacketList[nIndex].m_stObjPacket[j] = m_pObjPacketList[nIndex].m_stObjPacket[j + 1];
			}
			SAFE_DELETE(m_pObjList[nIndex][i]);
			m_pObjList[nIndex].erase(m_pObjList[nIndex].begin() + i);
			m_pObjPacketList[nIndex].m_nPivot--;


			break;
		}
	}
}

void CStage::setCameraObjMain(CCameraObject * a_pCameraObj)
{
	m_pCameraObj = a_pCameraObj;
	m_pTerrainObj->getSTParameters().m_pCamera = a_pCameraObj;
	setCameraObj(a_pCameraObj);
}

void CStage::setCameraObj(CCameraObject * a_pCameraObj)
{
	CCameraObject* pCameraObj = a_pCameraObj;
	if (m_pObjList != nullptr&&m_pTerrainObj != nullptr)
	{
		int fWidth = m_pTerrainObj->getCXDIB();
		int fHeight = m_pTerrainObj->getCZDIB();


		for (int i = 0; i < fWidth*fHeight; i++)
		{
			for (int j = 0; j < MAX_OBJ_CAPACITY; j++)
			{
				if (m_pObjPacketList[i].m_nObjCapacity[j] == GOUST_VALUE)break;
				else
				{
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::SKINNED)
					{
						CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(m_pObjList[i][j]);


					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::STATIC)
					{
						CStaticObject* pStaticObj = dynamic_cast<CStaticObject*>(m_pObjList[i][j]);
						pStaticObj->getSTParameters().m_pCamera = pCameraObj;
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::DECORATE_SOUND)
					{
						CDecorate_SoundObj* pSoundObj = dynamic_cast<CDecorate_SoundObj*>(m_pObjList[i][j]);
						pSoundObj->getSTParameters().m_pCamera = pCameraObj;
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::DECORATE_BILLBOARD)
					{
						CDecorate_BillboardObj* pBillboard = dynamic_cast<CDecorate_BillboardObj*>(m_pObjList[i][j]);
						pBillboard->getSTParameters().m_pCamera = pCameraObj;
					}
				}
			}
		}
	}
}

void CStage::setObjEffectTechname(std::string a_stTechname)
{
	if (m_pObjList != nullptr&&m_pTerrainObj != nullptr)
	{
		int fWidth = m_pTerrainObj->getCXDIB();
		int fHeight = m_pTerrainObj->getCZDIB();


		for (int i = 0; i < fWidth*fHeight; i++)
		{
			for (int j = 0; j < MAX_OBJ_CAPACITY; j++)
			{
				if (m_pObjPacketList[i].m_nObjCapacity[j] == GOUST_VALUE)break;
				else
				{
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::SKINNED)
					{
						CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(m_pObjList[i][j]);

					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::STATIC)
					{
						CStaticObject* pStaticObj = dynamic_cast<CStaticObject*>(m_pObjList[i][j]);
						pStaticObj->getTechniqueName() = a_stTechname;
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::DECORATE_BILLBOARD)
					{
						CDecorate_BillboardObj* pBillboard = dynamic_cast<CDecorate_BillboardObj*>(m_pObjList[i][j]);
						pBillboard->getTechniqueName() = a_stTechname;
					}

				}
			}
		}
	}
}

void CStage::setObjDebugMode(bool a_bIsDebug, EDebugDrawType a_eDebugMode)
{
	EDebugDrawType eDebugMode = a_eDebugMode;
	if (m_pObjList != nullptr&&m_pTerrainObj != nullptr)
	{
		int fWidth = m_pTerrainObj->getCXDIB();
		int fHeight = m_pTerrainObj->getCZDIB();


		for (int i = 0; i < fWidth*fHeight; i++)
		{
			for (int j = 0; j < MAX_OBJ_CAPACITY; j++)
			{
				if (m_pObjPacketList[i].m_nObjCapacity[j] == GOUST_VALUE)break;
				if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::DECORATE_SOUND)continue;
				else
				{
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::SKINNED)
					{
						CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(m_pObjList[i][j]);

					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::STATIC)
					{
						CStaticObject* pStaticObj = dynamic_cast<CStaticObject*>(m_pObjList[i][j]);
						pStaticObj->setDebugEnable(a_bIsDebug, eDebugMode);
					}
				}
			}
		}
	}
}

void CStage::addSpotLightObj(CSpotLightObject * a_pSpotLightObj)
{
	CSpotLightObject* pSpotLight = a_pSpotLightObj;
	if (m_pObjList != nullptr&&m_pTerrainObj != nullptr)
	{
		int fWidth = m_pTerrainObj->getCXDIB();
		int fHeight = m_pTerrainObj->getCZDIB();


		for (int i = 0; i < fWidth*fHeight; i++)
		{
			for (int j = 0; j < MAX_OBJ_CAPACITY; j++)
			{
				if (m_pObjPacketList[i].m_nObjCapacity[j] == GOUST_VALUE)break;
				if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::DECORATE_SOUND)continue;
				else
				{
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::SKINNED)
					{
						CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(m_pObjList[i][j]);


					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::STATIC)
					{
						CStaticObject* pStaticObj = dynamic_cast<CStaticObject*>(m_pObjList[i][j]);
						int nNumSpot = ++pStaticObj->getSTParameters().m_nNumSpotLight;
						if (nNumSpot < 10)
							pStaticObj->getSTParameters().m_pSpotLight[nNumSpot - 1] = pSpotLight;
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::DECORATE_BILLBOARD)
					{
						CDecorate_BillboardObj* pBillboard = dynamic_cast<CDecorate_BillboardObj*>(m_pObjList[i][j]);
						int nNumSpot = ++pBillboard->getSTParameters().m_nNumSpotLight;
						if (nNumSpot < 10)
							pBillboard->getSTParameters().m_pSpotLight[nNumSpot - 1] = pSpotLight;
					}
				}
			}
		}
	}
}

void CStage::delSpotLightObj()
{
	if (m_pObjList != nullptr&&m_pTerrainObj != nullptr)
	{
		int fWidth = m_pTerrainObj->getCXDIB();
		int fHeight = m_pTerrainObj->getCZDIB();


		for (int i = 0; i < fWidth*fHeight; i++)
		{
			for (int j = 0; j < MAX_OBJ_CAPACITY; j++)
			{
				if (m_pObjPacketList[i].m_nObjCapacity[j] == GOUST_VALUE)break;
				if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::DECORATE_SOUND)continue;
				else
				{
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::SKINNED)
					{
						CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(m_pObjList[i][j]);


					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::STATIC)
					{
						CStaticObject* pStaticObj = dynamic_cast<CStaticObject*>(m_pObjList[i][j]);
						pStaticObj->getSTParameters().m_nNumSpotLight = max(pStaticObj->getSTParameters().m_nNumSpotLight - 1, 0);
						int nNumSpot = pStaticObj->getSTParameters().m_nNumSpotLight;
						pStaticObj->getSTParameters().m_pSpotLight[nNumSpot] = nullptr;
					}
					else if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::DECORATE_BILLBOARD)
					{
						CDecorate_BillboardObj* pBillboard = dynamic_cast<CDecorate_BillboardObj*>(m_pObjList[i][j]);
						pBillboard->getSTParameters().m_nNumSpotLight = max(pBillboard->getSTParameters().m_nNumSpotLight - 1, 0);
						int nNumSpot = pBillboard->getSTParameters().m_nNumSpotLight;
						pBillboard->getSTParameters().m_pSpotLight[nNumSpot] = nullptr;
					}
				}
			}
		}
	}
}

void CStage::save(std::string m_oObjPacketListFilePath)
{
	if (m_pObjList != nullptr&&m_pTerrainObj != nullptr)
	{
		int fWidth = m_pTerrainObj->getCXDIB();
		int fHeight = m_pTerrainObj->getCZDIB();
		HANDLE	hFile = CreateFile(
			_T(m_oObjPacketListFilePath.c_str()),
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		DWORD dwWrite;
		WriteFile(hFile, m_pObjPacketList, sizeof(OBJCONTAINER)*fWidth*fHeight, &dwWrite, NULL);
		CloseHandle(hFile);
	}
}


void CStage::update()
{
	m_pTerrainObj->update();
	int fWidth = m_pTerrainObj->getCXDIB();
	int fHeight = m_pTerrainObj->getCZDIB();

	for (int i = 0; i < fWidth*fHeight; i++)
	{
		for (int j = 0; j < MAX_OBJ_CAPACITY; j++)
		{
			if (m_pObjPacketList[i].m_nObjCapacity[j] == GOUST_VALUE)break;
			else
			{
				if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::SKINNED)
				{
					CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(m_pObjList[i][j]);

				}
				else if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::STATIC)
				{
					CStaticObject* pStaticObj = dynamic_cast<CStaticObject*>(m_pObjList[i][j]);
					pStaticObj->update();
				}
				else if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::DECORATE_SOUND)
				{
					CDecorate_SoundObj* pSoundObj = dynamic_cast<CDecorate_SoundObj*>(m_pObjList[i][j]);
					pSoundObj->update();
				}
				else if (m_pObjPacketList[i].m_stObjPacket[j].m_EClasses == EObjClasses::DECORATE_BILLBOARD)
				{
					CDecorate_BillboardObj* pBillboard = dynamic_cast<CDecorate_BillboardObj*>(m_pObjList[i][j]);
					pBillboard->update();
				}
			}
		}
	}
}

void CStage::draw()
{
	m_pTerrainObj->draw();

	int fWidth = m_pTerrainObj->getCXDIB();
	int fHeight = m_pTerrainObj->getCZDIB();
	if (!m_bIsMaptool)
	{
		GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("OutlineRenderTarget")->m_stRenderTarget.m_pTexSurf);
		GET_DEVICE()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0.0f);
		GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("OutlineMeshRenderTarget")->m_stRenderTarget.m_pTexSurf);
		GET_DEVICE()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0.0f);
		GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pTexSurf);
	}
	D3DXMATRIXA16	stWorldMatrix;
	D3DXMATRIXA16	stViewMatrix;
	D3DXMATRIXA16	stProjectionMatrix;


	
	for (auto iter : m_oRenderObjList)
	{
		if (iter->getObjClasses() == EObjClasses::SKINNED)
		{
			CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(iter);

		}
		else if (iter->getObjClasses() == EObjClasses::STATIC)
		{
			CStaticObject* pStaticObj = dynamic_cast<CStaticObject*>(iter);
			if (pStaticObj->getSTParameters().m_pCamera->getCameraFrustum()->IsInSphere(pStaticObj->getFinalBoundingSphere()))
			{
				if (!pStaticObj->getVisible())pStaticObj->setVisible(true);
				else
				{
					
					if (pStaticObj->getbOutLineDraw()&&!m_bIsMaptool)
					{
						/********************************************/
						//OutlineRenderTarget에 Draw
						/********************************************/
						GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("OutlineRenderTarget")->m_stRenderTarget.m_pTexSurf);
					
						D3DXMATRIXA16	stWorldMatrix = pStaticObj->getFinalWorldMatrix();
						D3DXMATRIXA16	stViewMatrix = pStaticObj->getSTParameters().m_pCamera->getViewMatrix();
						D3DXMATRIXA16	stProjectionMatrix = pStaticObj->getSTParameters().m_pCamera->getProjectionMatrix();
					
						GET_DEVICE()->SetRenderState(D3DRS_ZWRITEENABLE, false);
					
						FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineEffect->SetMatrix("g_stWorldMatrix", &stWorldMatrix);
						FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineEffect->SetMatrix("g_stViewMatrix", &stViewMatrix);
						FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineEffect->SetMatrix("g_stProjectionMatrix", &stProjectionMatrix);
					
					
						RunEffectLoop(FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineEffect, "outline", [=](int nPassNum)->void {
							for (int i = 0; i < pStaticObj->getStaticMesh().m_nNumMaterials; ++i) {
								pStaticObj->getStaticMesh().m_pStaticMesh->DrawSubset(i);
							}
						});
					
						GET_DEVICE()->SetRenderState(D3DRS_ZWRITEENABLE, true);
						/********************************************/
						//OutlineMeshRenderTarget에 Draw
						/********************************************/
						GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("OutlineMeshRenderTarget")->m_stRenderTarget.m_pTexSurf);
						pStaticObj->draw();

						GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pTexSurf);

					}
					else
					{
						if(!m_bIsMaptool)
							GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pTexSurf);
						pStaticObj->getbOutLineDraw() = false;
						pStaticObj->draw();
					}
				}
			}
		}
		else if (iter->getObjClasses() == EObjClasses::DECORATE_SOUND)
		{
			if (m_bIsMaptool)
			{
				CDecorate_SoundObj* pSoundObj = dynamic_cast<CDecorate_SoundObj*>(iter);
				if (!pSoundObj->getVisible())pSoundObj->setVisible(true);
				else
					pSoundObj->draw();
			}
		}
		else if (iter->getObjClasses() == EObjClasses::DECORATE_BILLBOARD)
		{
			CDecorate_BillboardObj* pBillboard = dynamic_cast<CDecorate_BillboardObj*>(iter);
			if (pBillboard->getSTParameters().m_pCamera->getCameraFrustum()->IsInSphere(pBillboard->getFinalBoundingSphere()))
			{
				if (!pBillboard->getVisible())pBillboard->setVisible(true);
				else
				{
					
					if (pBillboard->getbOutLineDraw() && !m_bIsMaptool)
					{
						/********************************************/
						//OutlineRenderTarget에 Draw
						/********************************************/
						GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("OutlineRenderTarget")->m_stRenderTarget.m_pTexSurf);

						D3DXMATRIXA16	stWorldMatrix = pBillboard->getFinalWorldMatrix();
						D3DXMATRIXA16	stViewMatrix = pBillboard->getSTParameters().m_pCamera->getViewMatrix();
						D3DXMATRIXA16	stProjectionMatrix = pBillboard->getSTParameters().m_pCamera->getProjectionMatrix();

						GET_DEVICE()->SetRenderState(D3DRS_ZWRITEENABLE, false);

						FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineEffect->SetMatrix("g_stWorldMatrix", &stWorldMatrix);
						FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineEffect->SetMatrix("g_stViewMatrix", &stViewMatrix);
						FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineEffect->SetMatrix("g_stProjectionMatrix", &stProjectionMatrix);


						RunEffectLoop(FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineEffect, "outline", [=](int nPassNum)->void {
							pBillboard->getMesh()->DrawSubset(0);
						});

						GET_DEVICE()->SetRenderState(D3DRS_ZWRITEENABLE, true);
						/********************************************/
						//OutlineMeshRenderTarget에 Draw
						/********************************************/
						GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("OutlineMeshRenderTarget")->m_stRenderTarget.m_pTexSurf);
						pBillboard->draw();

						GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pTexSurf);

					}
					else
					{
						if (!m_bIsMaptool)
							GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pTexSurf);
						pBillboard->getbOutLineDraw() = false;
						if(!pBillboard->getbIsGet())
							pBillboard->draw();
					}
				}
			}

		}
	}

	/***************************************************/
	//StageRenderTarget에 draw
	/***************************************************/
	if (!m_bIsMaptool)
	{
		GET_DEVICE()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pTexSurf);

		D3DXMatrixIdentity(&stWorldMatrix);
		FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineBlurEffect->SetMatrix("g_stWorldMatrix", &stWorldMatrix);

		FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineBlurEffect->SetTexture("g_pOutlineRenderTexture", FIND_RENDERTARGET("OutlineRenderTarget")->m_stRenderTarget.m_pTex);
		FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineBlurEffect->SetTexture("g_pRenderTexture", FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pTex);
		FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineBlurEffect->SetVector("g_stOutlineColor", &D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
		FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineBlurEffect->SetFloat("g_fSize", GET_WINDOW_SIZE().cx);

		RunEffectLoop(FIND_RENDERTARGET("OutlineRenderTarget")->m_pOutlineBlurEffect, "outlineBlur", [=](int nPassNum)->void {
			FIND_RENDERTARGET("OutlineRenderTarget")->getPlaneMesh()->DrawSubset(0);
		});
		
		FIND_RENDERTARGET("OutlineMeshRenderTarget")->m_pBlendEffect->SetMatrix("g_stWorldMatrix", &stWorldMatrix);
		FIND_RENDERTARGET("OutlineMeshRenderTarget")->m_pBlendEffect->SetTexture("g_pTexture", FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pTex);
		FIND_RENDERTARGET("OutlineMeshRenderTarget")->m_pBlendEffect->SetTexture("g_pBlendTexture", FIND_RENDERTARGET("OutlineMeshRenderTarget")->m_stRenderTarget.m_pTex);

		RunEffectLoop(FIND_RENDERTARGET("OutlineMeshRenderTarget")->m_pBlendEffect, "BlendTexture", [=](int nPassNum)->void {
			FIND_RENDERTARGET("OutlineMeshRenderTarget")->getPlaneMesh()->DrawSubset(0);
		});
		GET_DEVICE()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

bool CStage::getPickingPosWithTerrain(D3DXVECTOR3& a_stPosition)
{
	return m_pTerrainObj->terrainPicking(a_stPosition);
}

void CStage::createRenderTarget()
{
	D3DVIEWPORT9 stViewport;
	GET_DEVICE()->GetViewport(&stViewport);
	int nWidth = GET_WINDOW_SIZE().cx;
	int nHeight = GET_WINDOW_SIZE().cy;

	GET_RENDERTARGET_MANAGER()->addRenderTarget("OutlineRenderTarget", new CRenderTarget(nWidth, nHeight, &stViewport));
	GET_RENDERTARGET_MANAGER()->addRenderTarget("OutlineMeshRenderTarget", new CRenderTarget(nWidth, nHeight, &stViewport));
}
