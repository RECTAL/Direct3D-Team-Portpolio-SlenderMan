#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CRenderObject.h"
#include "../../System/AnimationSystem/AnimationSystem_CAllocateHierarchy.h"

class CAnimationController;
class CCameraObject;
class CLightObject;
//! SkinnedObject 
class CSkinnedObject : public CRenderObject
{
public:
	struct STParameters
	{
		CCameraObject* m_pCameraObj;
		CLightObject*  m_pDirectionalLight;
		std::string m_oMeshFilepath;
		std::string m_oEffectFilepath;
	};

public:			// constructor, destructor

	CSkinnedObject(const STParameters &a_rstParameters);
	virtual ~CSkinnedObject(void);

public:			//interface
	virtual void update(void) override;					//interface : IUpdateable


protected:		// override function 
	/***********************************************************/
	//protected override function : about draw
	/***********************************************************/
	virtual void preDraw(void) override;				//! 물체를 그리기 전
	virtual void doDraw(void) override;					//! 물체를 그린다
	virtual void postDraw(void) override;				//! 물체를 그린 후

public:			// getter, setter
	/***********************************************************/
	//getter
	/***********************************************************/
	std::vector<std::string> getAnimationNameList(void);					//! 애니메이션 이름 리스트를 반환한다


	/***********************************************************/
	//setter
	/***********************************************************/
	void setTimeScale(float a_fTimeScale);									//! 타임 스케일을 변경한다

public:			//public function: Utility
	/***********************************************************/
	//public function : about Animation
	/***********************************************************/
	void playAnimation(const std::string &a_rAnimationName, bool a_bIsLoop = false);			//! 애니메이션을 재생한다
	void stopAnimation(void);																	//! 애니메이션을 중지한다


private:		// private function

	/***********************************************************/
	//private function : about setup,update,draw
	/***********************************************************/
	void updateBoneMatrix(LPD3DXFRAME a_pstFrame, const D3DXMATRIXA16 &a_rstMatrix);				//! 본 행렬을 갱신한다
	void drawBone(LPD3DXFRAME a_pstFrame);															//! 본을 그린다
	void drawMeshContainer(LPD3DXFRAME a_pstFrame,LPD3DXMESHCONTAINER a_pstMeshContainer);			//! 메시 컨테이너를 그린다
	void setupBone(LPD3DXFRAME a_pstFrame);															//! 본을 설정한다
	void setupBoneOnMeshContainer(LPD3DXFRAME a_pstFrame,LPD3DXMESHCONTAINER a_pstMeshContainer);	//! 본을 메시 컨테이너에 설정한다
	
	

	/***********************************************************/
	//private function : about createSkinnedMesh
	/***********************************************************/
	LPD3DXMESH createSkinnedMeshFromX(const std::string &a_rFilepath);								//! 스킨드 메시를 생성한다
	LPD3DXMESH createSkinnedMesh(LPD3DXMESHCONTAINER a_pstMeshContainer,int a_nMeshContainerNumber);//! 스킨드 메시를 생성한다

protected:			

	LPD3DXMESH m_pMesh = nullptr;
	LPD3DXEFFECT m_pEffect = nullptr;

	std::string m_stTechName;
	std::string m_oBasepath = "";
	STParameters m_stParameters;
	CAllocateHierarchy::STBone *m_pstRootBone = nullptr;

	CAnimationController *m_pAnimationController = nullptr;
	std::vector<CAllocateHierarchy::STMeshContainer *> m_oMeshContainerList;
};
