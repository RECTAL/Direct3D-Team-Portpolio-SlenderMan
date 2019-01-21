#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CRenderObject.h"
#include "ParticleSystem_CParticleEmitter.h"

//! Particle System
class CParticleSystem : public CRenderObject
{
public:
	struct STParameters
	{
		std::string m_oTextureFilepath;
		CParticleEmitter::STParameters m_stEmitterParameters;
	};
	
protected:			// constructor, destructor

	CParticleSystem(const STParameters &a_rstParameters,LPDIRECT3DVERTEXBUFFER9 a_pVertexBuffer);
	virtual ~CParticleSystem(void);


public:				//interface
	virtual void update(void) override;				//interface : IUpdateable

protected:		// protected override function

	virtual void preDraw(void) override;			//! ��ü�� �׸��� ��
	virtual void postDraw(void) override;			//! ��ü�� �׸� ��


public:			// getter , setter
	/*****************************************************/
	//setter
	/*****************************************************/


public:			// public function :Utility
	/*****************************************************/
	//public function : create decreate particle
	/*****************************************************/
	void startParticleEmit(float a_fActiveTime = 0.0f);			//! ��ƼŬ ������ �����Ѵ�
	void stopParticleEmit(void);								//! ��ƼŬ ������ �����Ѵ�

protected:		

	bool m_bIsEmitEnable = false;
	float m_fActiveTime = 0.0f;

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer = nullptr;
	LPDIRECT3DTEXTURE9 m_pTexture = nullptr;

	STParameters m_stParameters;
	CParticleEmitter *m_pParticleEmitter = nullptr;
};
