#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CRenderObject.h"

//! StaticObject
class CCameraObject;
class CLightObject;
class CSpotLightObject;
class CDecorate_SoundObj: public CRenderObject
{
public:
	struct STParameters
	{
		CCameraObject*	m_pCamera;
		CLightObject*	m_pDirectional;
	};

public:			//constructor , destructor
	CDecorate_SoundObj(STParameters &a_rstParameters, EPlayingBGM a_eSound);
	~CDecorate_SoundObj();

protected:		//interface		
	
	virtual void preDraw(void)override;
	virtual void postDraw(void)override;
	virtual void doDraw(void) override;		//! interface : IRenderable

public:
	LPD3DXMESH		getMesh() { return m_pMesh; }
	STParameters&	getSTParameters() { return m_stParameters; }
	EPlayingBGM		getSoundType() { return m_eSoundType; }


	virtual void update(void) override;		//! interface : IUpdateable

private:
	void	createSphereMesh();
	void	createMatrial();

protected:
	D3DMATERIAL9	m_stMtrl;
	LPD3DXMESH		m_pMesh = nullptr;
	LPD3DXEFFECT	m_pEffect = nullptr;

	D3DXVECTOR4		m_stColor;
	STParameters	m_stParameters;
	EPlayingBGM		m_eSoundType;
private:


};
