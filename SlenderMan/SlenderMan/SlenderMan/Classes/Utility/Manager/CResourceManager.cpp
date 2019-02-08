#include "CResourceManager.h"
#include "CDeviceManager.h"

CResourceManager::~CResourceManager(void)
{
	for (auto &rValueType : m_oStaticMeshList) {
		SAFE_RELEASE(rValueType.second.m_pStaticMesh);
	}

	for (auto &rValueType : m_oWaveSoundList) {
		SAFE_FREE(rValueType.second.m_pnBytes);
	}

	for (auto &rValueType : m_oEffectList) {
		SAFE_RELEASE(rValueType.second);
	}

	for (auto &rValueType : m_oTextureList) {
		SAFE_RELEASE(rValueType.second);
	}

	for (auto &rValueType : m_oCubeTextureList) {
		SAFE_RELEASE(rValueType.second);
	}
}

STStaticMesh CResourceManager::getStaticMesh(const std::string & a_rKey, bool a_bIsAutoCreate)
{
	auto oIterator = m_oStaticMeshList.find(a_rKey);

	// 보관 중인 정적 메시가 없을 경우
	if (oIterator == m_oStaticMeshList.end()) {
		STStaticMesh stStaticMesh;

		if (a_bIsAutoCreate) {
			stStaticMesh = this->createStaticMesh(a_rKey);
			this->addStaticMesh(a_rKey, stStaticMesh);
		}

		return stStaticMesh;
	}

	return oIterator->second;
}

STWaveSound CResourceManager::getWaveSound(const std::string & a_rKey, bool a_bIsAutoCreate)
{
	auto oIterator = m_oWaveSoundList.find(a_rKey);

	// 보관 중인 웨이브 사운드가 없을 경우
	if (oIterator == m_oWaveSoundList.end()) {
		STWaveSound stWaveSound;

		if (a_bIsAutoCreate) {
			stWaveSound = this->createWaveSound(a_rKey);
			this->addWaveSound(a_rKey, stWaveSound);
		}

		return stWaveSound;
	}

	return oIterator->second;
}

LPD3DXEFFECT CResourceManager::getEffect(const std::string & a_rKey, bool a_bIsAutoCreate)
{
	auto oIterator = m_oEffectList.find(a_rKey);

	// 보관 중인 이펙트가 없을 경우
	if (oIterator == m_oEffectList.end()) {
		LPD3DXEFFECT pEffect = nullptr;

		if (a_bIsAutoCreate) {
			pEffect = this->createEffect(a_rKey);
			this->addEffect(a_rKey, pEffect);
		}

		return pEffect;
	}

	return oIterator->second;
}

LPDIRECT3DTEXTURE9 CResourceManager::getTexture(const std::string & a_rKey, int width, int height, bool a_bIsAutoCreate)
{

	char path[MAX_PATH];
	sprintf(path, "%s%d%d", a_rKey.c_str(), width, height);
	auto oIterator = m_oTextureList.find(path);

	// 보관 중인 텍스처가 없을 경우
	if (oIterator == m_oTextureList.end()) {
		LPDIRECT3DTEXTURE9 pTexture = nullptr;

		if (a_bIsAutoCreate) {
			pTexture = this->createTexture(a_rKey);


			this->addTexture(a_rKey, pTexture, width, height);
			int a = 0;
		}

		return pTexture;
	}

	return oIterator->second;
}

LPDIRECT3DTEXTURE9 CResourceManager::getSpriteTexture(const std::string & a_rKey, const float width, const float height, bool a_bIsAutoCreate)
{
	auto oIterator = m_oTextureList.find(a_rKey);

	// 보관 중인 텍스처가 없을 경우
	if (oIterator == m_oTextureList.end()) {
		LPDIRECT3DTEXTURE9 pTexture = nullptr;

		if (a_bIsAutoCreate) {
			pTexture = this->createSpriteTexture(a_rKey, width, height);
			this->addTexture(a_rKey, pTexture,width,height);
		}

		return pTexture;
	}

	return oIterator->second;
}

LPDIRECT3DCUBETEXTURE9 CResourceManager::getCubeTexture(const std::string & a_rKey, bool a_bIsAutoCreate)
{
	auto oIterator = m_oCubeTextureList.find(a_rKey);

	// 보관 중인 텍스처가 없을 경우
	if (oIterator == m_oCubeTextureList.end()) {
		LPDIRECT3DCUBETEXTURE9 pCubeTexture = nullptr;

		if (a_bIsAutoCreate) {
			pCubeTexture = this->createCubeTexture(a_rKey);
			this->addCubeTexture(a_rKey, pCubeTexture);
		}

		return pCubeTexture;
	}

	return oIterator->second;
}

void CResourceManager::addStaticMesh(const std::string & a_rKey, STStaticMesh & a_rstStaticMesh)
{
	if (m_oStaticMeshList.find(a_rKey) == m_oStaticMeshList.end()) {
		m_oStaticMeshList.insert(decltype(m_oStaticMeshList)::value_type(a_rKey, a_rstStaticMesh));
	}
}

void CResourceManager::addWaveSound(const std::string & a_rKey, STWaveSound & a_rstWaveSound)
{
	if (m_oWaveSoundList.find(a_rKey) == m_oWaveSoundList.end()) {
		m_oWaveSoundList.insert(decltype(m_oWaveSoundList)::value_type(a_rKey, a_rstWaveSound));
	}
}

void CResourceManager::addEffect(const std::string & a_rKey, LPD3DXEFFECT a_pEffect)
{
	if (m_oEffectList.find(a_rKey) == m_oEffectList.end()) {
		m_oEffectList.insert(decltype(m_oEffectList)::value_type(a_rKey, a_pEffect));
	}
}

void CResourceManager::addTexture(const std::string & a_rKey, LPDIRECT3DTEXTURE9 a_pTexture, float width, float height)
{
	char path[MAX_PATH];
	sprintf(path, "%s%d%d", a_rKey.c_str(), width, height);

	if (m_oTextureList.find(a_rKey) == m_oTextureList.end()) {
		m_oTextureList.insert(decltype(m_oTextureList)::value_type(path, a_pTexture));
	}
}

void CResourceManager::addCubeTexture(const std::string & a_rKey, LPDIRECT3DCUBETEXTURE9 a_pCubeTexture)
{
	if (m_oCubeTextureList.find(a_rKey) == m_oCubeTextureList.end()) {
		m_oCubeTextureList.insert(decltype(m_oCubeTextureList)::value_type(a_rKey, a_pCubeTexture));
	}
}

STStaticMesh CResourceManager::createStaticMesh(const std::string & a_rFilepath)
{
	STStaticMesh stStaticMesh;

	LPD3DXBUFFER pAdjacency = nullptr;
	LPD3DXBUFFER pXMaterials = nullptr;


	D3DXLoadMeshFromXA(a_rFilepath.c_str(),
		D3DXMESH_MANAGED,
		GET_DEVICE(),
		&pAdjacency,							
		&pXMaterials,							
		NULL,
		&stStaticMesh.m_nNumMaterials,			
		&stStaticMesh.m_pStaticMesh);
	
	for (int i = 0; i < stStaticMesh.m_nNumMaterials; ++i) {
		auto pstXMaterials = (LPD3DXMATERIAL)pXMaterials->GetBufferPointer();
		LPDIRECT3DTEXTURE9 pTexture = nullptr;

		if (pstXMaterials[i].pTextureFilename != nullptr) {
			std::string oBasepath = "";

			if (a_rFilepath.rfind("/") != std::string::npos) {
				oBasepath = a_rFilepath.substr(0, a_rFilepath.rfind("/") + 1);
			}
			char szFilepath[MAX_PATH] = "";

			sprintf(szFilepath, "%s%s", oBasepath.c_str(),
				pstXMaterials[i].pTextureFilename);

			pTexture = GET_TEXTURE(szFilepath,GOUST_VALUE,GOUST_VALUE);
		}

		stStaticMesh.m_oTextureList.push_back(pTexture);
		stStaticMesh.m_oMaterialList.push_back(pstXMaterials[i].MatD3D);
	}

	D3DVERTEXELEMENT9 astElements[] = {
		0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0,
		0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0,
		0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0,
		0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0,
		0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0,
		D3DDECL_END()
	};

	auto pOriginMesh = stStaticMesh.m_pStaticMesh;

	pOriginMesh->CloneMesh(pOriginMesh->GetOptions(),
		astElements,
		GET_DEVICE(),
		&stStaticMesh.m_pStaticMesh);

	stStaticMesh.m_pStaticMesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE,
		(DWORD *)pAdjacency->GetBufferPointer(),
		(DWORD *)pAdjacency->GetBufferPointer(),
		NULL,
		NULL);

	D3DXComputeNormals(stStaticMesh.m_pStaticMesh, (DWORD *)pAdjacency->GetBufferPointer());

	D3DXComputeTangent(stStaticMesh.m_pStaticMesh, 0, 0, 0, TRUE, (DWORD *)pAdjacency->GetBufferPointer());

	SAFE_RELEASE(pAdjacency);
	SAFE_RELEASE(pXMaterials);
	SAFE_RELEASE(pOriginMesh);

	return stStaticMesh;
}

STWaveSound CResourceManager::createWaveSound(const std::string & a_rFilepath)
{
	STWaveSound stWaveSound;
	ZeroMemory(&stWaveSound, sizeof(stWaveSound));

	HMMIO hWaveFile = mmioOpenA((char *)a_rFilepath.c_str(), NULL, MMIO_READ);

	if (hWaveFile != NULL) {
		MMCKINFO stChunkInfo;
		ZeroMemory(&stChunkInfo, sizeof(stChunkInfo));

		stChunkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hWaveFile, &stChunkInfo, NULL, MMIO_FINDRIFF);

		MMCKINFO stSubChunkInfo;
		ZeroMemory(&stSubChunkInfo, sizeof(stSubChunkInfo));

		stSubChunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hWaveFile, &stSubChunkInfo, &stChunkInfo, MMIO_FINDCHUNK);

		mmioRead(hWaveFile,
			(char *)(&stWaveSound.m_stWaveFormat),
			sizeof(stWaveSound.m_stWaveFormat));

		mmioAscend(hWaveFile, &stSubChunkInfo, 0);

		stSubChunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hWaveFile, &stSubChunkInfo, &stChunkInfo, MMIO_FINDCHUNK);

		stWaveSound.m_nNumBytes = stSubChunkInfo.cksize;
		stWaveSound.m_pnBytes = (BYTE *)malloc(sizeof(BYTE) * stSubChunkInfo.cksize);

		mmioRead(hWaveFile,
			(char *)stWaveSound.m_pnBytes,
			stSubChunkInfo.cksize);

		mmioClose(hWaveFile, 0);
	}
	return stWaveSound;
}

LPD3DXEFFECT CResourceManager::createEffect(const std::string & a_rFilepath)
{
	LPD3DXEFFECT pEffect = nullptr;
	LPD3DXBUFFER pErrorBuffer = nullptr;

	// 이펙트를 생성한다
	D3DXCreateEffectFromFileA(GET_DEVICE(),
		a_rFilepath.c_str(),
		NULL,
		NULL,
		D3DXSHADER_DEBUG,
		NULL,
		&pEffect,
		&pErrorBuffer);

	if (pErrorBuffer != nullptr) {
		char *pszErrorMessage = (char *)pErrorBuffer->GetBufferPointer();
		printf("CResourceManager.createEffect: %s\n", pszErrorMessage);

		SAFE_RELEASE(pErrorBuffer);
	}

	return pEffect;
}

LPDIRECT3DTEXTURE9 CResourceManager::createTexture(const std::string & a_rFilepath)
{
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	D3DXCreateTextureFromFileA(GET_DEVICE(),
		a_rFilepath.c_str(),
		&pTexture);

	return pTexture;
}

LPDIRECT3DTEXTURE9 CResourceManager::createSpriteTexture(const std::string & a_rFilepath, const float width, const float height)
{
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	D3DXCreateTextureFromFileExA(GET_DEVICE(),
		a_rFilepath.c_str(),
		width,
		height,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0),
		NULL,
		NULL,
		&pTexture);

	return pTexture;
}

LPDIRECT3DCUBETEXTURE9 CResourceManager::createCubeTexture(const std::string & a_rFilepath)
{
	LPDIRECT3DCUBETEXTURE9 pCubeTexture = nullptr;

	// 큐브 텍스처를 생성한다
	D3DXCreateCubeTextureFromFileA(GET_DEVICE(),
		a_rFilepath.c_str(),
		&pCubeTexture);

	return pCubeTexture;
}
