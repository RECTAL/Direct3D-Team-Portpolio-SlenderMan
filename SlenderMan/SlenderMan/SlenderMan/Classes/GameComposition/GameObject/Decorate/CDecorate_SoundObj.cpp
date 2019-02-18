#include "CDecorate_SoundObj.h"
#include "../../../Utility/Debug/CDebugDraw.h"
#include "../../../Utility/Manager/CResourceManager.h"
#include "../../../Utility/Object/CameraObject/CCameraObject.h"
#include "../../../Utility/Object/LightObject/CLightObject.h"
#include "../../../Function/GlobalFunction.h"
#include "../../../Utility/Manager/CDeviceManager.h"
CDecorate_SoundObj::CDecorate_SoundObj(STParameters & a_rstParameters, EPlayingBGM a_eSound)
	:m_stParameters(a_rstParameters), m_eSoundType(a_eSound)
{
	this->createSphereMesh();
	this->createMatrial();

	auto stBoundingBox = CreateBoundingBox(m_pMesh);
	auto stBoundingSphere = CreateBoundingSphere(m_pMesh);

	this->setBoundingBox(stBoundingBox);
	this->setBoundingSphere(stBoundingSphere);

	m_pDebugDraw = new CDebugDraw(this, EDebugDrawType::SPHERE, true);
	m_pEffect = GET_EFFECT("Resources/Effects/DefaultSoundObj.fx");
}

CDecorate_SoundObj::~CDecorate_SoundObj()
{
	SAFE_DELETE(m_pDebugDraw);

}

void CDecorate_SoundObj::preDraw(void)
{
	D3DXMATRIXA16 stWorldMatrix;
	D3DXMatrixTranslation(&stWorldMatrix, this->getPosition().x, this->getPosition().y, this->getPosition().z);
	
	m_pEffect->SetMatrix("g_stWorldMatrix", &stWorldMatrix);
	m_pEffect->SetMatrix("g_stViewMatrix", &m_stParameters.m_pCamera->getViewMatrix());
	m_pEffect->SetMatrix("g_stProjectionMatrix", &m_stParameters.m_pCamera->getProjectionMatrix());
	
	m_pEffect->SetVector("g_stLightDirection", &D3DXVECTOR4(m_stParameters.m_pDirectional->getForwardDirection(), 1.0f));
	m_pEffect->SetVector("g_stViewPosition", &D3DXVECTOR4(m_stParameters.m_pCamera->getPosition(), 1.0f));

	m_pEffect->SetVector("g_stColor", &m_stColor);

}

void CDecorate_SoundObj::postDraw(void)
{
	m_pDebugDraw->draw();
}

void CDecorate_SoundObj::doDraw(void)
{
	CRenderObject::doDraw();
	RunEffectLoop(m_pEffect, "DefaultColor", [=](int nPassNum)->void {
		m_pMesh->DrawSubset(0);
	});
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
	if (m_eSoundType == EPlayingBGM::CROW)
	{
		m_stColor = D3DXVECTOR4{ 1.0f,0.0f,1.0f,1.0f };
	}
	else if (m_eSoundType == EPlayingBGM::CRIKET)
	{
		m_stColor = D3DXVECTOR4{ 0.0f,1.0f,0.0f,1.0f };
	}
	else if (m_eSoundType == EPlayingBGM::FIRE)
	{
		m_stColor = D3DXVECTOR4{ 1.0f,0.0f,0.0f,1.0f };
	}
	else if (m_eSoundType == EPlayingBGM::OWL)
	{
		m_stColor = D3DXVECTOR4{ 0.5f,0.5f,0.5f,1.0f };
	}
	else if (m_eSoundType == EPlayingBGM::RAIN)
	{
		m_stColor = D3DXVECTOR4{ 0.0f,0.0f,1.0f,1.0f };
	}
	else if (m_eSoundType == EPlayingBGM::WIND)
	{
		m_stColor = D3DXVECTOR4{ 0.5f,0.7f,1.0f,1.0f };
	}
	else 
	{
		assert("CDecorate_SoundObj : Not Collected SoundType!!!");
	}
}
