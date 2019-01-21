#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CRenderObject.h"

//! StaticObject
class CStaticObject : public CRenderObject
{
public:
	struct STParameters
	{
		std::string m_oMeshFilepath;
		std::string m_oEffectFilepath;
	};

public:			//constructor , destructor
	CStaticObject(STParameters &a_rstParameters);

protected:		//interface		

	virtual void doDraw(void) override;		//! interface : IRenderable

protected:
	LPD3DXEFFECT m_pEffect = nullptr;

	STStaticMesh m_stStaticMesh;
	STParameters m_stParameters;
};
