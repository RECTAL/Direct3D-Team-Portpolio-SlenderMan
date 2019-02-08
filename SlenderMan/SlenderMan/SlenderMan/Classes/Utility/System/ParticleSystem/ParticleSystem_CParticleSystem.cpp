#include "ParticleSystem_CParticleSystem.h"
#include "ParticleSystem_CParticleEmitter.h"
#include "../../Manager/CDeviceManager.h"
#include "../../Manager/CResourceManager.h"
#include "../../Manager/CTimeManager.h"

CParticleSystem::CParticleSystem(const STParameters & a_rstParameters,
	LPDIRECT3DVERTEXBUFFER9 a_pVertexBuffer)
	:
	m_stParameters(a_rstParameters),
	m_pVertexBuffer(a_pVertexBuffer)
{
	m_pTexture = GET_TEXTURE(a_rstParameters.m_oTextureFilepath,GOUST_VALUE,GOUST_VALUE);
}

CParticleSystem::~CParticleSystem(void)
{
	SAFE_DELETE(m_pParticleEmitter);
	SAFE_RELEASE(m_pVertexBuffer);
}


void CParticleSystem::update(void)
{
	CRenderObject::update();
	m_pParticleEmitter->update();

	if (m_bIsEmitEnable) {
		m_fActiveTime -= GET_DELTA_TIME();

		if (m_fActiveTime <= 0.0f) {
			this->stopParticleEmit();
		}
	}
}

void CParticleSystem::startParticleEmit(float a_fActiveTime)
{
	m_bIsEmitEnable = true;
	m_fActiveTime = a_fActiveTime;

	m_pParticleEmitter->setEmitEnable(true);
}

void CParticleSystem::stopParticleEmit(void)
{
	m_bIsEmitEnable = false;
	m_fActiveTime = 0.0f;

	m_pParticleEmitter->setEmitEnable(false);
}

void CParticleSystem::preDraw(void)
{
	CRenderObject::preDraw();

	GET_DEVICE()->SetRenderState(D3DRS_LIGHTING, FALSE);
	GET_DEVICE()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	GET_DEVICE()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GET_DEVICE()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_DEVICE()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	GET_DEVICE()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	GET_DEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG0, D3DTA_DIFFUSE);
	GET_DEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
}

void CParticleSystem::postDraw(void)
{
	CRenderObject::postDraw();
	GET_DEVICE()->SetTexture(0, NULL);

	GET_DEVICE()->SetRenderState(D3DRS_LIGHTING, TRUE);
	GET_DEVICE()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	GET_DEVICE()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}
