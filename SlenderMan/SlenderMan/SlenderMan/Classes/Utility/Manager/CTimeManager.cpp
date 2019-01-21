#include "CTimeManager.h"

void CTimeManager::init(void)
{
	m_oPrevTimePoint = std::chrono::system_clock::now();
	m_oStartTimePoint = std::chrono::system_clock::now();
}

void CTimeManager::update(void)
{
	auto oCurrentTimePoint = std::chrono::system_clock::now();

	m_fDeltaTime = this->calculateDeltaTime(m_oPrevTimePoint, oCurrentTimePoint);
	m_fRunningTime = this->calculateDeltaTime(m_oStartTimePoint, oCurrentTimePoint);

	m_oPrevTimePoint = oCurrentTimePoint;
}

float CTimeManager::getDeltaTime(void) const
{
	return m_fDeltaTime;
}

float CTimeManager::getRunningTime(void) const
{
	return m_fRunningTime;
}

float CTimeManager::calculateDeltaTime(const TTimePoint & a_rstStartTimePoint, const TTimePoint & a_rstEndTimePoint)
{
	return std::chrono::duration<float>(a_rstEndTimePoint - a_rstStartTimePoint).count();
}
