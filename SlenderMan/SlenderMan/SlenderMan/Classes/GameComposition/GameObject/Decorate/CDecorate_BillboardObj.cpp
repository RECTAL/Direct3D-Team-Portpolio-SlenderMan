#include "CDecorate_BillboardObj.h"
#include "../../../Utility/Manager/CDeviceManager.h"
#include "../../../Utility/Object/CameraObject/CCameraObject.h"
#include "../../../Utility/Object/LightObject/CLightObject.h"
#include "../../../Utility/Object/LightObject/SpotLightObject/SpotLightObject.h"
#include "../../../Utility/Manager/CResourceManager.h"
#include "../../../Utility/Manager/CTimeManager.h"
#include "../../../Function/GlobalFunction.h"


CDecorate_BillboardObj::CDecorate_BillboardObj(STParameters & a_rstParameters)
	:
	m_stParameters(a_rstParameters)
{
	m_nAnimationCount = a_rstParameters.m_nFrame;
	m_pEffect = GET_EFFECT(a_rstParameters.m_oEffectFilepath);
	m_pMesh = this->createPlaneMeshWithX();
	for (int i = 0; i < m_nAnimationCount; i++)
	{
		char path[MAX_PATH];
		sprintf(path, "%s_%d.%s", a_rstParameters.m_oTextureFilepath.c_str(), i, a_rstParameters.m_stExtantion.c_str());
		m_oSpriteTexture.push_back(GET_TEXTURE(path,GOUST_VALUE, GOUST_VALUE));
	}
	
	auto stBoundingBox = CreateBoundingBox(m_pMesh);
	auto stBoundingSphere = CreateBoundingSphere(m_pMesh);

	this->setBoundingBox(stBoundingBox);
	this->setBoundingSphere(stBoundingSphere);
}

CDecorate_BillboardObj::~CDecorate_BillboardObj()
{
	SAFE_RELEASE(m_pMesh);

	delete[] m_stParameters.m_pSpotLight;
	delete[] m_stParameters.m_pPointLight;
}

void CDecorate_BillboardObj::preDraw(void)
{
	CRenderObject::preDraw();




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


	m_pEffect->SetFloat("g_fFogDensity", 0.001f);
	m_pEffect->SetFloat("g_fFogEnd", 1000.0f);


	int nNumSpotLight = m_stParameters.m_nNumSpotLight;
	m_pEffect->SetInt("nNumSpotLight", nNumSpotLight);
	for (int i = 0; i < nNumSpotLight; i++)
	{
		stSpotLightPosition[i] = D3DXVECTOR4(m_stParameters.m_pSpotLight[i]->getPosition(), 1.0f);
		stSpotLightForward[i] = D3DXVECTOR4(m_stParameters.m_pSpotLight[i]->getForwardDirection(), 0.0f);
		fTheta[i] = m_stParameters.m_pSpotLight[i]->m_fTheta;
		fPhi[i] = m_stParameters.m_pSpotLight[i]->m_fPhi;
		fSpotDistance[i] = m_stParameters.m_pSpotLight[i]->m_fRange;
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
		stPointLightPosition[i] = D3DXVECTOR4(m_stParameters.m_pPointLight[i]->getPosition(), 1.0f);
		fPointDistance[i] = 10.0f;
	}

	for (int i = nNumPointLight; i < 10; i++)
	{
		stPointLightPosition[i] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		fPointDistance[i] = 0.0f;
	}

	m_pEffect->SetVectorArray("g_stPointLightPosition", stPointLightPosition, 10);
	m_pEffect->SetFloatArray("g_fPointDistance", fPointDistance, 10);

	m_pEffect->SetTexture("g_pTexture", m_oSpriteTexture[m_nTextureOffset]);

	GET_DEVICE()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CDecorate_BillboardObj::postDraw(void)
{
	CRenderObject::postDraw();
	GET_DEVICE()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CDecorate_BillboardObj::doDraw(void)
{
	CRenderObject::doDraw();

	RunEffectLoop(m_pEffect, m_stTechniqueName.c_str(), [=](int nPassNum)->void {
		m_pMesh->DrawSubset(0);
	});

}

void CDecorate_BillboardObj::update(void)
{
	CRenderObject::update();
	m_bIsCollision = false;
	m_fAnimationTimer += GET_DELTA_TIME();
	if (m_fAnimationTimer > 0.5f)
	{
		m_fAnimationTimer = 0.0f;
		m_nTextureOffset++;
		if (m_nTextureOffset >= m_oSpriteTexture.size())m_nTextureOffset = 0;
	}
}

void CDecorate_BillboardObj::createPlaneMesh()
{
	/*D3DVERTEXELEMENT9 astElements[] = {
		0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0,
		0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0,
		0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0,
		0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0,
		0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0,
		D3DDECL_END()
	};
	D3DXCreateMesh(
		2, 6, D3DXMESH_MANAGED, astElements, GET_DEVICE(), &m_pMesh
	);

	STVertex* pVertices = nullptr;
	if (SUCCEEDED(m_pMesh->LockVertexBuffer(0, (void**)&pVertices)))
	{
		pVertices[0].m_stPosition = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
		pVertices[0].m_stUV		  = D3DXVECTOR2( 0.0f, 0.0f);

		pVertices[1].m_stPosition = D3DXVECTOR3( 1.0f, 1.0f, 0.0f);
		pVertices[1].m_stUV		  = D3DXVECTOR2(1.0f, 0.0f);

		pVertices[2].m_stPosition = D3DXVECTOR3(-1.0f,-1.0f, 0.0f);
		pVertices[2].m_stUV		  = D3DXVECTOR2(0.0f, 1.0f);

		pVertices[3].m_stPosition = D3DXVECTOR3( 1.0f,-1.0f, 0.0f);
		pVertices[3].m_stUV		  = D3DXVECTOR2(1.0f, 1.0f);

		pVertices[4].m_stPosition = D3DXVECTOR3(-1.0f,-1.0f, 0.0f);
		pVertices[4].m_stUV		  = D3DXVECTOR2(0.0f, 1.0f);

		pVertices[5].m_stPosition = D3DXVECTOR3( 1.0f, 1.0f, 0.0f);
		pVertices[5].m_stUV		  = D3DXVECTOR2(1.0f, 0.0f);


		m_pMesh->UnlockVertexBuffer();
	}
	D3DXComputeNormals(m_pMesh, NULL);
	D3DXComputeTangent(m_pMesh,0,0,0,TRUE,NULL);*/

}

LPD3DXMESH CDecorate_BillboardObj::createPlaneMeshWithX()
{
	LPD3DXMESH pMesh = nullptr;
	LPD3DXBUFFER pAdjacency = nullptr;
	D3DXLoadMeshFromX(_T("Resources/Meshes/Plane/plane.x"), D3DXMESH_MANAGED, GET_DEVICE(), &pAdjacency, nullptr, nullptr, nullptr, &pMesh);

	D3DVERTEXELEMENT9 astElements[] = {
		0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0,
		0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0,
		0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,	 0,
		0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0,
		0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0,
		D3DDECL_END()
	};

	auto OriginMesh = pMesh;
	OriginMesh->CloneMesh(OriginMesh->GetOptions(), astElements, GET_DEVICE(), &pMesh);

	D3DXComputeNormals(pMesh, (DWORD*)pAdjacency->GetBufferPointer());
	D3DXComputeTangent(pMesh, 0, 0, 0, TRUE, (DWORD*)pAdjacency->GetBufferPointer());

	SAFE_RELEASE(pAdjacency);
	SAFE_RELEASE(OriginMesh);

	return pMesh;
}
