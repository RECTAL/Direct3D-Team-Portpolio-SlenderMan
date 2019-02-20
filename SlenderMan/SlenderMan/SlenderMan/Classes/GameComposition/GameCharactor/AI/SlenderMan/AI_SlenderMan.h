#pragma once
#include "../../../../Define/KGlobalDefine.h"
#include "../../CCharactor.h"


class CStage;
class CStaticObject;
class CSpotLightObject;
class player;
class slenderman :public CCharactor
{
public:
	slenderman(CStaticObject::STParameters a_stParameters);
	virtual ~slenderman(void);

public:
	virtual void init(void) override;
	virtual void update(void) override;

	virtual void preDraw(void) override;
	virtual void doDraw(void) override;
	virtual void postDraw(void) override;

public:		//getter,setter
	/**********************************************/
	//getter
	/**********************************************/
	bool	getbIsSpawn() { return m_bIsSpawn; }
	CStaticObject*	getStaticObj() { return m_pStaticObj; }

	/**********************************************/
	//setter
	/**********************************************/
	void	setStage(CStage* a_pStage) { m_pStage = a_pStage; }
	void	setPlayer(player* a_pPlayer) { m_pPlayer = a_pPlayer; }

public:
	void	spawnSlenderMan();
	void	addSpotLight(CSpotLightObject* a_pSpotLight);
	void	delSpotLight();
private:
	void	setStaticRotate();
	void	createSphereMesh();



private:
	CStage*			m_pStage = nullptr;
	player*			m_pPlayer = nullptr;
	CStaticObject*	m_pStaticObj = nullptr;


	D3DXVECTOR3		m_stStaticRightVec3;
	D3DXVECTOR3		m_stStaticUpVec3;
	D3DXVECTOR3		m_stStaticForwardVec3;


	LPD3DXMESH		m_pSphere = nullptr;

	bool			m_bIsSpawn = false;
	float			m_fSpawnTime = 0.0f;
	bool			reSpwan = true;
};