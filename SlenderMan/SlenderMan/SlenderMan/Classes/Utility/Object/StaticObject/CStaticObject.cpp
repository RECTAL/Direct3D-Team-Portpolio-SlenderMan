#include "CStaticObject.h"
#include "../../../Function/GlobalFunction.h"
#include "../../Base/CDirect3DApplication.h"
#include "../CameraObject/CCameraObject.h"
#include "../../Manager/CResourceManager.h"

CStaticObject::CStaticObject(STParameters & a_rstParameters)
	:
	m_stParameters(a_rstParameters)
{
	m_pEffect = GET_EFFECT(a_rstParameters.m_oEffectFilepath);
	m_stStaticMesh = GET_STATIC_MESH(a_rstParameters.m_oMeshFilepath);
}

void CStaticObject::doDraw(void)
{
	CRenderObject::doDraw();
	/*
	
	
	
	
	Do Draw!!!!!!
	

	
	
	*/
}
