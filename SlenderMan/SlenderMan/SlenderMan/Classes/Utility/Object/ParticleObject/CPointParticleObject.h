#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../System/ParticleSystem/ParticleSystem_CParticleSystem.h"

//! 포인트 파티클
class CParticleEmitter;
class CPointParticleObject : public CParticleSystem
{
public:
	struct STVertex
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE
		};

		D3DXVECTOR3 m_stPosition;
		float m_fSize;
		D3DCOLOR m_nDiffuse;
	};

public:			// constructor , destructor
	CPointParticleObject(const CParticleSystem::STParameters &a_rstParameters);

protected:			// protected override function : Utility
	/***************************************************/
	// protected override function : about draw
	/***************************************************/
	virtual void preDraw(void) override;							//! 물체를 그리기 전
	virtual void doDraw(void) override;								//! 물체를 그린다
	virtual void postDraw(void) override;							//! 물체를 그린 후

private:			// private function : Utility

	static LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(const CParticleSystem::STParameters &a_rstParameters);	//! 정점 버퍼를 생성한다
	CParticleEmitter*			   createParticleEmitter(EParticleType a_EParticleType);
};
