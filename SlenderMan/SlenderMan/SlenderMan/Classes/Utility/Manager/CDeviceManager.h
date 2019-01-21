#pragma once

#include "../../Define/KGlobalDefine.h"

//! DeviceManager
class CDeviceManager
{
private:			// destructor

	//! destructor
	virtual ~CDeviceManager(void);



public:			// getter,setter
	/************** getter ****************/
	//! getDirect3D
	LPDIRECT3D9 getDirect3D(void) const;
	//! getDevice
	LPDIRECT3DDEVICE9 getDevice(void) const;




	/************** setter ****************/



public:			

	//! singleton Declare
	DECLARE_SINGLETON(CDeviceManager);

	//! initialization
	void init(void);

private:			

	//! create Direct3D
	LPDIRECT3D9 createDirect3D(void);

	//! create Device
	LPDIRECT3DDEVICE9 createDevice(void);


private:		

	LPDIRECT3D9 m_pDirect3D = nullptr;
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
};
