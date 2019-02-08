#include "AnimationSystem_CAllocateHierarchy.h"
#include "../../Manager/CResourceManager.h"

CAllocateHierarchy::CAllocateHierarchy(const STParameters & a_rstParameters)
	:
	m_stParameters(a_rstParameters)
{
	// Do Nothing
}

HRESULT CAllocateHierarchy::CreateFrame(LPCSTR a_pszName, LPD3DXFRAME * a_pstOutFrame)
{
	auto pstBone = new STBone();

	// 프레임 이름이 있을 경우
	if (a_pszName != nullptr) {
		pstBone->Name = (char *)malloc(sizeof(char) * (strlen(a_pszName) + 1));
		strcpy(pstBone->Name, a_pszName);
	}

	D3DXMatrixIdentity(&pstBone->m_stCombineMatrix);
	D3DXMatrixIdentity(&pstBone->TransformationMatrix);

	*a_pstOutFrame = pstBone;
	return S_OK;
}

HRESULT CAllocateHierarchy::CreateMeshContainer(LPCSTR a_pszName,
	const D3DXMESHDATA * a_pstMeshData,
	const D3DXMATERIAL * a_pstXMaterials,
	const D3DXEFFECTINSTANCE * a_pstEffectInstances,
	DWORD a_nNumMaterials,
	const DWORD * a_pAdjacency,
	LPD3DXSKININFO a_pstSkinInfo,
	LPD3DXMESHCONTAINER * a_pstOutMeshContainer)
{
	auto pstMeshContainer = new STMeshContainer();

	// 일반 메시 일 경우
	if (a_pstMeshData->Type == D3DXMESHDATATYPE::D3DXMESHTYPE_MESH) {
		/*
		MeshData 의 메시 객체는 함수 호출이 완료되고 나면 Release 함수가 호출되기
		때문에 해당 메시 객체를 계속 사용하기 위해서는 AddRef 함수를 통해서 레퍼런스
		카운트를 증가시켜줘야한다.
		*/
		// 메시 정보를 설정한다
		pstMeshContainer->MeshData = *a_pstMeshData;
		pstMeshContainer->MeshData.pMesh->AddRef();

		// 인접 정보를 설정한다
		pstMeshContainer->pAdjacency = (DWORD *)malloc(sizeof(DWORD) * (a_pstMeshData->pMesh->GetNumFaces() * 3));
		CopyMemory(pstMeshContainer->pAdjacency, a_pAdjacency, sizeof(DWORD) * (a_pstMeshData->pMesh->GetNumFaces() * 3));

		// 메시 컨테이너 이름이 있을 경우
		if (a_pszName != nullptr) {
			pstMeshContainer->Name = (char *)malloc(sizeof(char) * (strlen(a_pszName) + 1));
			strcpy(pstMeshContainer->Name, a_pszName);
		}

		// 재질 정보를 설정한다
		// {
		pstMeshContainer->NumMaterials = a_nNumMaterials;

		pstMeshContainer->pMaterials = (LPD3DXMATERIAL)malloc(sizeof(D3DXMATERIAL) * a_nNumMaterials);
		CopyMemory(pstMeshContainer->pMaterials, a_pstXMaterials, sizeof(D3DXMATERIAL) * a_nNumMaterials);

		for (int i = 0; i < a_nNumMaterials; ++i) {
			LPDIRECT3DTEXTURE9 pTexture = nullptr;

			if (a_pstXMaterials[i].pTextureFilename != nullptr) {
				char szFilepath[MAX_PATH] = "";

				sprintf(szFilepath, "%s%s", m_stParameters.m_oBasepath.c_str(),
					a_pstXMaterials[i].pTextureFilename);

				pTexture = GET_TEXTURE(szFilepath,GOUST_VALUE,GOUST_VALUE);
			}

			pstMeshContainer->m_oTextureList.push_back(pTexture);
		}
		// }

		// 스킨 정보를 설정한다
		// {
		if (a_pstSkinInfo != nullptr) {
			pstMeshContainer->pSkinInfo = a_pstSkinInfo;
			pstMeshContainer->pSkinInfo->AddRef();

			/*
			GetBoneOffsetMatrix 함수는 3 차원 모델을 구성하고 있는 각 본에 대한
			위치 정보를 포함하고 있는 행렬을 반환하는 역할을 한다.

			각 본은 인덱스의 개념으로 0 ~ GetNumBones() - 1 까지의 범위로
			구성되어있다.
			*/
			for (int i = 0; i < a_pstSkinInfo->GetNumBones(); ++i) {
				D3DXMATRIXA16 stMatrix = *a_pstSkinInfo->GetBoneOffsetMatrix(i);
				pstMeshContainer->m_oBoneMatrixList.push_back(stMatrix);
			}

			m_nMeshContainerNumber += 1;
			pstMeshContainer->m_pSkinnedMesh = m_stParameters.m_oSkinnedMeshCreator(pstMeshContainer, m_nMeshContainerNumber);
		}
		// }
	}

	*a_pstOutMeshContainer = pstMeshContainer;
	return S_OK;
}

HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME a_pstFrame)
{
	SAFE_FREE(a_pstFrame->Name);
	SAFE_DELETE(a_pstFrame);

	return S_OK;
}

HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER a_pstMeshContainer)
{
	auto pstMeshContainer = (STMeshContainer *)a_pstMeshContainer;

	SAFE_FREE(pstMeshContainer->Name);
	SAFE_FREE(pstMeshContainer->pAdjacency);
	SAFE_FREE(pstMeshContainer->pMaterials);

	SAFE_RELEASE(pstMeshContainer->pSkinInfo);
	SAFE_RELEASE(pstMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pstMeshContainer->m_pSkinnedMesh);

	SAFE_DELETE(pstMeshContainer);
	return S_OK;
}
