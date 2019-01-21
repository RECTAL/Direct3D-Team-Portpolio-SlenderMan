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
	std::vector<std::string> getAnimationNameList(void);						//! 애니메이션 이름 리스트를 반환한다
	void setTimeScale(float a_fTimeScale);										//! 타임 스케일을 변경한다

public:			// public function :Utility
	/***************************************************/
	//public function : about animation
	/***************************************************/
	void playAnimation(const std::string &a_rAnimationName, bool a_bIsLoop = false);			//! 애니메이션을 재생한다
	void playAnimation(LPD3DXANIMATIONSET a_pAnimationSet, bool a_bIsLoop = false);				//! 애니메이션을 재생한다
	void stopAnimation(void);																	//! 애니메이션을 중지한다

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

