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


//! °æ°è º¼·ý
struct STBoundingBox
{
	D3DXVECTOR3 m_stMinPoint;
	D3DXVECTOR3 m_stMaxPoint;
};

//! °´Ã¼ º¼·ý
struct STObjectBox
{
	float m_fHalfLengths[3];

	D3DXVECTOR3 m_stCenter;
	D3DXVECTOR3 m_astDirections[3];
};

//! °æ°è ±¸
struct STBoundingSphere
{
	float m_fRadius;
	D3DXVECTOR3 m_stCenter;
};

