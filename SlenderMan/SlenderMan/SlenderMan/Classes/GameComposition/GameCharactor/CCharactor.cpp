#include "CCharactor.h"

CCharactor::CCharactor()
{
}

CCharactor::~CCharactor(void)
{
}

void CCharactor::init(void)
{
}

void CCharactor::update(void)
{
	CObject::update();
}

void CCharactor::preDraw(void)
{
}

void CCharactor::doDraw(void)
{
}

void CCharactor::postDraw(void)
{
}

CSkinnedObject * CCharactor::createSkinnedMesh(CSkinnedObject::STParameters stParameters)
{
	m_pSkinnedParameters = stParameters;

	return new CSkinnedObject(m_pSkinnedParameters);
}

CStaticObject * CCharactor::createStaticMesh(CStaticObject::STParameters stParameters)
{
	m_pStaticParameters = stParameters;

	return new CStaticObject(m_pStaticParameters);
}
