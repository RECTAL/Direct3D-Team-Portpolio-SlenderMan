#pragma once
#include "../KGlobalDefine.h"


struct fPOINT
{
	float x;
	float y;
	void operator=(fPOINT& obj)
	{
		x = obj.x;
		y = obj.y;
	}
};

//! Ray Struct Define
struct STRay
{
	D3DXVECTOR3 m_stOrigin;
	D3DXVECTOR3 m_stDirection;
};


//! Static Mesh Struct Define
struct STStaticMesh
{
	DWORD m_nNumMaterials;
	LPD3DXMESH m_pStaticMesh;

	std::vector<D3DMATERIAL9> m_oMaterialList;
	std::vector<LPDIRECT3DTEXTURE9> m_oTextureList;
};

//! Wav Sound Struct Define
struct STWaveSound
{
	BYTE *m_pnBytes;

	DWORD m_nNumBytes;
	WAVEFORMATEX m_stWaveFormat;
};

struct STBoundingSphere
{
	D3DXVECTOR3 m_stPosition;
	float		m_fRadius;
};