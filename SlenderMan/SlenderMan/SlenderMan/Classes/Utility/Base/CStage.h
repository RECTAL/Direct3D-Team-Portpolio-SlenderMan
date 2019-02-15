#pragma once
#include "../../Define/KGlobalDefine.h"
#include "../Object/StaticObject/CStaticObject.h"
#include "../Object/SkinnedObject/CSkinnedObject.h"
#include "../Object/TerrainObject/CTerrainObject.h"




class CCameraObject;
class CRenderObject;
class CStage
{
public:
	typedef struct objPacket
	{
		EObjType	m_EObjType = EObjType::NONE;
		BOOL		m_bIsSkinned = false;
		BOOL		m_bIsCollision = false;
		D3DXVECTOR3 m_stPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 m_stForwordVec = WORLD_FORWARD_DIRECTION;
		D3DXVECTOR3 m_stUpVec = WORLD_UP_DIRECTION;
		D3DXVECTOR3 m_stRightVec = WORLD_RIGHT_DIRECTION;
		D3DXVECTOR3 m_stScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}OBJPACKET;

	typedef struct objContainer
	{
		int			m_nPivot = 0;
		int			m_nObjCapacity[MAX_OBJ_CAPACITY];
		OBJPACKET	m_stObjPacket[MAX_OBJ_CAPACITY];
	}OBJCONTAINER;
public:

	virtual ~CStage();
public:
	void	init(CTerrainObject::STParameters a_stParameters, std::string m_oObjPacketListFilePath);
	void	release();
	void	load(CTerrainObject::STParameters a_stParameters, std::string m_oObjPacketListFilePath);
	void	addObj(OBJPACKET& a_stPacket, D3DXVECTOR3 a_stPosition, bool a_bIsDebug);
	void	delObj(CRenderObject* a_pRenderObj, D3DXVECTOR3 a_stPosition);
	void	setCameraObjMain(CCameraObject* a_pCameraObj);
	void	setCameraObj(CCameraObject* a_pCameraObj);
	void	setObjEffectTechname(std::string a_stTechname);
	void	setObjDebugMode(bool a_bIsDebug, EDebugDrawType a_eDebugMode);
	void	addSpotLightObj(CSpotLightObject* a_pSpotLightObj);
	void	delSpotLightObj();
	void	save(std::string m_oObjPacketListFilePath);
	void	update();
	void	draw();


	bool	getPickingPosWithTerrain(D3DXVECTOR3& a_stPosition);


	CTerrainObject*	getTerrainObj() { return m_pTerrainObj; }
	CLightObject*	getDirectionalLightObj() { return m_pDirectionLightObj; }
	std::vector<CRenderObject*>*	getObjList() { return m_pObjList; }
	BOOL&	getbIsMaptool() { return m_bIsMaptool; }

private:
	void		createRenderTarget();


private:
	CCameraObject*	m_pCameraObj = nullptr;
	CLightObject*	m_pDirectionLightObj = nullptr;
	CTerrainObject*	m_pTerrainObj = nullptr;
	OBJCONTAINER* m_pObjPacketList = nullptr;
	std::vector<CRenderObject*>* m_pObjList = nullptr;



	BOOL	m_bIsMaptool = false;
};