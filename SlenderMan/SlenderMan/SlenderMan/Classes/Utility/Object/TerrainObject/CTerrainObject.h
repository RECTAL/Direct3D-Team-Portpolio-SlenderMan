#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../Object/CameraObject/CCameraObject.h"
#include "../../Base/CRenderObject.h"

class CQuadTree;
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
		int					m_nSmoothLevel;
		D3DXVECTOR3			m_vfScale;

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
	LPD3DXEFFECT	getEffect() { return m_pEffect; }
	std::string&	getTechniqueName(){ return m_stTechniqueName; }

	virtual	void	doDraw(void) override;
	virtual void	update(void) override;

	void			mapToolUpdate();
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
	LPDIRECT3DTEXTURE9				m_pSplatTexture;
	LPDIRECT3DTEXTURE9				m_pTex[CTerrainObject::MAX_TERRAIN_TEX];
	LPD3DXMESH						m_pTerrainMesh;
	int								m_nTriangles;
	D3DXVECTOR3*					m_pHeightMap;
	CQuadTree*						m_pQuadTree;
	CCameraObject*					m_pCamera;

	STParameters					m_stParameters;
	LPD3DXEFFECT					m_pEffect = nullptr;
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration = nullptr;
	std::string						m_stTechniqueName;

	std::vector<float>				m_oHeightList;
};