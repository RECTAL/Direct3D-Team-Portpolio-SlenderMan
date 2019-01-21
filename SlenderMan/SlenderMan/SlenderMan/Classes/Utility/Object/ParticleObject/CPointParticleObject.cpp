#include "CPointParticleObject.h"
#include "../../../Function/GlobalFunction.h"
#include "../../Manager/CDeviceManager.h"


#include "../../System/ParticleSystem/ParticleSystem_EmitterKind/ParticleSystem_EmitterDefault.h"

CPointParticleObject::CPointParticleObject(const CParticleSystem::STParameters & a_rstParameters)
	:CParticleSystem(a_rstParameters,CPointParticleObject::createVertexBuffer(a_rstParameters))
{
	m_pParticleEmitter = this->createParticleEmitter(a_rstParameters.m_stEmitterParameters.m_eParticleType);
}

void CPointParticleObject::preDraw(void)
{
	CParticleSystem::preDraw();

	GET_DEVICE()->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
	GET_DEVICE()->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);

	GET_DEVICE()->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDWORD(0.0f));
	GET_DEVICE()->SetRenderState(D3DRS_POINTSIZE_MAX, FloatToDWORD(1000.0f));

	GET_DEVICE()->SetRenderState(D3DRS_POINTSCALE_A, FloatToDWORD(0.0f));
	GET_DEVICE()->SetRenderState(D3DRS_POINTSCALE_B, FloatToDWORD(0.0f));
	GET_DEVICE()->SetRenderState(D3DRS_POINTSCALE_C, FloatToDWORD(1.0f));
}

void CPointParticleObject::doDraw(void)
{
	CParticleSystem::doDraw();

	auto stWorldMatrix = this->getFinalWorldMatrix();
	GET_DEVICE()->SetTransform(D3DTS_WORLD, &stWorldMatrix);

	STVertex *pstVertices = nullptr;
	auto &rParticleList = m_pParticleEmitter->getParticleList();

	if (SUCCEEDED(m_pVertexBuffer->Lock(0, 0, (void **)&pstVertices, 0))) {
		for (int i = 0; i < rParticleList.size(); ++i) {
			pstVertices[i].m_stPosition = rParticleList[i].m_stPosition;
			pstVertices[i].m_fSize = rParticleList[i].m_fSize;
			pstVertices[i].m_nDiffuse = rParticleList[i].m_stColor;
		}

		m_pVertexBuffer->Unlock();
	}

	GET_DEVICE()->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(STVertex));
	GET_DEVICE()->SetFVF(STVertex::FVF);
	GET_DEVICE()->SetTexture(0, m_pTexture);

	GET_DEVICE()->DrawPrimitive(D3DPT_POINTLIST, 0, rParticleList.size());

}

void CPointParticleObject::postDraw(void)
{
	CParticleSystem::postDraw();

	GET_DEVICE()->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	GET_DEVICE()->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
}

LPDIRECT3DVERTEXBUFFER9 CPointParticleObject::createVertexBuffer(const CParticleSystem::STParameters & a_rstParameters)
{
	return CreateVertexBuffer(sizeof(STVertex) * a_rstParameters.m_stEmitterParameters.m_nMaxNumParticles,
		D3DUSAGE_WRITEONLY,
		STVertex::FVF);
}

CParticleEmitter * CPointParticleObject::createParticleEmitter(EParticleType a_EParticleType)
{
	switch (a_EParticleType)
	{
	case EParticleType::NONE:
		return new CParticleEmitter_Default(m_stParameters.m_stEmitterParameters);
	}
}
