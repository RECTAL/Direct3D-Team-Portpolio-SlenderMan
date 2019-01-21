#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Interface/IUpdateable.h"

//! AnimationController
class CAnimationController : public IUpdateable
{

public:			// constructor, destructor
	CAnimationController(LPD3DXANIMATIONCONTROLLER a_pAnimationController);
	virtual ~CAnimationController(void);

public:			// interface

	virtual void update(void) override;		//interface: IUpdateable

public:			// getter, setter
	/***************************************************/
	//getter
	/***************************************************/
	std::vector<std::string> getAnimationNameList(void);						//! �ִϸ��̼� �̸� ����Ʈ�� ��ȯ�Ѵ�
	void setTimeScale(float a_fTimeScale);										//! Ÿ�� �������� �����Ѵ�

public:			// public function :Utility
	/***************************************************/
	//public function : about animation
	/***************************************************/
	void playAnimation(const std::string &a_rAnimationName, bool a_bIsLoop = false);			//! �ִϸ��̼��� ����Ѵ�
	void playAnimation(LPD3DXANIMATIONSET a_pAnimationSet, bool a_bIsLoop = false);				//! �ִϸ��̼��� ����Ѵ�
	void stopAnimation(void);																	//! �ִϸ��̼��� �����Ѵ�

private:		

	bool m_bIsLoop = false;
	bool m_bIsPlaying = false;

	float m_fTimeScale = 0.0f;
	float m_fMixedTime = 0.0f;
	float m_fLeftMixedTime = 0.0f;

	LPD3DXANIMATIONSET m_pAnimationSet = nullptr;
	LPD3DXANIMATIONSET m_pNextAnimationSet = nullptr;
	LPD3DXANIMATIONCONTROLLER m_pAnimationController = nullptr;

	std::vector<std::string> m_oAnimationNameList;
	std::unordered_map<std::string, LPD3DXANIMATIONSET> m_oAnimationSetList;
};

