#include "CDeviceManager.h"
#include "CWindowManager.h"

CDeviceManager::~CDeviceManager(void)
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pDirect3D);
}

void CDeviceManager::init(void)
{
	m_pDirect3D = this->createDirect3D();
	m_pDevice = this->createDevice();
}

LPDIRECT3D9 CDeviceManager::getDirect3D(void) const
{
	return m_pDirect3D;
}

LPDIRECT3DDEVICE9 CDeviceManager::getDevice(void) const
{
	return m_pDevice;
}

LPDIRECT3D9 CDeviceManager::createDirect3D(void)
{
	return Direct3DCreate9(D3D_SDK_VERSION);
}

LPDIRECT3DDEVICE9 CDeviceManager::createDevice(void)
{
	D3DCAPS9 stDeviceCaps;
	m_pDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &stDeviceCaps);

	DWORD nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	if (stDeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
;
	D3DPRESENT_PARAMETERS stParameters;
	ZeroMemory(&stParameters, sizeof(stParameters));

	stParameters.BackBufferWidth = GET_WINDOW_SIZE().cx;
	stParameters.BackBufferHeight = GET_WINDOW_SIZE().cy;
	stParameters.BackBufferCount = 1;
	stParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	stParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	stParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	stParameters.MultiSampleQuality = 0;

	stParameters.EnableAutoDepthStencil = true;
	stParameters.AutoDepthStencilFormat = D3DFMT_D24S8;

	stParameters.Flags = 0;
	stParameters.Windowed = true;
	stParameters.hDeviceWindow = GET_WINDOW_HANDLE();

	stParameters.FullScreen_RefreshRateInHz = D3DPRESENT_INTERVAL_DEFAULT;
	stParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		stParameters.hDeviceWindow,
		nVertexProcessing| D3DCREATE_MULTITHREADED,
		&stParameters,
		&pDevice);

	return pDevice;
}
