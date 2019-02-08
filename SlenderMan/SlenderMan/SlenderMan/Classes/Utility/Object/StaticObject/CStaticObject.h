#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CRenderObject.h"

//! StaticObject
class CCameraObject;
class CLightObject;
class CSpotLightObject;
class CStaticObject : public CRenderObject
{
public:
	struct STParameters
	{
		CCameraObject*	m_pCamera;
		CLightObject*	m_pDirectional;
		
		int	m_nNumSpotLight;
		CSpotLightObject*	m_pSpotLight;

		int m_nNumPointLight;
		CLightObject*	m_pPointLight;

		std::string m_oMeshFilepath;
		std::string m_oEffectFilepath;
	};

public:			//constructor , destructor
	CStaticObject(STParameters &a_rstParameters);

protected:		//interface		

	virtual void doDraw(void) override;		//! interface : IRenderable


public:
	STStaticMesh	getStaticMesh() { return m_stStaticMesh; }
	LPD3DXEFFECT	getEffect() { return m_pEffect; }
	std::string&	getTechniqueName() { return m_stTechniqueName; }
	STParameters&	getSTParameters() { return m_stParameters; }

	virtual void update(void) override;		//! interface : IUpdateable

private:
	STBoundingSphere createBoundingSphere();
	LPD3DXMESH		 createDebugMesh();

protected:
	LPD3DXEFFECT m_pEffect = nullptr;

	STStaticMesh m_stStaticMesh;
	STParameters m_stParameters;
	LPD3DXMESH	 m_pDebugMesh;
	std::string	m_stTechniqueName;

private:
	float	m_fTmpRadius = 0.0f;

};
