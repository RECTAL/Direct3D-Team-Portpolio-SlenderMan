#include "CSkinnedObject.h"
#include "../../../Function/GlobalFunction.h"
#include "../../Base/CDirect3DApplication.h"
#include "../../System/AnimationSystem/AnimationSystem_CAllocateHierarchy.h"
#include "../../System/AnimationSystem/AnimationSystem_CAnimationController.h"
#include "../../Object/LightObject/CLightObject.h"
#include "../../Object/CameraObject/CCameraObject.h"
#include "../../Manager/CDeviceManager.h"
#include "../../Manager/CResourceManager.h"
#include "../../Manager/CTimeManager.h"


CSkinnedObject::CSkinnedObject(const STParameters & a_rstParameters)
	:
	m_stParameters(a_rstParameters)
{
	if (a_rstParameters.m_oMeshFilepath.rfind("/") != std::string::npos) {
		m_oBasepath = a_rstParameters.m_oMeshFilepath.substr(0,
			a_rstParameters.m_oMeshFilepath.rfind("/") + 1);
	}

	m_pEffect = GET_EFFECT(a_rstParameters.m_oEffectFilepath);
	m_pMesh = this->createSkinnedMeshFromX(a_rstParameters.m_oMeshFilepath);

	auto stBoundingBox = CreateBoundingBox(m_pMesh);
	auto stBoundingSphere = CreateBoundingSphere(m_pMesh);

	this->setBoundingBox(stBoundingBox);
	this->setBoundingSphere(stBoundingSphere);
}

CSkinnedObject::~CSkinnedObject(void)
{
	SAFE_DELETE(m_pAnimationController);
}

std::vector<std::string> CSkinnedObject::getAnimationNameList(void)
{
	return m_pAnimationController->getAnimationNameList();
}

void CSkinnedObject::setTimeScale(float a_fTimeScale)
{
	m_pAnimationController->setTimeScale(a_fTimeScale);
}

void CSkinnedObject::update(void)
{
	CRenderObject::update();
	m_pAnimationController->update();

	D3DXMATRIXA16 stWorldMatrix = this->getFinalWorldMatrix();
	this->updateBoneMatrix(m_pstRootBone, stWorldMatrix);
}

void CSkinnedObject::playAnimation(const std::string & a_rAnimationName, bool a_bIsLoop)
{
	m_pAnimationController->playAnimation(a_rAnimationName, a_bIsLoop);
}

void CSkinnedObject::stopAnimation(void)
{
	m_pAnimationController->stopAnimation();
}

void CSkinnedObject::preDraw(void)
{
	CRenderObject::preDraw();
	GET_DEVICE()->SetRenderState(D3DRS_LIGHTING, false);
}

void CSkinnedObject::doDraw(void)
{
	CRenderObject::doDraw();
	D3DXMATRIXA16 stViewMatrix = m_stParameters.m_pCameraObj->getViewMatrix();
	D3DXMATRIXA16 stProjectionMatrix = m_stParameters.m_pCameraObj->getProjectionMatrix();

	m_pEffect->SetMatrix("g_stViewMatrix", &stViewMatrix);
	m_pEffect->SetMatrix("g_stProjectionMatrix", &stProjectionMatrix);

	D3DXVECTOR4 stViewPosition(m_stParameters.m_pCameraObj->getPosition(), 1.0f);
	D3DXVECTOR4 stLightDirection(m_stParameters.m_pDirectionalLight->getForwardDirection(), 0.0f);

	m_pEffect->SetVector("g_stViewPosition", &stViewPosition);
	m_pEffect->SetVector("g_stLightDirection", &stLightDirection);


	this->drawBone(m_pstRootBone);
}

void CSkinnedObject::postDraw(void)
{
	CRenderObject::postDraw();
	GET_DEVICE()->SetRenderState(D3DRS_LIGHTING, true);

}

void CSkinnedObject::updateBoneMatrix(LPD3DXFRAME a_pstFrame, const D3DXMATRIXA16 & a_rstMatrix)
{
	auto pstBone = (CAllocateHierarchy::STBone *)a_pstFrame;
	pstBone->m_stCombineMatrix = pstBone->TransformationMatrix * a_rstMatrix;

	// 형제 본 정보가 있을 경우
	if (pstBone->pFrameSibling != nullptr) {
		this->updateBoneMatrix(pstBone->pFrameSibling, a_rstMatrix);
	}

	// 자식 본 정보가 있을 경우
	if (pstBone->pFrameFirstChild != nullptr) {
		this->updateBoneMatrix(pstBone->pFrameFirstChild, pstBone->m_stCombineMatrix);
	}
}

void CSkinnedObject::drawBone(LPD3DXFRAME a_pstFrame)
{
	auto pstMeshContainer = a_pstFrame->pMeshContainer;

	while (pstMeshContainer != nullptr) {
		this->drawMeshContainer(a_pstFrame, pstMeshContainer);
		pstMeshContainer = pstMeshContainer->pNextMeshContainer;
	}

	// 형제 본 정보가 있을 경우
	if (a_pstFrame->pFrameSibling != nullptr) {
		this->drawBone(a_pstFrame->pFrameSibling);
	}

	// 자식 본 정보가 있을 경우
	if (a_pstFrame->pFrameFirstChild != nullptr) {
		this->drawBone(a_pstFrame->pFrameFirstChild);
	}
}

void CSkinnedObject::drawMeshContainer(LPD3DXFRAME a_pstFrame, LPD3DXMESHCONTAINER a_pstMeshContainer)
{
	if (a_pstMeshContainer->pSkinInfo != nullptr) {

		auto pstMeshContainer = (CAllocateHierarchy::STMeshContainer *)a_pstMeshContainer;

		for (int i = 0; i < pstMeshContainer->m_oBoneCombinationList.size(); ++i) {
			int nNumBlends = 0;
			auto stBoneCombination = pstMeshContainer->m_oBoneCombinationList[i];

			// 뼈대 가중치 연산 횟수를 탐색한다
			for (int j = 0; j < pstMeshContainer->m_nNumBlends; ++j) {
				if (stBoneCombination.BoneId[j] != UINT_MAX) {
					nNumBlends = j;
				}
			}

			// 월드 행렬을 설정한다
			// {
			D3DXMATRIXA16 astWorldMatrices[4];
			int nSize = sizeof(astWorldMatrices) / sizeof(astWorldMatrices[0]);

			for (int j = 0; j < nSize; ++j) {
				D3DXMatrixIdentity(astWorldMatrices + j);
			}
			// }

			for (int j = 0; j < pstMeshContainer->m_nNumBlends; ++j) {
				int nBoneID = stBoneCombination.BoneId[j];

				if (nBoneID != UINT_MAX) {
					auto pstBone = pstMeshContainer->m_oBoneList[nBoneID];
					D3DXMATRIXA16 stBoneMatrix = pstMeshContainer->m_oBoneMatrixList[nBoneID];

					astWorldMatrices[j] = stBoneMatrix * pstBone->m_stCombineMatrix;
				}
			}

			m_pEffect->SetInt("g_nNumBlends", nNumBlends);
			m_pEffect->SetMatrixArray("g_stWorldMatrices", astWorldMatrices, 4);

			int nAttributeID = stBoneCombination.AttribId;
			m_pEffect->SetTexture("g_pTexture", pstMeshContainer->m_oTextureList[nAttributeID]);

			RunEffectLoop(m_pEffect, "DefaultSkinnedMesh", [=](int a_nPassNumber) -> void {
				pstMeshContainer->m_pSkinnedMesh->DrawSubset(i);
			});
		}
	}
}

void CSkinnedObject::setupBone(LPD3DXFRAME a_pstFrame)
{
	if (a_pstFrame->pMeshContainer != nullptr) {
		this->setupBoneOnMeshContainer(a_pstFrame, a_pstFrame->pMeshContainer);
	}

	// 형제 본 정보가 있을 경우
	if (a_pstFrame->pFrameSibling != nullptr) {
		this->setupBone(a_pstFrame->pFrameSibling);
	}

	// 자식 본 정보가 있을 경우
	if (a_pstFrame->pFrameFirstChild != nullptr) {
		this->setupBone(a_pstFrame->pFrameFirstChild);
	}
}

void CSkinnedObject::setupBoneOnMeshContainer(LPD3DXFRAME a_pstFrame, LPD3DXMESHCONTAINER a_pstMeshContainer)
{
	auto pstMeshContainer = (CAllocateHierarchy::STMeshContainer *)a_pstMeshContainer;

	if (pstMeshContainer->pSkinInfo != nullptr) {
		int nNumBones = pstMeshContainer->pSkinInfo->GetNumBones();

		for (int i = 0; i < nNumBones; ++i) {
			/*
			GetBoneName 함수는 본의 식별자를 이용해서 해당 본의 이름을
			반환하는 역할을 한다.
			*/
			auto pszBoneName = pstMeshContainer->pSkinInfo->GetBoneName(i);

			/*
			D3DXFrameFind 함수는 루트 본과 본의 이름을 이용해서 특정 본을
			탐색하는 역할을 한다.
			*/
			auto pstBone = D3DXFrameFind(m_pstRootBone, pszBoneName);
			pstMeshContainer->m_oBoneList.push_back((CAllocateHierarchy::STBone *)pstBone);
		}
	}
}

LPD3DXMESH CSkinnedObject::createSkinnedMeshFromX(const std::string & a_rFilepath)
{
	CAllocateHierarchy::STParameters stParameters = {
		m_oBasepath,
		std::bind(&CSkinnedObject::createSkinnedMesh, this, std::placeholders::_1, std::placeholders::_2)
	};

	CAllocateHierarchy oAllocateHierarchy(stParameters);
	LPD3DXANIMATIONCONTROLLER pAnimationController = nullptr;

	// 메시를 생성한다
	D3DXLoadMeshHierarchyFromXA(a_rFilepath.c_str(),
		D3DXMESH_MANAGED,
		GET_DEVICE(),
		&oAllocateHierarchy,
		NULL,
		(LPD3DXFRAME *)&m_pstRootBone,			// 모델을 구성하는 본 중에 최상위 본
		&pAnimationController);					// 애니메이션을 제어하기 위한 컨트롤러

	// 본을 설정한다
	this->setupBone(m_pstRootBone);

	// 애니메이션 컨트롤러를 생성한다
	m_pAnimationController = new CAnimationController(pAnimationController);

	/*
	모델링 파일의 구조에 따라서 MeshContainer 는 N 개 이상이 생성 될 수 있기 때문에
	해당 MeshContainer 중에서 가장 첫번째 객체가 지니고 있는 메시 정보를 반환한다.

	(즉, 일반적으로 가장 처음에 해당하는 MeshContainer 가 모델링 정보의 본체에
	해당한다.)
	*/
	return m_oMeshContainerList.front()->m_pSkinnedMesh;
}

LPD3DXMESH CSkinnedObject::createSkinnedMesh(LPD3DXMESHCONTAINER a_pstMeshContainer, int a_nMeshContainerNumber)
{
	auto pstMeshContainer = (CAllocateHierarchy::STMeshContainer *)a_pstMeshContainer;

	DWORD nNumBlends = 0;
	DWORD nNumBoneCombinations = 0;

	LPD3DXMESH pMesh = nullptr;
	LPD3DXBUFFER pBoneCombinationBuffer = nullptr;

	// 스킨드 메시로 변환한다
	pstMeshContainer->pSkinInfo->ConvertToBlendedMesh(pstMeshContainer->MeshData.pMesh,
		pstMeshContainer->MeshData.pMesh->GetOptions() | D3DXMESHOPT_VERTEXCACHE,
		pstMeshContainer->pAdjacency,
		pstMeshContainer->pAdjacency,
		NULL,
		NULL,
		&nNumBlends,
		&nNumBoneCombinations,
		&pBoneCombinationBuffer,
		&pMesh);

	// 정점 정보를 설정한다
	// {
	D3DVERTEXELEMENT9 astElements[] = {
		0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0,
		0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0,
		0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0,
		0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0,
		0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0,
		0, 56, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0,
		D3DDECL_END()
	};
		/*
		스키닝 애니메이션을 처리하기 위해서는 정점 정보에 반드시 뼈대 가중치 정보에
		해당하는 BLENDWEIGHT 시멘틱이 반드시 포함되어있어야한다.
		*/

	auto pOriginMesh = pMesh;

	// 사본 메시를 생성한다
	pOriginMesh->CloneMesh(pOriginMesh->GetOptions(),
		astElements,
		GET_DEVICE(),
		&pMesh);

	// 법선 정보를 계산한다
	D3DXComputeNormals(pMesh, pstMeshContainer->pAdjacency);

	// 접선, 종법선 정보를 계산한다
	D3DXComputeTangent(pMesh, 0, 0, 0, TRUE, pstMeshContainer->pAdjacency);
	// }

	// 스키닝 정보를 설정한다
	// {
	pstMeshContainer->m_nNumBlends = nNumBlends;
	auto pstBoneCombinations = (LPD3DXBONECOMBINATION)pBoneCombinationBuffer->GetBufferPointer();

	// 본 조합 정보를 복사한다
	for (int i = 0; i < nNumBoneCombinations; ++i) {
		D3DXBONECOMBINATION stBoneCombination = pstBoneCombinations[i];

		auto stCopiedBoneCombination = stBoneCombination;
		stCopiedBoneCombination.BoneId = (DWORD *)malloc(sizeof(DWORD) * nNumBlends);

		CopyMemory(stCopiedBoneCombination.BoneId, stBoneCombination.BoneId, sizeof(DWORD) * nNumBlends);
		pstMeshContainer->m_oBoneCombinationList.push_back(stCopiedBoneCombination);
	}
	// }

	SAFE_RELEASE(pOriginMesh);
	SAFE_RELEASE(pBoneCombinationBuffer);

	m_oMeshContainerList.push_back(pstMeshContainer);
	return pMesh;
}
