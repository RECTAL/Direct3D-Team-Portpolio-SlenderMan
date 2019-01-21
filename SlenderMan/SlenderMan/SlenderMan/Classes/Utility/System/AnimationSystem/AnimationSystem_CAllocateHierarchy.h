#pragma once

#include "../../../Define/KGlobalDefine.h"

//! 계층 구조 할당자
class CAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:

	//! 본
	struct STBone : public D3DXFRAME
	{
		// 최종 변환 행렬 (즉, 로컬 행렬 * 부모 행렬)
		D3DXMATRIXA16 m_stCombineMatrix;
	};

	//! 메시 컨테이너
	struct STMeshContainer : public D3DXMESHCONTAINER
	{
		DWORD m_nNumBlends;
		LPD3DXMESH m_pSkinnedMesh;

		std::vector<STBone *> m_oBoneList;
		std::vector<D3DXMATRIXA16> m_oBoneMatrixList;
		std::vector<LPDIRECT3DTEXTURE9> m_oTextureList;
		std::vector<D3DXBONECOMBINATION> m_oBoneCombinationList;
	};

	//! 매개 변수
	struct STParameters
	{
		std::string m_oBasepath;
		std::function<LPD3DXMESH(LPD3DXMESHCONTAINER, int)> m_oSkinnedMeshCreator;
	};

public:			// 생성자

	//! 생성자
	CAllocateHierarchy(const STParameters &a_rstParameters);


public:			// 인터페이스 구현

	//! 프레임 (본) 을 생성한다
	virtual HRESULT CALLBACK CreateFrame(LPCSTR a_pszName,
		LPD3DXFRAME *a_pstOutFrame) override;

	//! 메시 컨테이너 (피부) 를 생성한다
	virtual HRESULT CALLBACK CreateMeshContainer(LPCSTR a_pszName,
		const D3DXMESHDATA *a_pstMeshData,
		const D3DXMATERIAL *a_pstXMaterials,
		const D3DXEFFECTINSTANCE *a_pstEffectInstances,
		DWORD a_nNumMaterials,
		const DWORD *a_pAdjacency,
		LPD3DXSKININFO a_pstSkinInfo,
		LPD3DXMESHCONTAINER *a_pstOutMeshContainer) override;

	//! 프레임을 제거한다
	virtual HRESULT CALLBACK DestroyFrame(LPD3DXFRAME a_pstFrame) override;

	//! 메시 컨테이너를 제거한다
	virtual HRESULT CALLBACK DestroyMeshContainer(LPD3DXMESHCONTAINER a_pstMeshContainer) override;

private:			// private 변수

	int m_nMeshContainerNumber = 0;
	STParameters m_stParameters;
};
