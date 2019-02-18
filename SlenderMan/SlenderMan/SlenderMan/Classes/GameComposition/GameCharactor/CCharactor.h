#pragma once

#include "../../Define/KGlobalDefine.h"
#include "../../Utility/Base/CRenderObject.h"
#include "../../Utility/Object/SkinnedObject/CSkinnedObject.h"
#include "../../Utility/Object/StaticObject/CStaticObject.h"

class CCharactor : public CRenderObject
{
public:
	CCharactor();
	virtual ~CCharactor(void);

public:
	virtual void init(void);
	virtual void update(void) override;

	virtual void preDraw(void) override;
	virtual void doDraw(void) override;
	virtual void postDraw(void) override;



public:		//getter, setter
	CSkinnedObject::STParameters getSkinnedParameters() { return m_pSkinnedParameters; }
	void setSkinnedParameters(CSkinnedObject::STParameters stParameters) { m_pSkinnedParameters = stParameters; }

	CStaticObject::STParameters getStaticParameters() { return m_pStaticParameters; }
	void setStaticParameters(CStaticObject::STParameters stParameters) { m_pStaticParameters = stParameters; }

public:
	CSkinnedObject* createSkinnedMesh(CSkinnedObject::STParameters stParameters);
	CStaticObject* createStaticMesh(CStaticObject::STParameters stParameters);

protected:
	CSkinnedObject::STParameters m_pSkinnedParameters;
	CStaticObject::STParameters m_pStaticParameters;

	STRay m_stRay;


};