#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CRenderObject.h"
#include "../../System/AnimationSystem/AnimationSystem_CAllocateHierarchy.h"

class CAnimationController;
//! SkinnedObject 
class SkinnedObject : public CRenderObject
{
public:
	struct STParameters
	{
		std::string m_oMeshFilepath;
		std::string m_oEffectFilepath;
	};

public:			// constructor, destructor

	SkinnedObject(const STParameters &a_rstParameters);
	virtual ~SkinnedObject(void);

public:			//interface
	virtual void update(void) override;					//interface : IUpdateable


protected:		// override function 
	/***********************************************************/
	//protected override function : about draw
	/***********************************************************/
	virtual void preDraw(void) override;				//! ��ü�� �׸��� ��
	virtual void doDraw(void) override;					//! ��ü�� �׸���
	virtual void postDraw(void) override;				//! ��ü�� �׸� ��

public:			// getter, setter
	/***********************************************************/
	//getter
	/***********************************************************/
	std::vector<std::string> getAnimationNameList(void);					//! �ִϸ��̼� �̸� ����Ʈ�� ��ȯ�Ѵ�


	/***********************************************************/
	//setter
	/***********************************************************/
	void setTimeScale(float a_fTimeScale);									//! Ÿ�� �������� �����Ѵ�

public:			//public function: Utility
	/***********************************************************/
	//public function : about Animation
	/***********************************************************/
	void playAnimation(const std::string &a_rAnimationName, bool a_bIsLoop = false);			//! �ִϸ��̼��� ����Ѵ�
	void stopAnimation(void);																	//! �ִϸ��̼��� �����Ѵ�


private:		// private function

	/***********************************************************/
	//private function : about setup,update,draw
	/***********************************************************/
	void updateBoneMatrix(LPD3DXFRAME a_pstFrame, const D3DXMATRIXA16 &a_rstMatrix);				//! �� ����� �����Ѵ�
	void drawBone(LPD3DXFRAME a_pstFrame);															//! ���� �׸���
	void drawMeshContainer(LPD3DXFRAME a_pstFrame,LPD3DXMESHCONTAINER a_pstMeshContainer);			//! �޽� �����̳ʸ� �׸���
	void setupBone(LPD3DXFRAME a_pstFrame);															//! ���� �����Ѵ�
	void setupBoneOnMeshContainer(LPD3DXFRAME a_pstFrame,LPD3DXMESHCONTAINER a_pstMeshContainer);	//! ���� �޽� �����̳ʿ� �����Ѵ�
	
	

	/***********************************************************/
	//private function : about createSkinnedMesh
	/***********************************************************/
	LPD3DXMESH createSkinnedMeshFromX(const std::string &a_rFilepath);								//! ��Ų�� �޽ø� �����Ѵ�
	LPD3DXMESH createSkinnedMesh(LPD3DXMESHCONTAINER a_pstMeshContainer,int a_nMeshContainerNumber);//! ��Ų�� �޽ø� �����Ѵ�

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
