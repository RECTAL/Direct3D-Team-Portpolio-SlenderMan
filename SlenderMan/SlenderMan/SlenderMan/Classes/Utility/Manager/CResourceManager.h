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
	STStaticMesh			getStaticMesh(const std::string &a_rKey, bool a_bIsAutoCreate = true);			//! ���� �޽ø� ��ȯ�Ѵ�
	STWaveSound				getWaveSound(const std::string &a_rKey,bool a_bIsAutoCreate = true);			//! ���̺� ���带 ��ȯ�Ѵ�
	LPD3DXEFFECT			getEffect(const std::string &a_rKey,bool a_bIsAutoCreate = true);				//! ����Ʈ�� ��ȯ�Ѵ�
	LPDIRECT3DTEXTURE9		getTexture(const std::string &a_rKey,int width,int height,bool a_bIsAutoCreate = true);				//! �ؽ�ó�� ��ȯ�Ѵ�
	LPDIRECT3DTEXTURE9		getSpriteTexture(const std::string &a_rKey, const float width, const float height, bool a_bIsAutoCreate = true);		//! ��������Ʈ �ؽ�ó�� ��ȯ�Ѵ�
	LPDIRECT3DCUBETEXTURE9	getCubeTexture(const std::string &a_rKey,bool a_bIsAutoCreate = true);			//! ť�� �ؽ�ó�� ��ȯ�Ѵ�

public:			// public function: Utility
	/*******************************************************/
	// public function : addFunction
	/*******************************************************/
	void addStaticMesh(const std::string &a_rKey,STStaticMesh &a_rstStaticMesh);				//! ���� �޽ø� �߰��Ѵ�
	void addWaveSound(const std::string &a_rKey,STWaveSound &a_rstWaveSound);					//! ���̺� ���带 �߰��Ѵ�
	void addEffect(const std::string &a_rKey,LPD3DXEFFECT a_pEffect);							//! ����Ʈ�� �߰��Ѵ�
	void addTexture(const std::string &a_rKey, LPDIRECT3DTEXTURE9 a_pTexture, float width, float height);					//! �ؽ�ó�� �߰��Ѵ�
	void addCubeTexture(const std::string &a_rKey,LPDIRECT3DCUBETEXTURE9 a_pCubeTexture);		//! ť�� �ؽ�ó�� �߰��Ѵ�;

private:		// private function utility
	/*******************************************************/
	// public function : createFunction
	/*******************************************************/
	STStaticMesh createStaticMesh(const std::string &a_rFilepath);						//! ���� �޽ø� �����Ѵ�
	STWaveSound createWaveSound(const std::string &a_rFilepath);						//! ���̺� ���带 �����Ѵ�
	LPD3DXEFFECT createEffect(const std::string &a_rFilepath);							//! ����Ʈ�� �����Ѵ�
	LPDIRECT3DTEXTURE9 createTexture(const std::string &a_rFilepath);					//! �ؽ�ó�� �����Ѵ�
	LPDIRECT3DTEXTURE9 createSpriteTexture(const std::string &a_rFilepath,const float width, const float height);				//! ��������Ʈ �ؽ�ó�� �����Ѵ�
	LPDIRECT3DCUBETEXTURE9 createCubeTexture(const std::string &a_rFilepath);			//! ť�� �ؽ�ó�� �����Ѵ�

private:			// private value

	std::unordered_map<std::string, STStaticMesh> m_oStaticMeshList;
	std::unordered_map<std::string, STWaveSound> m_oWaveSoundList;

	std::unordered_map<std::string, LPD3DXEFFECT> m_oEffectList;
	std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_oTextureList;
	std::unordered_map<std::string, LPDIRECT3DCUBETEXTURE9> m_oCubeTextureList;
};
