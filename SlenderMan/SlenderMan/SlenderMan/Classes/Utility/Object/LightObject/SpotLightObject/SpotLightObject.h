#pragma once
#include "../../../../Define/KGlobalDefine.h"
#include "../CLightObject.h"

class CSpotLightObject :public CLightObject
{
public:
	CSpotLightObject(int a_nIndex,float a_fRange,float a_fTheta,float a_fPhi);

public:			// interface 
	virtual void update(void) override;

public:
	float m_fRange;
	float m_fTheta;
	float m_fPhi;
};