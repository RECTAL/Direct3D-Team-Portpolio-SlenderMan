#include "SpotLightObject.h"
#include "../../CameraObject/CCameraObject.h"
#include "../../../../Function/GlobalFunction.h"
#include "../../../Manager/CDeviceManager.h"
#include "../../../Manager/CResourceManager.h"
#include "../../../Manager/CTimeManager.h"
#include "../../../Manager/CRendertargetManager.h"

CSpotLightObject::CSpotLightObject(int a_nIndex, float a_fRange, float a_fTheta, float a_fPhi)
	:CLightObject(a_nIndex)
{
	m_stLight.Type = D3DLIGHT_SPOT;
	m_stLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_stLight.Attenuation0 = 0.0000001f;
	m_stLight.Attenuation1 = 0.0000001f;
	m_stLight.Attenuation2 = 0.0000001f;
	m_stLight.Falloff = 1.0f;
	m_stLight.Range = a_fRange;
	m_stLight.Theta = a_fTheta;
	m_stLight.Phi = a_fPhi;


	m_fRange	= a_fRange;
	m_fTheta	= a_fTheta;
	m_fPhi		= a_fPhi;

	m_stSpotMesh = GET_STATIC_MESH("Resources/Meshes/cone/cone.X");
	m_pEffect = this->createSpotEffect();
}

CSpotLightObject::~CSpotLightObject()
{
}

void CSpotLightObject::update(void)
{
	CObject::update();
	m_stLight.Direction = this->getForwardDirection();


	D3DXMatrixIdentity(&m_stViewMatrix);

	m_stViewMatrix(0, 0) = m_stRightDirection.x;
	m_stViewMatrix(1, 0) = m_stRightDirection.y;
	m_stViewMatrix(2, 0) = m_stRightDirection.z;
	m_stViewMatrix(3, 0) = -D3DXVec3Dot(&m_stPosition, &m_stRightDirection);

	m_stViewMatrix(0, 1) = m_stUpDirection.x;
	m_stViewMatrix(1, 1) = m_stUpDirection.y;
	m_stViewMatrix(2, 1) = m_stUpDirection.z;
	m_stViewMatrix(3, 1) = -D3DXVec3Dot(&m_stPosition, &m_stUpDirection);

	m_stViewMatrix(0, 2) = m_stForwardDirection.x;
	m_stViewMatrix(1, 2) = m_stForwardDirection.y;
	m_stViewMatrix(2, 2) = m_stForwardDirection.z;
	m_stViewMatrix(3, 2) = -D3DXVec3Dot(&m_stPosition, &m_stForwardDirection);
}

void CSpotLightObject::draw(void)
{
	//()->SetRenderTarget(0, FIND_RENDERTARGET("StageRenderTarget")->m_stRenderTarget.m_pTexSurf);
	//GET_DEVICE()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0, 100, 100, 100), 1.0f, 0.0f);
	D3DXMATRIXA16	stSpotRotateMatrix;
	D3DXMATRIXA16	stSpotWorldMatrix;
	D3DXMATRIXA16	stSpotScaleMatrix;
	D3DXMATRIXA16	stSpotTransMatrix;
	D3DXMatrixTranslation(&stSpotTransMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&stSpotRotateMatrix, D3DXToRadian(0.0f), D3DXToRadian(0.0f), D3DXToRadian(0.0f));
	D3DXMatrixScaling(&stSpotScaleMatrix, 1.0f, 1.0f, 1.0f);
	stSpotWorldMatrix = stSpotScaleMatrix * stSpotRotateMatrix*stSpotTransMatrix;
	D3DXMATRIXA16	stSpotFinalMatrix	= stSpotWorldMatrix * this->getWorldMatrix();

	D3DXMATRIXA16	stViewMatrix		= m_pCameraObj->getViewMatrix();
	D3DXMATRIXA16	stProjectionMatrix	= m_pCameraObj->getProjectionMatrix();
	
	D3DXVECTOR4		stViewPosition = D3DXVECTOR4(m_pCameraObj->getPosition(),1.0f);
	
	float fFresnel		= 1.0f;
	float fAlphaOffset	= 0.5f;
	float fNoiseSpeed	= 0.2f;
	float fAmbient		= 0.2f;
	float fIntensity	= 0.3f;
	float fFade			= 1.0f;
	float fWind			= 0.3f;
	float fTime			= GET_RUNNING_TIME();



	m_pEffect->SetMatrix("g_stWorldMatrix", &stSpotFinalMatrix);
	m_pEffect->SetMatrix("g_stViewMatrix", &stViewMatrix);
	m_pEffect->SetMatrix("g_stProjectionMatrix", &stProjectionMatrix);

	m_pEffect->SetVector("g_stViewPosition", &stViewPosition);

	m_pEffect->SetFloat("g_fFresnel", fFresnel);
	m_pEffect->SetFloat("g_fAlphaOffset", fAlphaOffset);
	m_pEffect->SetFloat("g_fNoiseSpeed", fNoiseSpeed);
	m_pEffect->SetFloat("g_fAmbient", fAmbient);
	m_pEffect->SetFloat("g_fIntensity", fIntensity);
	m_pEffect->SetFloat("g_fFade", fFade);
	m_pEffect->SetFloat("g_fWind", fWind);
	m_pEffect->SetFloat("g_fTime", fTime);



	GET_DEVICE()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	GET_DEVICE()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GET_DEVICE()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_DEVICE()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GET_DEVICE()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	for (int i = 0; i < m_stSpotMesh.m_nNumMaterials; ++i) {
		RunEffectLoop(m_pEffect, "spotLightRay", [=](int a_nPassNumber) -> void {
			m_stSpotMesh.m_pStaticMesh->DrawSubset(i);
		});
	}


	GET_DEVICE()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	GET_DEVICE()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	GET_DEVICE()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
}

LPD3DXMESH CSpotLightObject::createSpotMesh()
{
	LPD3DXMESH pMesh = nullptr;
	LPD3DXBUFFER	pAdjacency = nullptr;
	D3DVERTEXELEMENT9 astElements[] = {
		0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0,
		0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0,
		0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0,
		0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0,
		0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0,
		D3DDECL_END()
	};
	D3DXLoadMeshFromXA("Resources/Meshes/cone/cone.X", D3DXMESH_MANAGED, GET_DEVICE(), &pAdjacency, NULL, NULL, NULL, &pMesh);

	LPD3DXMESH pCloneMesh = nullptr;
	pMesh->CloneMesh(pMesh->GetOptions(), astElements, GET_DEVICE(), &pCloneMesh);
	
	pCloneMesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE,
		(DWORD *)pAdjacency->GetBufferPointer(),
		(DWORD *)pAdjacency->GetBufferPointer(),
		NULL,
		NULL);

	D3DXComputeNormals(pCloneMesh, (DWORD*)pAdjacency->GetBufferPointer());
	D3DXComputeTangent(pCloneMesh, 0, 0, 0, TRUE, (DWORD*)pAdjacency->GetBufferPointer());


	return pCloneMesh;
}

LPD3DXEFFECT CSpotLightObject::createSpotEffect()
{
	LPD3DXEFFECT pEffect = nullptr;

	pEffect = GET_EFFECT("Resources/Effects/spotLightRay.fx");

	return pEffect;
}
