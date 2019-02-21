#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../Object/CameraObject/CCameraObject.h"
#include "../../Base/CRenderObject.h"

class CQuadTree;
class CSpotLightObject;
class CLightObject;
class CTerrainObject:public CRenderObject
{
public:
	enum
	{
		MAX_TERRAIN_TEX = 4
	};

	struct TERRAINVERTEX
	{
		D3DXVECTOR3 m_stPosition;
		D3DXVECTOR2 m_stUV;
		D3DXVECTOR3 m_stNormal;
		D3DXVECTOR3	m_stBinormal;
		D3DXVECTOR3 m_stTangent;
	};

	struct STParameters
	{
		CCameraObject*		m_pCamera;
		CLightObject*		m_pLight;
		int					m_nSmoothLevel;
		D3DXVECTOR3			m_vfScale;

		int	m_nNumSpotLight;
		CSpotLightObject**	m_pSpotLight;

		int m_nNumPointLight;
		CLightObject**	m_pPointLight;

		std::string m_oSplatFilepath;
		std::string m_oHeightFilepath;
		std::string m_oEffectFilepath;

		SIZE m_stMapSize;
		std::vector<std::string> m_oTextureFilepathList;
	};

public:
	CTerrainObject(const STParameters& a_stParameters);
	~CTerrainObject();


public:
	HRESULT		create();

	int				getCXDIB() { return m_cxDIB; }
	int				getCZDIB() { return m_czDIB; }
	float			getCXTerrain() { return m_cxTerrain; }
	float			getCYTerrain() { return m_cyTerrain; }
	float			getCZTerrain() { return m_czTerrain; }
	float			getHeight(D3DXVECTOR3 a_stPosition);
	int				findIndex(D3DXVECTOR3 a_stPosition);
	int				getTriangles() { return m_nTriangles; }
	D3DXVECTOR3*	getHeightMap() { return m_pHeightMap; }
	
	LPD3DXMESH		getTerrainMesh() { return m_pTerrainMesh; }
	LPD3DXEFFECT	getEffect() { return m_pEffect; }
	std::string&	getTechniqueName(){ return m_stTechniqueName; }
	STParameters&	getSTParameters() { return m_stParameters; }


	virtual	void	doDraw(void) override;
	virtual void	update(void) override;

	bool			terrainPicking(D3DXVECTOR3& a_stPosition);
private:
	HRESULT			destroy();
	HRESULT			loadTextures(std::vector<std::string>&  lpTexFilename);
	HRESULT			buildHeightMap(std::string lpFilename);
	HRESULT			buildQuadTree(D3DXVECTOR3* pHeightMap);
	HRESULT			createTerrainMesh();
	HRESULT			render();


private:
	int								m_cxDIB;
	int								m_czDIB;
	int								m_cyDIB;

	int								m_cxTerrain;
	int								m_cyTerrain;
	int								m_czTerrain;

	D3DXVECTOR3						m_vfScale;
	LPDIRECT3DTEXTURE9				m_pSplatTexture = nullptr;
	LPDIRECT3DTEXTURE9				m_pBumpTexture = nullptr;
	LPDIRECT3DTEXTURE9				m_pTex[CTerrainObject::MAX_TERRAIN_TEX];
	LPD3DXMESH						m_pTerrainMesh;
	int								m_nTriangles;
	D3DXVECTOR3*					m_pHeightMap;
	CQuadTree*						m_pQuadTree;
	CCameraObject*					m_pCamera;
	CLightObject*					m_pLightObj;
	STParameters					m_stParameters;
	LPD3DXEFFECT					m_pEffect = nullptr;
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration = nullptr;
	std::string						m_stTechniqueName = "DefaultTerrain";

	std::vector<float>				m_oHeightList;
};