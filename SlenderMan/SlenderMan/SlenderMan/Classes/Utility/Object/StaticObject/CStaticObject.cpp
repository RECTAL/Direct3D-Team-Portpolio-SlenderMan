#include "CStaticObject.h"
#include "../../../Utility/Manager/CDeviceManager.h"
#include "../../../Function/GlobalFunction.h"
#include "../../Base/CDirect3DApplication.h"
#include "../CameraObject/CCameraObject.h"
#include "../LightObject/CLightObject.h"
#include "../LightObject/SpotLightObject/SpotLightObject.h"
#include "../../Manager/CResourceManager.h"

CStaticObject::CStaticObject(STParameters & a_rstParameters)
	:
	m_stParameters(a_rstParameters)
{
	m_pEffect = GET_EFFECT(a_rstParameters.m_oEffectFilepath);
	m_stStaticMesh = GET_STATIC_MESH(a_rstParameters.m_oMeshFilepath);

	m_stBoundingSphere = this->createBoundingSphere();
	m_pDebugMesh = this->createDebugMesh();
}

void CStaticObject::update(void)
{
	CObject::update();
}

void CStaticObject::doDraw(void)
{
	CRenderObject::doDraw();
	D3DXMATRIXA16	stWorldMatrix = this->getFinalWorldMatrix();
	D3DXMATRIXA16	stViewMatrix = m_stParameters.m_pCamera->getViewMatrix();
	D3DXMATRIXA16	stProjectionMatrix = m_stParameters.m_pCamera->getProjectionMatrix();

	D3DXVECTOR4		stDirectionalLight = D3DXVECTOR4(m_stParameters.m_pDirectional->getForwardDirection(), 0.0f);
	D3DXVECTOR4		stViewPosition = D3DXVECTOR4(m_stParameters.m_pCamera->getPosition(), 1.0f);

	D3DXVECTOR4	stSpotLightPosition[10];
	D3DXVECTOR4	stSpotLightForward[10];
	float	fSpotDistance[10];
	float	fTheta[10];
	float	fPhi[10];

	D3DXVECTOR4 stPointLightPosition[10];
	float	fPointDistance[10];

	m_pEffect->SetMatrix("g_stWorldMatrix", &stWorldMatrix);
	m_pEffect->SetMatrix("g_stViewMatrix", &m_stParameters.m_pCamera->getViewMatrix());
	m_pEffect->SetMatrix("g_stProjectionMatrix", &m_stParameters.m_pCamera->getProjectionMatrix());

	m_pEffect->SetVector("g_stLightDirection", &stDirectionalLight);
	m_pEffect->SetVector("g_stViewPosition", &stViewPosition);

	int nNumSpotLight = m_stParameters.m_nNumSpotLight;
	m_pEffect->SetInt("nNumSpotLight", nNumSpotLight);
	for (int i = 0; i < nNumSpotLight; i++)
	{
		stSpotLightPosition[i] = D3DXVECTOR4(m_stParameters.m_pSpotLight[i].getPosition(), 1.0f);
		stSpotLightForward[i] = D3DXVECTOR4(m_stParameters.m_pSpotLight[i].getForwardDirection(), 0.0f);
		fTheta[i] = m_stParameters.m_pSpotLight[i].m_fTheta;
		fPhi[i] = m_stParameters.m_pSpotLight[i].m_fPhi;
		fSpotDistance[i] = m_stParameters.m_pSpotLight[i].m_fRange;
	}
	for (int i = nNumSpotLight; i < 10; i++)
	{
		stSpotLightPosition[i] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		stSpotLightForward[i] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		fTheta[i] = 0.0f;
		fPhi[i] = 0.0f;
		fSpotDistance[i] = 0.0f;
	}

	m_pEffect->SetVectorArray("g_stSpotLightPosition", stSpotLightPosition, 10);
	m_pEffect->SetVectorArray("g_stSpotLightForward", stSpotLightForward, 10);
	m_pEffect->SetFloatArray("g_fSpotDistance", fSpotDistance, 10);
	m_pEffect->SetFloatArray("g_fTheta", fTheta, 10);
	m_pEffect->SetFloatArray("g_fPhi", fPhi, 10);

	int nNumPointLight = m_stParameters.m_nNumPointLight;
	m_pEffect->SetInt("nNumPointLight", nNumPointLight);
	for (int i = 0; i < nNumPointLight; i++)
	{
		stPointLightPosition[i] = D3DXVECTOR4(m_stParameters.m_pPointLight->getPosition(), 1.0f);
		fPointDistance[i] = 10.0f;
	}
	
	for (int i = nNumPointLight; i < 10; i++)
	{
		stPointLightPosition[i] = D3DXVECTOR4(0.0f,0.0f,0.0f, 1.0f);
		fPointDistance[i] = 0.0f;
	}

	m_pEffect->SetVectorArray("g_stPointLightPosition", stPointLightPosition, 10);
	m_pEffect->SetFloatArray("g_fPointDistance", fPointDistance, 10);

	for (int i = 0; i < m_stStaticMesh.m_nNumMaterials; ++i) {
		auto pTexture = m_stStaticMesh.m_oTextureList[i];
		m_pEffect->SetTexture("g_pTexture", pTexture);

		RunEffectLoop(m_pEffect, m_stTechniqueName.c_str(), [=](int a_nPassNumber) -> void {
			m_stStaticMesh.m_pStaticMesh->DrawSubset(i);
		});
	}


	GET_DEVICE()->SetTransform(D3DTS_WORLD, &stWorldMatrix);
	GET_DEVICE()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pDebugMesh->DrawSubset(0);

	GET_DEVICE()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}


STBoundingSphere CStaticObject::createBoundingSphere()
{
	STBoundingSphere stBoundingSphere;
	ZeroMemory(&stBoundingSphere, sizeof(stBoundingSphere));

	void* pVertices = nullptr;
	if (SUCCEEDED(m_stStaticMesh.m_pStaticMesh->LockVertexBuffer(0, &pVertices)))
	{
		D3DXComputeBoundingSphere((D3DXVECTOR3*)pVertices, m_stStaticMesh.m_pStaticMesh->GetNumVertices(),
			m_stStaticMesh.m_pStaticMesh->GetNumBytesPerVertex(), &stBoundingSphere.m_stPosition, &stBoundingSphere.m_fRadius
		);

		m_stStaticMesh.m_pStaticMesh->UnlockVertexBuffer();
	}



	return stBoundingSphere;
}

LPD3DXMESH CStaticObject::createDebugMesh()
{
	LPD3DXMESH pMesh = nullptr;
	D3DXCreateSphere(GET_DEVICE(), m_stBoundingSphere.m_fRadius, 10, 10, &pMesh, NULL);
	return pMesh;
}
