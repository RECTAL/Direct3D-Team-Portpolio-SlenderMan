#pragma once
#include "../../../../Define/KGlobalDefine.h"
#include "../CLightObject.h"
#include "../../../Interface/IRenderable.h"

class CCameraObject;
class CSpotLightObject :public CLightObject,IRenderable
{
public:
	CSpotLightObject(int a_nIndex,float a_fRange,float a_fTheta,float a_fPhi);
	virtual ~CSpotLightObject();
public:			// interface 
	virtual void update(void) override;			//interface : IUpdateable
	virtual void draw(void) override;			//interface	: IRenderable

	void	setCameraObj(CCameraObject* obj) { m_pCameraObj = obj; }

private:
	LPD3DXMESH		createSpotMesh();
	LPD3DXEFFECT	createSpotEffect();

public:
	float m_fRange;
	float m_fTheta;
	float m_fPhi;

	CCameraObject*	m_pCameraObj = nullptr;
	STStaticMesh 	m_stSpotMesh;
	LPD3DXEFFECT	m_pEffect = nullptr;
};