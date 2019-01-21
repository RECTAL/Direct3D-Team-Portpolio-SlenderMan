#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Interface/IUpdateable.h"

//! ParticleEmitter
class CParticleEmitter : public IUpdateable
{
public:
	struct STParticle
	{
		bool m_bIsActive;

		float m_fLifeTime;
		float m_fLeftLifeTime;

		float m_fSize;
		float m_fStartSize;
		float m_fEndSize;

		float m_fAngle;
		float m_fAngleVelocity;

		D3DXCOLOR m_stColor;
		D3DXCOLOR m_stStartColor;
		D3DXCOLOR m_stEndColor;

		D3DXVECTOR3 m_stPosition;
		D3DXVECTOR3 m_stPositionVelocity;
	};

	struct STParameters
	{
		int m_nMaxNumParticles;
		int m_nNumParticlesPerSecond;

		EParticleType m_eParticleType;
	};

public:			// constructor
	CParticleEmitter(const STParameters &a_rstParameters);


public:			// interface
	virtual void update(void) override;		//interface : IUpdateable

public:			// getter, setter
	/******************************************************************/
	//getter
	/******************************************************************/
	std::vector<STParticle> & getParticleList(void);						//! ��ƼŬ ����Ʈ�� ��ȯ�Ѵ�
	

	/******************************************************************/
	//setter
	/******************************************************************/
	void setEmitEnable(bool a_bIsEmitEnable);								//! ��ƼŬ ���� ���θ� �����Ѵ�
	

protected:		// protected override function : Utility
	/******************************************************************/
	// protected override function : about create update remove 
	/******************************************************************/
	virtual void createParticles(void) =0;												//! ��ƼŬ�� �����Ѵ�
	virtual void updateParticles(void) =0;												//! ��ƼŬ�� �����Ѵ�
	virtual void removeParticles(void);													//! ��ƼŬ�� �����Ѵ�

protected:			

	bool m_bIsEmitEnable = false;
	float m_fSkipTime = 0.0f;

	STParameters m_stParameters;
	std::vector<STParticle> m_oParticleList;
};
