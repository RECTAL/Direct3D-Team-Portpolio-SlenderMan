#include "CDecorate_SoundObj.h"
#include "../../../Utility/Object/CameraObject/CCameraObject.h"
#include "../../../Utility/Object/LightObject/CLightObject.h"
#include "../../../Function/GlobalFunction.h"
#include "../../../Utility/Manager/CDeviceManager.h"
CDecorate_SoundObj::CDecorate_SoundObj(STParameters & a_rstParameters, EPlayingBGM a_eSound)
	:m_stParameters(a_rstParameters), m_eSoundType(a_eSound)
{
	this->createSphereMesh();

	auto stBoundingBox = CreateBoundingBox(m_pMesh);
	auto stBoundingSphere = CreateBoundingSphere(m_pMesh);

	this->setBoundingBox(stBoundingBox);
	this->setBoundingSphere(stBoundingSphere);

}

CDecorate_SoundObj::~CDecorate_SoundObj()
{
	delete[] m_stParameters.m_pSpotLight;
	delete[] m_stParameters.m_pPointLight;
}

void CDecorate_SoundObj::preDraw(void)
{
	D3DXMATRIXA16 stWorldMatrix;
	D3DXMatrixTranslation(&stWorldMatrix, this->getPosition().x, this->getPosition().y, this->getPosition().z);
	GET_DEVICE()->SetTransform(D3DTS_WORLD, &stWorldMatrix);
	GET_DEVICE()->SetTransform(D3DTS_VIEW, &m_stParameters.m_pCamera->getViewMatrix());
	GET_DEVICE()->SetTransform(D3DTS_PROJECTION, &m_stParameters.m_pCamera->getProjectionMatrix());

	m_stParameters.m_pDirectional->setLightEnable(true);
	GET_DEVICE()->SetMaterial(&m_stMtrl);
}

void CDecorate_SoundObj::postDraw(void)
{
	m_stParameters.m_pDirectional->setLightEnable(false);
	GET_DEVICE()->SetMaterial(NULL);
}

void CDecorate_SoundObj::doDraw(void)
{
	CRenderObject::doDraw();

}

void CDecorate_SoundObj::update(void)
{
	CObject::update();

}

void CDecorate_SoundObj::createSphereMesh()
{
	m_pMesh = nullptr;
	LPD3DXBUFFER	pAdjacency = nullptr;
	D3DXCreateSphere(GET_DEVICE(), 3.0f, 10, 10, &m_pMesh, &pAdjacency);
	D3DXComputeNormals(m_pMesh, (DWORD*)pAdjacency->GetBufferPointer());
}

void CDecorate_SoundObj::createMatrial()
{
	if (m_eSoundType == EPlayingBGM::BIRD)
	{
		m_stMtrl.Diffuse = D3DCOLORVALUE{ 1.0f,0.0f,1.0f,1.0f };
		m_stMtrl.Ambient = D3DCOLORVALUE{ 0.0f,0.0f,0.0f,1.0f };
		m_stMtrl.Specular = D3DCOLORVALUE{ 1.0f,1.0f,1.0f,1.0f };
		m_stMtrl.Power = 25.0f;
	}
	else if (m_eSoundType == EPlayingBGM::CRIKET)
	{
		m_stMtrl.Diffuse = D3DCOLORVALUE{ 0.0f,1.0f,0.0f,1.0f };
		m_stMtrl.Ambient = D3DCOLORVALUE{ 0.0f,0.0f,0.0f,1.0f };
		m_stMtrl.Specular = D3DCOLORVALUE{ 1.0f,1.0f,1.0f,1.0f };
		m_stMtrl.Power = 25.0f;
	}
	else if (m_eSoundType == EPlayingBGM::FIRE)
	{
		m_stMtrl.Diffuse = D3DCOLORVALUE{ 1.0f,0.0f,0.0f,1.0f };
		m_stMtrl.Ambient = D3DCOLORVALUE{ 0.0f,0.0f,0.0f,1.0f };
		m_stMtrl.Specular = D3DCOLORVALUE{ 1.0f,1.0f,1.0f,1.0f };
		m_stMtrl.Power = 25.0f;
	}
	else if (m_eSoundType == EPlayingBGM::OWL)
	{
		m_stMtrl.Diffuse = D3DCOLORVALUE{ 0.5f,0.5f,0.5f,1.0f };
		m_stMtrl.Ambient = D3DCOLORVALUE{ 0.0f,0.0f,0.0f,1.0f };
		m_stMtrl.Specular = D3DCOLORVALUE{ 1.0f,1.0f,1.0f,1.0f };
		m_stMtrl.Power = 25.0f;
	}
	else if (m_eSoundType == EPlayingBGM::RAIN)
	{
		m_stMtrl.Diffuse = D3DCOLORVALUE{ 0.0f,0.0f,1.0f,1.0f };
		m_stMtrl.Ambient = D3DCOLORVALUE{ 0.0f,0.0f,0.0f,1.0f };
		m_stMtrl.Specular = D3DCOLORVALUE{ 1.0f,1.0f,1.0f,1.0f };
		m_stMtrl.Power = 25.0f;
	}
	else if (m_eSoundType == EPlayingBGM::WIND)
	{
		m_stMtrl.Diffuse = D3DCOLORVALUE{ 0.5f,0.7f,1.0f,1.0f };
		m_stMtrl.Ambient = D3DCOLORVALUE{ 0.0f,0.0f,0.0f,1.0f };
		m_stMtrl.Specular = D3DCOLORVALUE{ 1.0f,1.0f,1.0f,1.0f };
		m_stMtrl.Power = 25.0f;
	}
	else 
	{
		assert("CDecorate_SoundObj : Not Collected SoundType!!!");
	}
}
