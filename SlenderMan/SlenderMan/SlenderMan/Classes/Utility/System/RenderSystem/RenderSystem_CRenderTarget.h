#pragma once
#include "../../../Define/KGlobalDefine.h"

class CRenderTarget
{
public:
	typedef struct STRenderTarget
	{
		LPDIRECT3DTEXTURE9	m_pTex = nullptr;


		LPDIRECT3DSURFACE9	m_pTexSurf = nullptr;
		LPDIRECT3DSURFACE9	m_pDepthStencil = nullptr;
		D3DVIEWPORT9		m_stViewPort;

		int					m_nWidth;
		int					m_nHeight;
	}RT;

public:
	CRenderTarget();
	CRenderTarget(int a_nWidth, int a_nHeight, D3DVIEWPORT9* a_stViewport = nullptr);
	~CRenderTarget();

private:
	LPDIRECT3DTEXTURE9	createNewTexture(int width, int height);
	LPDIRECT3DSURFACE9  createTexSurface(LPDIRECT3DTEXTURE9 a_pTex);
	LPDIRECT3DSURFACE9	createNewDepthStencil(int width, int height);
	LPD3DXMESH			createPlaneMesh();

	void createEffect();
protected:
	virtual void preRenderState_SamplerState() {}
	virtual void preRenderState_StencilState() {}
	virtual void preRenderState_ZEnable() {}
	virtual void preRenderState_CullMode() {}

	virtual void postRenderState_SamplerState() {}
	virtual void postRenderState_StencilState() {}
	virtual void postRenderState_ZEnable() {}
	virtual void postRenderState_CullMode() {}

public:
	LPD3DXMESH getPlaneMesh() { return m_pPlaneMesh; }

public:
	void	preDraw();
	void	postDraw();

public:
	RT		m_stRenderTarget;
	LPD3DXEFFECT	m_pCopyEffect = nullptr;
	LPD3DXEFFECT	m_pOutlineEffect = nullptr;
	LPD3DXEFFECT	m_pOutlineBlurEffect = nullptr;
	LPD3DXEFFECT	m_pBlendEffect = nullptr;
	LPD3DXEFFECT	m_pLerpEffect = nullptr;
	LPD3DXEFFECT	m_pNoiseEffect = nullptr;
private:
	LPD3DXMESH	m_pPlaneMesh = nullptr;
};