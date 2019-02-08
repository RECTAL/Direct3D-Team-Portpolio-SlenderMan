#pragma once

#include "../../Define/KGlobalDefine.h"

//! Resource Manager
class CResourceManager
{

private:			// constructor , destructor
	virtual ~CResourceManager(void);

public:
	//! singleton declare
	DECLARE_SINGLETON(CResourceManager);

public:			// getter
	/*******************************************************/
	//getter
	/*******************************************************/
	STStaticMesh			getStaticMesh(const std::string &a_rKey, bool a_bIsAutoCreate = true);			//! 정적 메시를 반환한다
	STWaveSound				getWaveSound(const std::string &a_rKey,bool a_bIsAutoCreate = true);			//! 웨이브 사운드를 반환한다
	LPD3DXEFFECT			getEffect(const std::string &a_rKey,bool a_bIsAutoCreate = true);				//! 이펙트를 반환한다
	LPDIRECT3DTEXTURE9		getTexture(const std::string &a_rKey,int width,int height,bool a_bIsAutoCreate = true);				//! 텍스처를 반환한다
	LPDIRECT3DTEXTURE9		getSpriteTexture(const std::string &a_rKey, const float width, const float height, bool a_bIsAutoCreate = true);		//! 스프라이트 텍스처를 반환한다
	LPDIRECT3DCUBETEXTURE9	getCubeTexture(const std::string &a_rKey,bool a_bIsAutoCreate = true);			//! 큐브 텍스처를 반환한다

public:			// public function: Utility
	/*******************************************************/
	// public function : addFunction
	/*******************************************************/
	void addStaticMesh(const std::string &a_rKey,STStaticMesh &a_rstStaticMesh);				//! 정적 메시를 추가한다
	void addWaveSound(const std::string &a_rKey,STWaveSound &a_rstWaveSound);					//! 웨이브 사운드를 추가한다
	void addEffect(const std::string &a_rKey,LPD3DXEFFECT a_pEffect);							//! 이펙트를 추가한다
	void addTexture(const std::string &a_rKey, LPDIRECT3DTEXTURE9 a_pTexture, float width, float height);					//! 텍스처를 추가한다
	void addCubeTexture(const std::string &a_rKey,LPDIRECT3DCUBETEXTURE9 a_pCubeTexture);		//! 큐브 텍스처를 추가한다;

private:		// private function utility
	/*******************************************************/
	// public function : createFunction
	/*******************************************************/
	STStaticMesh createStaticMesh(const std::string &a_rFilepath);						//! 정적 메시를 생성한다
	STWaveSound createWaveSound(const std::string &a_rFilepath);						//! 웨이브 사운드를 생성한다
	LPD3DXEFFECT createEffect(const std::string &a_rFilepath);							//! 이펙트를 생성한다
	LPDIRECT3DTEXTURE9 createTexture(const std::string &a_rFilepath);					//! 텍스처를 생성한다
	LPDIRECT3DTEXTURE9 createSpriteTexture(const std::string &a_rFilepath,const float width, const float height);				//! 스프라이트 텍스처를 생성한다
	LPDIRECT3DCUBETEXTURE9 createCubeTexture(const std::string &a_rFilepath);			//! 큐브 텍스처를 생성한다

private:			// private value

	std::unordered_map<std::string, STStaticMesh> m_oStaticMeshList;
	std::unordered_map<std::string, STWaveSound> m_oWaveSoundList;

	std::unordered_map<std::string, LPD3DXEFFECT> m_oEffectList;
	std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_oTextureList;
	std::unordered_map<std::string, LPDIRECT3DCUBETEXTURE9> m_oCubeTextureList;
};
