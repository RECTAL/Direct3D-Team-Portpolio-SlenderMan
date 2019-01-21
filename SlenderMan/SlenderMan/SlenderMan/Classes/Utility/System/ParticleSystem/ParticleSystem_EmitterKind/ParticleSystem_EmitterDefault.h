#pragma once
#include "../ParticleSystem_CParticleEmitter.h"


class CParticleEmitter_Default :public CParticleEmitter
{
public:
	CParticleEmitter_Default(const STParameters &a_rstParameters);

public:			// interface
	virtual void update(void) override;		//interface : IUpdateable

protected:		// protected override function : Utility
/******************************************************************/
// protected override function : about create update remove 
/******************************************************************/
	virtual void createParticles(void)override;												//! ��ƼŬ�� �����Ѵ�
	virtual void updateParticles(void)override;												//! ��ƼŬ�� �����Ѵ�
};