#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../System/ParticleSystem/ParticleSystem_CParticleSystem.h"

//! ����Ʈ ��ƼŬ
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
	virtual void preDraw(void) override;							//! ��ü�� �׸��� ��
	virtual void doDraw(void) override;								//! ��ü�� �׸���
	virtual void postDraw(void) override;							//! ��ü�� �׸� ��

private:			// private function : Utility

	static LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(const CParticleSystem::STParameters &a_rstParameters);	//! ���� ���۸� �����Ѵ�
	CParticleEmitter*			   createParticleEmitter(EParticleType a_EParticleType);
};
