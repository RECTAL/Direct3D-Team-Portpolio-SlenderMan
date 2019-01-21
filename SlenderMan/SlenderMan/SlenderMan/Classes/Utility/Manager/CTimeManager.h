#pragma once

#include "../../Define/KGlobalDefine.h"
#include "../Interface/IUpdateable.h"

//! time Manager
class CTimeManager : public IUpdateable
{
public:
	// redefine type
	typedef std::chrono::system_clock::time_point TTimePoint;

public:

	//! singleton Declare
	DECLARE_SINGLETON(CTimeManager);

	//! initalization
	void init(void);


public:			// interface

	//! interface: IUpdatealbe
	virtual void update(void) override;

public:			// getter,setter

	/**************** getter ******************/
	//! get Delta Time
	float getDeltaTime(void) const;

	//! get Running Time
	float getRunningTime(void) const;

private:			

	//! calculate Delta Time
	float calculateDeltaTime(const TTimePoint &a_rstStartTimePoint,
		const TTimePoint &a_rstEndTimePoint);

private:			

	float m_fDeltaTime = 0.0f;
	float m_fRunningTime = 0.0f;

	TTimePoint m_oPrevTimePoint;
	TTimePoint m_oStartTimePoint;
};
