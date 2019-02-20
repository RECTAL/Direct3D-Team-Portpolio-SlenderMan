#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CRenderObject.h"

//! StaticObject
class CCameraObject;
class CLightObject;
class CSpotLightObject;
class CDecorate_BillboardObj : public CRenderObject
{
private:
	typedef std::vector<LPDIRECT3DTEXTURE9> oSpriteTexture;
public:
	struct STParameters
	{
		CCameraObject*	m_pCamera;
		CLightObject*	m_pDirectional;

		int	m_nNumSpotLight;
		CSpotLightObject**	m_pSpotLight;

		int m_nNumPointLight;
		CLightObject**	m_pPointLight;

		std::string m_oTextureFilepath;
		std::string m_stExtantion;
		int			m_nFrame;
		std::string m_oEffectFilepath;
	};

	struct STVertex
	{
		D3DXVECTOR3	m_stPosition;
		D3DXVECTOR2 m_stUV;
		D3DXVECTOR3 m_stNormal;
		D3DXVECTOR3 m_stBinormal;
		D3DXVECTOR3 m_stTangent;
	};

public:			//constructor , destructor
	CDecorate_BillboardObj(STParameters &a_rstParameters);
	~CDecorate_BillboardObj();

protected:		//interface		

	virtual void preDraw(void)override;
	virtual void postDraw(void)override;
	virtual void doDraw(void) override;		//! interface : IRenderable

public:
	LPD3DXMESH		getMesh() { return m_pMesh; }
	LPD3DXEFFECT	getEffect() { return m_pEffect; }
	std::string&	getTechniqueName() { return m_stTechniqueName; }
	STParameters&	getSTParameters() { return m_stParameters; }
	oSpriteTexture	getSpriteTexture() { return m_oSpriteTexture; }

	bool&			getbIsGet() { return m_bIsGet; }

	virtual void update(void) override;		//! interface : IUpdateable

private:
	void	createPlaneMesh();
	LPD3DXMESH	createPlaneMeshWithX();


protected:
	LPD3DXMESH				m_pMesh = nullptr;
	LPD3DXEFFECT			m_pEffect = nullptr;

	LPDIRECT3DTEXTURE9		m_pTexture;
	STParameters			m_stParameters;
	oSpriteTexture			m_oSpriteTexture;

	bool					m_bIsGet = false;

	float m_fAnimationTimer = 0.0f;
	int m_nTextureOffset = 0;
	int m_nAnimationCount = 0;
	std::string	m_stTechniqueName = "DefaultStaticMesh";

};
