#include "CTerrainObject.h"
#include "../../../Function/GlobalFunction.h"
#include "../LightObject/CLightObject.h"
#include "../LightObject/SpotLightObject/SpotLightObject.h"
#include "../../Manager/CDeviceManager.h"
#include "../../Manager/CResourceManager.h"
#include "../../Manager/CInputManager.h"
#include "../../Manager/CWindowManager.h"
#include "../../System/CollisionSystem/CollisionSystem_CQuadSystem.h"

CTerrainObject::CTerrainObject(const STParameters& a_stParameters)
{

	m_stParameters	= a_stParameters;
	
	m_cxDIB			= a_stParameters.m_stMapSize.cx;
	m_czDIB			= a_stParameters.m_stMapSize.cy;
	m_cyDIB			= 0;
	m_vfScale		= a_stParameters.m_vfScale;
	m_pCamera		= a_stParameters.m_pCamera;
	m_pLightObj		= a_stParameters.m_pLight;

	m_cxTerrain = m_cxDIB * m_vfScale.x;
	m_czTerrain = m_czDIB * m_vfScale.z;
	m_cyTerrain = 0;

	m_nTriangles	= 0;

	m_pTerrainMesh	= NULL;
	m_pQuadTree		= NULL;

	m_pHeightMap	= new D3DXVECTOR3[m_cxDIB*m_czDIB];
	m_pSplatTexture = GET_TEXTURE(a_stParameters.m_oSplatFilepath.c_str(), m_cxDIB-1, m_czDIB-1);
	m_pEffect		= GET_EFFECT(a_stParameters.m_oEffectFilepath.c_str());

	for (int i = 0; i < CTerrainObject::MAX_TERRAIN_TEX; i++) m_pTex[i] = NULL;

	this->create();
}

CTerrainObject::~CTerrainObject()
{
	delete[] m_stParameters.m_pSpotLight;
	delete[] m_stParameters.m_pPointLight;
	destroy();
}

HRESULT CTerrainObject::create()
{
	if (FAILED(buildHeightMap(m_stParameters.m_oHeightFilepath))) { destroy(); return E_FAIL; }
	if (FAILED(loadTextures(m_stParameters.m_oTextureFilepathList))) {destroy(); return E_FAIL;}
	if (FAILED(createTerrainMesh())) {destroy(); return E_FAIL;}
	m_pQuadTree = new CQuadTree(m_cxDIB, m_czDIB);
	if (FAILED(buildQuadTree(m_pHeightMap))) { destroy(); return E_FAIL; }

	return S_OK;
}


HRESULT CTerrainObject::destroy()
{
	SAFE_DELETE(m_pQuadTree);
	SAFE_DELETE_ARRAY(m_pHeightMap);
	SAFE_RELEASE(m_pTerrainMesh);


	return S_OK;
}

HRESULT CTerrainObject::loadTextures(std::vector<std::string>&  lpTexFilename)
{
	for (int i = 0; i < CTerrainObject::MAX_TERRAIN_TEX; i++)
	{
		m_pTex[i] = GET_TEXTURE(lpTexFilename[i],m_cxDIB,m_czDIB);
		if (m_pTex == nullptr)
		{
			assert("CTerrainObject: texture loading false");
		}
	}
	return S_OK;
}

HRESULT CTerrainObject::buildHeightMap(std::string lpFilename)
{

	if (lpFilename.rfind(".raw") != std::string::npos)
	{
		auto pfReadStream = fopen(lpFilename.c_str(), "rb");
		auto oReadStreamPtr = std::shared_ptr<FILE>(pfReadStream, [=](FILE* a_pfFileStream)->void {fclose(a_pfFileStream); });

		for (int i = 0; i < m_czDIB; i++)
		{
			for (int j = 0; j < m_cxDIB; j++)
			{
				int nIndex = (i*m_cxDIB) + j;
				USHORT nHeight;
				fread(&nHeight, sizeof(nHeight), 1, oReadStreamPtr.get());
				m_oHeightList.push_back(nHeight*m_vfScale.y);
				m_pHeightMap[nIndex].y = nHeight * m_vfScale.y;
			}
		}

		int a = 1;
	}
	else
	{

		auto pHeightTexture = GET_TEXTURE(lpFilename.c_str(), m_cxDIB, m_czDIB);
		D3DLOCKED_RECT stLockRect;
		ZeroMemory(&stLockRect, sizeof(stLockRect));

		if (SUCCEEDED(pHeightTexture->LockRect(0, &stLockRect, NULL, 0)))
		{
			D3DCOLOR *pnBytes = (D3DCOLOR*)stLockRect.pBits;
			for (int i = 0; i < m_czDIB; i++)
			{
				for (int j = 0; j < m_cxDIB; j++)
				{
					int nIndex = (i*m_cxDIB) + j;
					D3DCOLOR nColor = pnBytes[nIndex];
					int nRedColor = (nColor >> 16) & 0xff;
					int nGreenColor = (nColor >> 8) & 0xff;
					int nBlueColor = (nColor >> 0) & 0xff;

					int nHeight = nRedColor + nGreenColor + nBlueColor;
					m_pHeightMap[nIndex].y = nHeight * m_vfScale.y;

				}
			}
			pHeightTexture->UnlockRect(0);
		}

	}

	return S_OK;
}

HRESULT CTerrainObject::buildQuadTree(D3DXVECTOR3* pHeightMap)
{
	m_pQuadTree->build(pHeightMap);
	return S_OK;
}

HRESULT CTerrainObject::createTerrainMesh()
{
	D3DVERTEXELEMENT9 astElements[] = {
		0, 0,	D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,	0,
		0, 12,	D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,	0,
		0, 20,	D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,	0,
		0, 32,	D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BINORMAL,	0,
		0, 44,	D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TANGENT,	0,
		D3DDECL_END()
	};

	D3DXCreateMesh(
		(m_cxDIB*m_czDIB) * 2,
		(m_cxDIB*m_czDIB),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		astElements,
		GET_DEVICE(),
		&m_pTerrainMesh
	);

	GET_DEVICE()->CreateVertexDeclaration(astElements, &m_pVertexDeclaration);

	TERRAINVERTEX* pstVertices = nullptr;
	if (SUCCEEDED(m_pTerrainMesh->LockVertexBuffer(0, (void**)&pstVertices)))
	{
		D3DXVECTOR3 stBasePosition = 
		{
			(m_cxTerrain/-2.0f) + (m_vfScale.x/2.0f),
			0.0f,
			(m_czTerrain/2.0f) - (m_vfScale.z/2.0f)
		};

		for (int i = 0; i < m_czDIB; i++)
		{
			for (int j = 0; j < m_cxDIB; j++)
			{
				int nIndex = (i*m_cxDIB) + j;
				pstVertices[nIndex].m_stPosition = stBasePosition + D3DXVECTOR3(j*m_vfScale.x, m_pHeightMap[nIndex].y, i*-m_vfScale.z);
				m_pHeightMap[nIndex] = pstVertices[nIndex].m_stPosition;
				pstVertices[nIndex].m_stUV = D3DXVECTOR2(j * 80  / (float)(m_cxDIB - 1), i * 80 / (float)(m_czDIB - 1));
			}
		}
		m_pTerrainMesh->UnlockVertexBuffer();
	}

	DWORD*	pIndices = nullptr;
	if (SUCCEEDED(m_pTerrainMesh->LockIndexBuffer(0, (void**)&pIndices)))
	{
		for (int i = 0; i < m_czDIB - 1; i++)
		{
			for (int j = 0; j < m_cxDIB - 1; j++)
			{
				int nVertexIndex = (i*m_cxDIB) + j;
				int nIndex = nVertexIndex * 6;

				pIndices[nIndex + 0] = nVertexIndex;
				pIndices[nIndex + 1] = nVertexIndex + 1;
				pIndices[nIndex + 2] = nVertexIndex + m_cxDIB;

				pIndices[nIndex + 3] = nVertexIndex + m_cxDIB + 1;
				pIndices[nIndex + 4] = nVertexIndex + m_cxDIB;
				pIndices[nIndex + 5] = nVertexIndex + 1;

			}
		}
		m_pTerrainMesh->UnlockIndexBuffer();
	}

	D3DXComputeNormals(m_pTerrainMesh, NULL);
	D3DXComputeTangent(m_pTerrainMesh, 0, 0, 0, TRUE, NULL);

	return S_OK;
}

void CTerrainObject::update(void)
{
	CObject::update();
	m_pCamera = m_stParameters.m_pCamera;
	m_pLightObj = m_stParameters.m_pLight;



	LPDWORD		pIndices = nullptr;

	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
	m_pTerrainMesh->GetIndexBuffer(&pIndexBuffer);


	
	if (SUCCEEDED(pIndexBuffer->Lock(0, (m_cxDIB - 1)*(m_czDIB - 1) * 2 * sizeof(DWORD) * 3, (void**)&pIndices, 0)))
	{
		m_nTriangles = m_pQuadTree->GenerateIndex(pIndices, m_pHeightMap, m_pCamera->getCameraFrustum());

		pIndexBuffer->Unlock();
	}
	
	
}

bool CTerrainObject::terrainPicking(D3DXVECTOR3& a_stPosition)
{
	STRay	ray;
	float	fu, fv;
	TERRAINVERTEX* pVertices = nullptr;
	LPDWORD			pIndices = nullptr;
	bool isPicking = false;
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
	m_pTerrainMesh->GetIndexBuffer(&pIndexBuffer);
	
	ray = CreateRay(GET_MOUSE_POSITION());
	
	
	if (SUCCEEDED(m_pTerrainMesh->LockVertexBuffer(0, (void**)&pVertices)))
	{
		if (SUCCEEDED(pIndexBuffer->Lock(0, (m_cxDIB - 1)*(m_czDIB - 1) * 2 * sizeof(DWORD) * 3, (void**)&pIndices, 0)))
		{
			for (int i = 0; i < m_nTriangles * 3; i += 3)
			{
				if (D3DXIntersectTri(&pVertices[pIndices[i]].m_stPosition, &pVertices[pIndices[i + 1]].m_stPosition, &pVertices[pIndices[i + 2]].m_stPosition,
					&ray.m_stOrigin, &ray.m_stDirection, &fu, &fv, NULL))
				{

					D3DXVECTOR3 du;
					D3DXVec3Normalize(&du,&(pVertices[pIndices[i + 1]].m_stPosition - pVertices[pIndices[i]].m_stPosition));
					du *= fu;

					D3DXVECTOR3 dv;
					D3DXVec3Normalize(&dv, &(pVertices[pIndices[i + 2]].m_stPosition - pVertices[pIndices[i]].m_stPosition));
					dv *= fv;

					a_stPosition = pVertices[pIndices[i]].m_stPosition + du + dv;
					isPicking = true;
				}
			}
			pIndexBuffer->Unlock();
		}
	}
	m_pTerrainMesh->UnlockVertexBuffer();
	return isPicking;
}

int CTerrainObject::findIndex(D3DXVECTOR3 a_stPosition)
{
	D3DXVECTOR3 stPosition = D3DXVECTOR3((a_stPosition.x + m_cxTerrain / 2) / m_stScale.x, 0, (a_stPosition.y + m_cxTerrain / 2) / m_stScale.y);
	int xIndex = (int)stPosition.x;
	int zIndex = (int)stPosition.z;


	return (zIndex*m_cxDIB + xIndex);
}

void CTerrainObject::doDraw()
{
	render();
}


HRESULT CTerrainObject::render()
{
	CRenderObject::doDraw();

	D3DXMATRIXA16 stWorldMatrix = this->getFinalWorldMatrix();
	D3DXMATRIXA16 stViewMatrix = m_pCamera->getViewMatrix();
	D3DXMATRIXA16 stProjectionMatrix = m_pCamera->getProjectionMatrix();
	D3DXVECTOR4	  stLightDirection = D3DXVECTOR4(m_pLightObj->getForwardDirection(), 0.0f);
	D3DXVECTOR4	  stViewPosition = D3DXVECTOR4(m_pCamera->getPosition(), 1.0f);

	D3DXVECTOR4	stSpotLightPosition[10];
	D3DXVECTOR4	stSpotLightForward[10];
	float	fSpotDistance[10];
	float	fTheta[10];
	float	fPhi[10];

	D3DXVECTOR4 stPointLightPosition[10];
	float	fPointDistance[10];


	m_pEffect->SetMatrix("g_stWorldMatrix", &stWorldMatrix);
	m_pEffect->SetMatrix("g_stViewMatrix", &stViewMatrix);
	m_pEffect->SetMatrix("g_stProjectionMatrix", &stProjectionMatrix);

	m_pEffect->SetVector("g_stLightDirection", &stLightDirection);
	m_pEffect->SetVector("g_stViewPosition", &stViewPosition);
	// }
	// 텍스처를 설정한다
	// {
	m_pEffect->SetTexture("g_pSplatTexture", m_pSplatTexture);

	for (int i = 0; i < 4; ++i) {
		char szVariableName[MAX_PATH] = "";
		sprintf(szVariableName, "g_pTexture%c", 'A' + i);

		m_pEffect->SetTexture(szVariableName, m_pTex[i]);
	}


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

	// }
	LPDIRECT3DVERTEXBUFFER9	pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
	
	m_pTerrainMesh->GetVertexBuffer(&pVertexBuffer);
	m_pTerrainMesh->GetIndexBuffer(&pIndexBuffer);
	
	GET_DEVICE()->SetStreamSource(0, pVertexBuffer, 0, sizeof(TERRAINVERTEX));
	GET_DEVICE()->SetVertexDeclaration(m_pVertexDeclaration);
	GET_DEVICE()->SetIndices(pIndexBuffer);

	RunEffectLoop(m_pEffect, m_stTechniqueName.c_str(), [=](int a_nPassNumber) -> void {
		GET_DEVICE()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_cxDIB*m_czDIB, 0, m_nTriangles);
	});

	return S_OK;
}