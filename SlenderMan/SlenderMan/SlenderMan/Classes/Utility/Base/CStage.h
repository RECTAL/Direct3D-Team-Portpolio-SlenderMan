#pragma once
#include "../../Define/KGlobalDefine.h"
#include "../Object/StaticObject/CStaticObject.h"
#include "../Object/SkinnedObject/CSkinnedObject.h"
#include "../Object/TerrainObject/CTerrainObject.h"


#define MAX_OBJ_CAPACITY		10
#define GOUST_VALUE				-1

class CStage
{
private:
	typedef struct objPacket
	{
		EObjType	m_EObjType		= EObjType::NONE;
		BOOL		m_bIsSkinned	= false;
		D3DXVECTOR3 m_stPosition	= D3DXVECTOR3(0.0f,0.0f,0.0f);
		D3DXVECTOR3 m_stForwordVec	= WORLD_FORWARD_DIRECTION;
		D3DXVECTOR3 m_stUpVec		= WORLD_UP_DIRECTION;
		D3DXVECTOR3 m_stRightVec	= WORLD_RIGHT_DIRECTION;
		D3DXVECTOR3 m_stScale		= D3DXVECTOR3(1.0f,0.0f,0.0f);
	}OBJPACKET;

	typedef struct objContainer
	{
		int			m_nObjCapacity[MAX_OBJ_CAPACITY];
		OBJPACKET	m_stObjPacket[MAX_OBJ_CAPACITY];
	}OBJCONTAINER;
public:
	
	void	init(CTerrainObject::STParameters a_stParameters, std::string m_oObjPacketListFilePath);
	void	release();
	void	load(CTerrainObject::STParameters a_stParameters,std::string m_oObjPacketListFilePath);
	void	save(std::string m_oObjPacketListFilePath);
	void	update();
	void	draw();


private:
	CTerrainObject*	m_pTerrainObj;
	OBJCONTAINER* m_pObjPacketList;
	std::vector<CRenderObject*>* m_pObjList;
};