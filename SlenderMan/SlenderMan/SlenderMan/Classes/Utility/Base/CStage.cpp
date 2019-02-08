#include "CStage.h"
#include "../Base/CRenderObject.h"
#include "../System/CollisionSystem/CollisionSystem_CFrustum.h"
#include "../Object/SkinnedObject/CSkinnedObject.h"
#include "../Object/StaticObject/CStaticObject.h"
#include "../Object/CameraObject/CCameraObject.h"
#include "../Object/LightObject/CLightObject.h"

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
	m_pTerrainObj = new CTerrainObject(a_stParameters);
	
	int fWidth  = m_pTerrainObj->getCXDIB();
	int fHeight = m_pTerrainObj->getCZDIB();

	m_pObjPacketList	= new OBJCONTAINER[fWidth*fHeight];
	for (int i = 0; i < fWidth*fHeight; i++)
	{
		for (int j = 0; j < MAX_OBJ_CAPACITY; j++)
		{
			m_pObjPacketList[i].m_nPivot = 0;
			m_pObjPacketList[i].m_nObjCapacity[j] = GOUST_VALUE;
			ZeroMemory(&m_pObjPacketList[i].m_stObjPacket[j], sizeof(m_pObjPacketList[i].m_stObjPacket[j]));
		}
	}

	m_pObjList			= new std::vector<CRenderObject*>[fWidth*fHeight];

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
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_1)
					{
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,NULL,
							0,NULL,
							"Resources/Meshes/tree1/tree.X",
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
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,NULL,
							0,NULL,
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
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,NULL,
							0,NULL,
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
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,NULL,
							0,NULL,
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
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,NULL,
							0,NULL,
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
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,NULL,
							0,NULL,
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
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_7)
					{
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,NULL,
							0,NULL,
							"Resources/Meshes/tree7/tree7.X",
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
					if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_8)
					{
						CStaticObject::STParameters stParameters = {
							m_pCameraObj,m_pDirectionLightObj,
							0,NULL,
							0,NULL,
							"Resources/Meshes/tree8/tree8.X",
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

void CStage::addObj(OBJPACKET& a_stPacket, D3DXVECTOR3 a_stPosition)
{
	int nIndex = m_pTerrainObj->findIndex(a_stPosition);
	for (int i = 0; i < MAX_OBJ_CAPACITY; i++)
	{
		if (m_pObjPacketList[nIndex].m_nObjCapacity[i] == GOUST_VALUE)
		{
			m_pObjPacketList[nIndex].m_nObjCapacity[i] = (int)a_stPacket.m_EObjType;
			m_pObjPacketList[nIndex].m_stObjPacket[i] = a_stPacket;


			if (a_stPacket.m_EObjType == EObjType::TREE_1)
			{
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,NULL,
					0,NULL,
					"Resources/Meshes/tree1/tree.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->setVisible(false);
				m_pObjList[nIndex].push_back(stStaticObj);
				int a = 0;
			}
			else if (a_stPacket.m_EObjType == EObjType::TREE_2)
			{
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,NULL,
					0,NULL,
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
				m_pObjList[nIndex].push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::TREE_3)
			{
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,NULL,
					0,NULL,
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
				m_pObjList[nIndex].push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::TREE_4)
			{
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,NULL,
					0,NULL,
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
				m_pObjList[nIndex].push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::TREE_5)
			{
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,NULL,
					0,NULL,
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
				m_pObjList[nIndex].push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::TREE_6)
			{
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,NULL,
					0,NULL,
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
				m_pObjList[nIndex].push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::TREE_7)
			{
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,NULL,
					0,NULL,
					"Resources/Meshes/tree7/tree7.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->setVisible(false);
				m_pObjList[nIndex].push_back(stStaticObj);
			}
			else if (a_stPacket.m_EObjType == EObjType::TREE_8)
			{
				CStaticObject::STParameters stParameters =
				{
					m_pCameraObj,m_pDirectionLightObj,
					0,NULL,
					0,NULL,
					"Resources/Meshes/tree8/tree8.X",
					"Resources/Effects/DefaultStaticMesh.fx"
				};
				CStaticObject* stStaticObj = new CStaticObject(stParameters);
				stStaticObj->setPosition(a_stPacket.m_stPosition);
				stStaticObj->setScale(a_stPacket.m_stScale);
				stStaticObj->setForwardDirection(a_stPacket.m_stForwordVec);
				stStaticObj->setUpDirection(a_stPacket.m_stUpVec);
				stStaticObj->setRightDirection(a_stPacket.m_stRightVec);
				stStaticObj->setVisible(false);
				m_pObjList[nIndex].push_back(stStaticObj);
			}
			m_pObjPacketList[nIndex].m_nPivot ++;
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
			for (int j = i; j < m_pObjPacketList[nIndex].m_nPivot-1; j++)
			{
				m_pObjPacketList[nIndex].m_nObjCapacity[j] = m_pObjPacketList[nIndex].m_nObjCapacity[j + 1];
				m_pObjPacketList[nIndex].m_stObjPacket[j] = m_pObjPacketList[nIndex].m_stObjPacket[j + 1];
			}
			for (int j = m_pObjPacketList[nIndex].m_nPivot; j < MAX_OBJ_CAPACITY; j++)
			{
				m_pObjPacketList[nIndex].m_nObjCapacity[j] = GOUST_VALUE;
				ZeroMemory(&m_pObjPacketList[nIndex].m_stObjPacket[j], sizeof(m_pObjPacketList[nIndex].m_stObjPacket[j]));
			}
			m_pObjPacketList[nIndex].m_nPivot--;
			break;
		}
	}
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
		WriteFile(hFile,m_pObjPacketList, sizeof(OBJCONTAINER)*fWidth*fHeight,&dwWrite,NULL);
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
					//if (pStaticObj->getSTParameters().m_pCamera->getCameraFrustum()->IsInSphere(pStaticObj->getBoundingSphere()))
					//{
					if (!pStaticObj->getVisible())pStaticObj->setVisible(true);
					else pStaticObj->draw();
					//}
				}
			}
		}
	}
}

bool CStage::getPickingPosWithTerrain(D3DXVECTOR3& a_stPosition)
{
	return m_pTerrainObj->terrainPicking(a_stPosition);
}
