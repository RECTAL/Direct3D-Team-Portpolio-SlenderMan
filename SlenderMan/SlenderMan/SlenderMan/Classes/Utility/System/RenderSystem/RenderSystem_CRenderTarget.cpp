#include "RenderSystem_CRenderTarget.h"
#include "../../Manager/CDeviceManager.h"
#include "../../../Utility/Manager/CResourceManager.h"

CRenderTarget::CRenderTarget()
{
}

CRenderTarget::CRenderTarget(int a_nWidth, int a_nHeight, D3DVIEWPORT9* a_stViewport)
{
	m_stRenderTarget.m_pTex = this->createNewTexture(a_nWidth, a_nHeight);
	m_stRenderTarget.m_pTexSurf = this->createTexSurface(m_stRenderTarget.m_pTex);
	m_stRenderTarget.m_pDepthStencil = this->createNewDepthStencil(a_nWidth, a_nHeight);

	if (a_stViewport != nullptr)
		CopyMemory(&m_stRenderTarget.m_stViewPort, a_stViewport, sizeof(D3DVIEWPORT9));

	m_stRenderTarget.m_nWidth = a_nWidth;
	m_stRenderTarget.m_nHeight = a_nHeight;

	m_pPlaneMesh = this->createPlaneMesh();
	this->createEffect();
}

CRenderTarget::~CRenderTarget()
{
	SAFE_RELEASE(m_pPlaneMesh);
}

LPDIRECT3DTEXTURE9 CRenderTarget::createNewTexture(int width, int height)
{
	LPDIRECT3DTEXTURE9 pTex = nullptr;
	GET_DEVICE()->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTex, NULL);
	return pTex;
}

LPDIRECT3DSURFACE9 CRenderTarget::createTexSurface(LPDIRECT3DTEXTURE9 a_pTex)
{
	LPDIRECT3DSURFACE9 pTexSurf = nullptr;
	a_pTex->GetSurfaceLevel(0, &pTexSurf);
	return pTexSurf;
}

LPDIRECT3DSURFACE9 CRenderTarget::createNewDepthStencil(int width, int height)
{
	LPDIRECT3DSURFACE9 pDepthStencil = nullptr;
	GET_DEVICE()->CreateDepthStencilSurface(
		width, height,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&pDepthStencil,
		NULL
	);
	return pDepthStencil;
}

LPD3DXMESH CRenderTarget::createPlaneMesh()
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

void CRenderTarget::createEffect()
{
	m_pCopyEffect = GET_EFFECT("Resources/Effects/CopyTexture.fx");
	m_pOutlineEffect = GET_EFFECT("Resources/Effects/Outline.fx");
	m_pOutlineBlurEffect = GET_EFFECT("Resources/Effects/OutlineBlur.fx");
	m_pBlendEffect = GET_EFFECT("Resources/Effects/BlendTexture.fx");
	m_pLerpEffect = GET_EFFECT("Resources/Effects/LerpTexture.fx");
	m_pNoiseEffect = GET_EFFECT("Resources/Effects/NoiseTexture.fx");
}

void CRenderTarget::preDraw()
{
	this->preRenderState_SamplerState();
	this->preRenderState_StencilState();
	this->preRenderState_ZEnable();
	this->preRenderState_CullMode();
}

void CRenderTarget::postDraw()
{
	this->postRenderState_SamplerState();
	this->postRenderState_StencilState();
	this->postRenderState_ZEnable();
	this->postRenderState_CullMode();
}
