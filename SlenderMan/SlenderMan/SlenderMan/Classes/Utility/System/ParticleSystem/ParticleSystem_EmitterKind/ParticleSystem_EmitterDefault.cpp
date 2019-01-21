#include "ParticleSystem_EmitterDefault.h"
#include "../../../../Function/GlobalFunction.h"
#include "../../../Manager/CTimeManager.h"


CParticleEmitter_Default::CParticleEmitter_Default(const STParameters & a_rstParameters)
	:CParticleEmitter(a_rstParameters)
{
}

void CParticleEmitter_Default::update(void)
{
	this->updateParticles();
}

void CParticleEmitter_Default::createParticles(void)
{
	m_fSkipTime += GET_DELTA_TIME();

	while (m_fSkipTime >= 1.0f / m_stParameters.m_nNumParticlesPerSecond) {
		m_fSkipTime -= 1.0f / m_stParameters.m_nNumParticlesPerSecond;

		if (m_oParticleList.size() < m_stParameters.m_nMaxNumParticles) {
			STParticle stParticle;
			ZeroMemory(&stParticle, sizeof(stParticle));

			stParticle.m_fStartSize = RandomFloatValue(1.0f, 3.0f);
			stParticle.m_fEndSize = RandomFloatValue(1.0f, 3.0f);

			stParticle.m_fAngleVelocity = RandomFloatValue(50.0f, 120.0f);

			stParticle.m_stStartColor = D3DXCOLOR(RandomFloatValue(0.0f, 1.0f),
				RandomFloatValue(0.0f, 1.0f),
				RandomFloatValue(0.0f, 1.0f), 1.0f);

			stParticle.m_stEndColor = D3DXCOLOR(RandomFloatValue(0.0f, 1.0f),
				RandomFloatValue(0.0f, 1.0f),
				RandomFloatValue(0.0f, 1.0f), 0.0f);

				float fVelocity = RandomFloatValue(1.0f, 5.0f);

				D3DXVECTOR3 stDirection = {
					RandomFloatValue(-1.0f, 1.0f),
					RandomFloatValue(-1.0f, 1.0f),
					0.0f
				};

				D3DXVec3Normalize(&stDirection, &stDirection);
				stParticle.m_stPositionVelocity = stDirection * fVelocity;

				
			

			stParticle.m_fLifeTime = RandomFloatValue(0.1f, 2.0f);
			stParticle.m_fLeftLifeTime = stParticle.m_fLifeTime;

			stParticle.m_bIsActive = true;
			m_oParticleList.push_back(stParticle);
		}
	}
}

void CParticleEmitter_Default::updateParticles(void)
{
	for (auto &rstParticle : m_oParticleList) {
		rstParticle.m_fLeftLifeTime -= GET_DELTA_TIME();

		// 파티클의 활동 시간이 끝났을 경우
		if (rstParticle.m_fLeftLifeTime <= 0.0f) {
			rstParticle.m_bIsActive = false;
		}
		else {
			rstParticle.m_fAngle += rstParticle.m_fAngleVelocity * GET_DELTA_TIME();
			rstParticle.m_stPosition += rstParticle.m_stPositionVelocity * GET_DELTA_TIME();

			/*
			선형 보간 공식
			:
			- S (시작 값)
			- E (종료 값)
			- T (0 ~ 1 의 범위를 지니는 시간)

			Value = S + (E - S) * T
			*/
			float fPercent = rstParticle.m_fLeftLifeTime / rstParticle.m_fLifeTime;

			rstParticle.m_fSize = rstParticle.m_fStartSize +
				((rstParticle.m_fEndSize - rstParticle.m_fStartSize) * (1.0f - fPercent));

			rstParticle.m_stColor = rstParticle.m_stStartColor +
				((rstParticle.m_stEndColor - rstParticle.m_stStartColor) * (1.0f - fPercent));
		}
	}
}
