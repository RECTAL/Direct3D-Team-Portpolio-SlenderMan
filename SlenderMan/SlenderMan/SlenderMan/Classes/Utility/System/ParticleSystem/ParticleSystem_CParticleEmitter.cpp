#include "ParticleSystem_CParticleEmitter.h"
#include "../../../Function/GlobalFunction.h"
#include "../../Manager/CTimeManager.h"

CParticleEmitter::CParticleEmitter(const STParameters & a_rstParameters)
	:
	m_stParameters(a_rstParameters)
{
	// Do Nothing
}

void CParticleEmitter::update(void)
{
	if (m_bIsEmitEnable) {
		this->createParticles();
	}

	this->updateParticles();
	this->removeParticles();
}

std::vector<CParticleEmitter::STParticle>& CParticleEmitter::getParticleList(void)
{
	return m_oParticleList;
}

void CParticleEmitter::setEmitEnable(bool a_bIsEmitEnable)
{
	m_bIsEmitEnable = a_bIsEmitEnable;
}


void CParticleEmitter::removeParticles(void)
{
	auto oIterator = m_oParticleList.begin();

	while (oIterator != m_oParticleList.end()) {
		if (oIterator->m_bIsActive) {
			++oIterator;
		}
		else {
			oIterator = m_oParticleList.erase(oIterator);
		}
	}
}
