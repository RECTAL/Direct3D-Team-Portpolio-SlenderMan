#pragma once
#include "../Define/KGlobalDefine.h"

//! Change Float -> DWORD 
DWORD FloatToDWORD(float a_fValue);

//! Create Random Number Type: int
int RandomIntValue(int a_nMin, int a_nMax);

//! Create Random Number Type: float
float RandomFloatValue(float a_fMin, float a_fMax);

//! Run Effect Object pass0 -> passN
void RunEffectLoop(LPD3DXEFFECT a_pEffect,
	const std::string &a_rTechnique,
	const std::function<void(int)> &a_rCallback);

//! Create Ray
STRay CreateRay(const POINT &a_rstPoint);

//! Create Mesh with D3DFVF
LPD3DXMESH CreateMesh(int a_nFaceSize, int a_nVertexSize, DWORD a_nOptions, DWORD a_nFVF);

//! Create Mesh with D3DVERTEXELEMENT9
LPD3DXMESH CreateMesh(int a_nFaceSize, int a_nVertexSize, DWORD a_nOptions, D3DVERTEXELEMENT9 *a_pstElements);

//! Create VertexBuffer
LPDIRECT3DVERTEXBUFFER9 CreateVertexBuffer(int a_nSize, DWORD a_nOptions, DWORD a_nFVF);

//! Create IndexBuffer
LPDIRECT3DINDEXBUFFER9 CreateIndexBuffer(int a_nSize, DWORD a_nOptions, D3DFORMAT a_nFormat);
