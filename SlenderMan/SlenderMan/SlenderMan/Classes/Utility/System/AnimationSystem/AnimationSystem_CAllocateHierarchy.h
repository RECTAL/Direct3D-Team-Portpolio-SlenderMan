#pragma once

#include "../../../Define/KGlobalDefine.h"

//! ���� ���� �Ҵ���
class CAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:

	//! ��
	struct STBone : public D3DXFRAME
	{
		// ���� ��ȯ ��� (��, ���� ��� * �θ� ���)
		D3DXMATRIXA16 m_stCombineMatrix;
	};

	//! �޽� �����̳�
	struct STMeshContainer : public D3DXMESHCONTAINER
	{
		DWORD m_nNumBlends;
		LPD3DXMESH m_pSkinnedMesh;

		std::vector<STBone *> m_oBoneList;
		std::vector<D3DXMATRIXA16> m_oBoneMatrixList;
		std::vector<LPDIRECT3DTEXTURE9> m_oTextureList;
		std::vector<D3DXBONECOMBINATION> m_oBoneCombinationList;
	};

	//! �Ű� ����
	struct STParameters
	{
		std::string m_oBasepath;
		std::function<LPD3DXMESH(LPD3DXMESHCONTAINER, int)> m_oSkinnedMeshCreator;
	};

public:			// ������

	//! ������
	CAllocateHierarchy(const STParameters &a_rstParameters);


public:			// �������̽� ����

	//! ������ (��) �� �����Ѵ�
	virtual HRESULT CALLBACK CreateFrame(LPCSTR a_pszName,
		LPD3DXFRAME *a_pstOutFrame) override;

	//! �޽� �����̳� (�Ǻ�) �� �����Ѵ�
	virtual HRESULT CALLBACK CreateMeshContainer(LPCSTR a_pszName,
		const D3DXMESHDATA *a_pstMeshData,
		const D3DXMATERIAL *a_pstXMaterials,
		const D3DXEFFECTINSTANCE *a_pstEffectInstances,
		DWORD a_nNumMaterials,
		const DWORD *a_pAdjacency,
		LPD3DXSKININFO a_pstSkinInfo,
		LPD3DXMESHCONTAINER *a_pstOutMeshContainer) override;

	//! �������� �����Ѵ�
	virtual HRESULT CALLBACK DestroyFrame(LPD3DXFRAME a_pstFrame) override;

	//! �޽� �����̳ʸ� �����Ѵ�
	virtual HRESULT CALLBACK DestroyMeshContainer(LPD3DXMESHCONTAINER a_pstMeshContainer) override;

private:			// private ����

	int m_nMeshContainerNumber = 0;
	STParameters m_stParameters;
};
