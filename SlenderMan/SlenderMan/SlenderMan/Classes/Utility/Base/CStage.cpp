#include "CStage.h"
#include "../Base/CRenderObject.h"
#include "../System/CollisionSystem/CollisionSystem_CFrustum.h"
#include "../Object/SkinnedObject/CSkinnedObject.h"
#include "../Object/StaticObject/CStaticObject.h"
#include "../Object/CameraObject/CCameraObject.h"
#include "../Object/LightObject/CLightObject.h"

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
		SAFE_DELETE(m_pTerrainObj);
	}

}


void CStage::load(CTerrainObject::STParameters a_stParameters, std::string m_oObjPacketListFilePath)
{
	release();
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

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::GRASS)
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

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::MOUNTAIN)
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

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::OLDHOUSE)
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

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::OLDWOODDOCK)
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

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::PLANTS)
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

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::ROUNDWOOD)
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

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::URBANDEBRIS)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/urbanDebris/urbanDebris.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::WOODHOUSE)
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

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_1)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/tree1/tree1.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_2)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/tree2/tree2.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_3)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/tree3/tree3.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_4)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/tree4/tree4.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_5)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/tree5/tree5.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);

						m_pObjList[i].push_back(stStaticObj);
					}
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_6)
					{
						CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
						CLightObject** ppPointLightObj = new CLightObject*[10];
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,ppSpotLightObj,
							0,ppPointLightObj,
							"Resources/Meshes/tree6/tree6.X",
							"Resources/Effects/DefaultStaticMesh.fx"
						};
						CStaticObject* stStaticObj = new CStaticObject(stParameters);
						stStaticObj->setPosition(m_pObjPacketList[i].m_stObjPacket[j].m_stPosition);
						stStaticObj->setScale(m_pObjPacketList[i].m_stObjPacket[j].m_stScale);
						stStaticObj->setForwardDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stForwordVec);
						stStaticObj->setUpDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stUpVec);
						stStaticObj->setRightDirection(m_pObjPacketList[i].m_stObjPacket[j].m_stRightVec);

						m_pObjList[i].push_back(stStaticObj);
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
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
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
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
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
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
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
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
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
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
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
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
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
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::URBANDEBRIS)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/urbanDebris/urbanDebris.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
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
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::TREE_1)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/tree1/tree1.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
				int a = 0;
			}
			else if (a_stPacket.m_EObjType == EObjType::TREE_2)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/tree2/tree2.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::TREE_3)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/tree3/tree3.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::TREE_4)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/tree4/tree4.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::TREE_5)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];

				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/tree5/tree5.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::TREE_6)
			{
				CSpotLightObject** ppSpotLightObj = new CSpotLightObject*[10];
				CLightObject** ppPointLightObj = new CLightObject*[10];
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,ppSpotLightObj,
					0,ppPointLightObj,
					"Resources/Meshes/tree6/tree6.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->setVisible(false);
				stStaticObj->setDebugEnable(a_bIsDebug, EDebugDrawType::BOX);
				m_pObjList[nIndex].push_back(stStaticObj);
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
			for (int j = i; j < MAX_OBJ_CAPACITY - 1; j++)
			{
				m_pObjPacketList[nIndex].m_nObjCapacity[j] = m_pObjPacketList[nIndex].m_nObjCapacity[j + 1];
				m_pObjPacketList[nIndex].m_stObjPacket[j] = m_pObjPacketList[nIndex].m_stObjPacket[j + 1];
			}
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
					if (m_pObjPacketList[i].m_stObjPacket[j].m_bIsSkinned)
					{
						CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(m_pObjList[i][j]);


					}
					else
					{
						CStaticObject* pStaticObj = dynamic_cast<CStaticObject*>(m_pObjList[i][j]);
						pStaticObj->getSTParameters().m_pCamera = pCameraObj;
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
					if (m_pObjPacketList[i].m_stObjPacket[j].m_bIsSkinned)
					{
						CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(m_pObjList[i][j]);

					}
					else
					{
						CStaticObject* pStaticObj = dynamic_cast<CStaticObject*>(m_pObjList[i][j]);
						pStaticObj->getTechniqueName() = a_stTechname;
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
				else
				{
					if (m_pObjPacketList[i].m_stObjPacket[j].m_bIsSkinned)
					{
						CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(m_pObjList[i][j]);

					}
					else
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
				else
				{
					if (m_pObjPacketList[i].m_stObjPacket[j].m_bIsSkinned)
					{
						CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(m_pObjList[i][j]);


					}
					else
					{
						CStaticObject* pStaticObj = dynamic_cast<CStaticObject*>(m_pObjList[i][j]);
						int nNumSpot = ++pStaticObj->getSTParameters().m_nNumSpotLight;
						if (nNumSpot < 10)
							pStaticObj->getSTParameters().m_pSpotLight[nNumSpot - 1] = pSpotLight;
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
				else
				{
					if (m_pObjPacketList[i].m_stObjPacket[j].m_bIsSkinned)
					{
						CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(m_pObjList[i][j]);


					}
					else
					{
						CStaticObject* pStaticObj = dynamic_cast<CStaticObject*>(m_pObjList[i][j]);
						pStaticObj->getSTParameters().m_nNumSpotLight = max(pStaticObj->getSTParameters().m_nNumSpotLight - 1, 0);
						int nNumSpot = pStaticObj->getSTParameters().m_nNumSpotLight;
						pStaticObj->getSTParameters().m_pSpotLight[nNumSpot] = nullptr;
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
				if (m_pObjPacketList[i].m_stObjPacket[j].m_bIsSkinned)
				{
					CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(m_pObjList[i][j]);

				}
				else
				{
					CStaticObject* pStaticObj = dynamic_cast<CStaticObject*>(m_pObjList[i][j]);
					pStaticObj->update();
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

	for (int i = 0; i < fWidth*fHeight; i++)
	{
		for (int j = 0; j < MAX_OBJ_CAPACITY; j++)
		{
			if (m_pObjPacketList[i].m_nObjCapacity[j] == GOUST_VALUE)break;
			else
			{
				if (m_pObjPacketList[i].m_stObjPacket[j].m_bIsSkinned)
				{
					CSkinnedObject* pSkinnedObj = dynamic_cast<CSkinnedObject*>(m_pObjList[i][j]);

				}
				else
				{
					CStaticObject* pStaticObj = dynamic_cast<CStaticObject*>(m_pObjList[i][j]);
					if (pStaticObj->getSTParameters().m_pCamera->getCameraFrustum()->IsInSphere(pStaticObj->getFinalBoundingSphere()))
					{
						if (!pStaticObj->getVisible())pStaticObj->setVisible(true);
						else
						{
							
							pStaticObj->getbOutLineDraw() = true;
							if (pStaticObj->getbOutLineDraw()&&!m_bIsMaptool)
							{
								/********************************************/
								//OutlineRenderTarget¿¡ Draw
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
								//OutlineMeshRenderTarget¿¡ Draw
								/********************************************/
								GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("OutlineMeshRenderTarget")->m_stRenderTarget.m_pTexSurf);
								pStaticObj->draw();

								GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pTexSurf);

							}
							else
							{
								if(!m_bIsMaptool)
									GET_DEVICE()->SetRenderTarget(0, FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pTexSurf);
								pStaticObj->draw();
							}
						}
					}
				}
			}
		}
	}

	/***************************************************/
	//StageRenderTarget¿¡ draw
	/***************************************************/
	if (!m_bIsMaptool)
	{
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
