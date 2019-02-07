#include "CStage.h"

void CStage::init(CTerrainObject::STParameters a_stParameters, std::string m_oObjPacketListFilePath)
{
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
			SAFE_DELETE_ARRAY(m_pObjPacketList);
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
			SAFE_DELETE_ARRAY(m_pObjList);
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
	m_pObjList			= new std::vector<CRenderObject*>[fWidth*fHeight];


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
	
	ReadFile(hFile, m_pObjPacketList, fWidth*fHeight, &dwRead, NULL);
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
						NULL,NULL,
						0,NULL,
						0,NULL,
						"Resources/Meshes/tree1/tree.X",
						"Resources/Effects/DefaultStaticMesh.fx"
					};
					m_pObjList[i].push_back(new CStaticObject(stParameters));
				}
				if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_2)
				{
					CStaticObject::STParameters stParameters = {
						NULL,NULL,
						0,NULL,
						0,NULL,
						"Resources/Meshes/tree2/tree2.X",
						"Resources/Effects/DefaultStaticMesh.fx"
					};
					m_pObjList[i].push_back(new CStaticObject(stParameters));
				}
				if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_3)
				{
					CStaticObject::STParameters stParameters = {
						NULL,NULL,
						0,NULL,
						0,NULL,
						"Resources/Meshes/tree3/tree3.X",
						"Resources/Effects/DefaultStaticMesh.fx"
					};
					m_pObjList[i].push_back(new CStaticObject(stParameters));
				}
				if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_4)
				{
					CStaticObject::STParameters stParameters = {
						NULL,NULL,
						0,NULL,
						0,NULL,
						"Resources/Meshes/tree4/tree4.X",
						"Resources/Effects/DefaultStaticMesh.fx"
					};
					m_pObjList[i].push_back(new CStaticObject(stParameters));
				}
				if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_5)
				{
					CStaticObject::STParameters stParameters = {
						NULL,NULL,
						0,NULL,
						0,NULL,
						"Resources/Meshes/tree5/tree5.X",
						"Resources/Effects/DefaultStaticMesh.fx"
					};
					m_pObjList[i].push_back(new CStaticObject(stParameters));
				}
				if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_6)
				{
					CStaticObject::STParameters stParameters = {
						NULL,NULL,
						0,NULL,
						0,NULL,
						"Resources/Meshes/tree6/tree6.X",
						"Resources/Effects/DefaultStaticMesh.fx"
					};
					m_pObjList[i].push_back(new CStaticObject(stParameters));
				}
				if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_7)
				{
					CStaticObject::STParameters stParameters = {
						NULL,NULL,
						0,NULL,
						0,NULL,
						"Resources/Meshes/tree7/tree7.X",
						"Resources/Effects/DefaultStaticMesh.fx"
					};
					m_pObjList[i].push_back(new CStaticObject(stParameters));
				}
				if (m_pObjPacketList[i].m_stObjPacket[j].m_EObjType == EObjType::TREE_8)
				{
					CStaticObject::STParameters stParameters = {
						NULL,NULL,
						0,NULL,
						0,NULL,
						"Resources/Meshes/tree8/tree8.X",
						"Resources/Effects/DefaultStaticMesh.fx"
					};
					m_pObjList[i].push_back(new CStaticObject(stParameters));
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
		WriteFile(hFile,m_pObjList, fWidth*fHeight,&dwWrite,NULL);
	}
}


void CStage::update()
{
}

void CStage::draw()
{
}
