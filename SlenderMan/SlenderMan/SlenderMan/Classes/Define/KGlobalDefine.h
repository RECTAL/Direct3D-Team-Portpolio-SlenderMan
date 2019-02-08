#pragma once
/******************** libaray ***************************/
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")



/******************* window header **********************/
#include <Windows.h>


/********************* c++ header ***********************/
#include <iostream>
#include <memory>
#include <cassert>
#include <chrono>
#include <tchar.h>
#include <mmsystem.h>
#include <thread>
#include <mutex>

/****************** c++ stl header **********************/
#include <vector>
#include <string>
#include <array>
#include <random>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <numeric>




/****************** directx9 header *********************/
#include <d3dx9.h>
#include <dinput.h>
#include <dsound.h>




/****************** custom Define header *****************/
#include "KGlobalEnumDefine/KGlobalEnumDefine.h"
#include "KGlobalStructDefine/KGlobalStructDefine.h"



/*******************************************************************************************************/
//Define Macro
/*******************************************************************************************************/
/*Macro utility*/
#define WORLD_RIGHT_DIRECTION				(D3DXVECTOR3(1.0f, 0.0f, 0.0f))
#define WORLD_UP_DIRECTION					(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define WORLD_FORWARD_DIRECTION				(D3DXVECTOR3(0.0f, 0.0f, 1.0f))
#define MAX_OBJ_CAPACITY					10
#define GOUST_VALUE							-1


#define SAFE_FREE(TARGET)					if((TARGET) != nullptr) { free((TARGET)); (TARGET) = nullptr; }
#define SAFE_DELETE(TARGET)					if((TARGET) != nullptr) { delete (TARGET); (TARGET) = nullptr; }
#define SAFE_DELETE_ARRAY(TARGET)			if((TARGET) != nullptr) { delete[] (TARGET); (TARGET) = nullptr; }
#define SAFE_RELEASE(TARGET)				if((TARGET) != nullptr) { (TARGET)->Release(); (TARGET) = nullptr; }

#define	GAMESCENE_TITLE						"GameScene_Title"
#define GAMESCENE_LOADING					"GameScene_Loading"
#define GAMESCENE_MAINPLAY					"GameScene_MainPlay"
#define GAMESCENE_MAPTOOL					"GameScene_MapTool"
#define GAMESCENE_GAMEOVER					"GameScene_GameOver"
#define GAMESCENE_VICTORY					"GameScene_Victory"
#define GAMESCENE_VIRTUAL					"GameScene_Virtual"

/*Macro GetSingleton*/
//application
#define GET_WINDOW_APPLICATION()						(CWindowApplication::getInstance())
#define GET_DIRECT3D_APPLICATION()						((CDirect3DApplication *)GET_WINDOW_APPLICATION())

//manager
#define GET_WINDOW_MANAGER()							(CWindowManager::getInstance())
#define GET_DEVICE_MANAGER()							(CDeviceManager::getInstance())
#define GET_INPUT_MANAGER()								(CInputManager::getInstance())
#define GET_TIME_MANAGER()								(CTimeManager::getInstance())
#define GET_RESOURCE_MANAGER()							(CResourceManager::getInstance())
#define GET_SCENE_MANAGER()								(CSceneManager::getInstance())
#define GET_SOUND_MANAGER()								(CSoundManager::getInstance())
#define GET_RENDERTARGET_MANAGER()						(CRendertargetManager::getInstance())

//windowOption
#define GET_WINDOW_SIZE()								(GET_WINDOW_MANAGER()->getWindowSize())
#define GET_WINDOW_HANDLE()								(GET_WINDOW_MANAGER()->getWindowHandle())
#define GET_INSTANCE_HANDLE()							(GET_WINDOW_MANAGER()->getInstanceHandle())

//deviceOption
#define GET_DEVICE()									(GET_DEVICE_MANAGER()->getDevice())
#define GET_DIRECT3D()									(GET_DEVICE_MANAGER()->getDirect3D())



//inputOption
#define IS_KEY_DOWN(KEY_CODE)							(GET_INPUT_MANAGER()->isKeyDown((KEY_CODE)))
#define IS_KEY_PRESSED(KEY_CODE)						(GET_INPUT_MANAGER()->isKeyPressed((KEY_CODE)))
#define IS_KEY_RELEASED(KEY_CODE)						(GET_INPUT_MANAGER()->isKeyReleased((KEY_CODE)))
#define IS_MOUSE_BUTTON_DOWN(MOUSE_INPUT)				(GET_INPUT_MANAGER()->isMouseButtonDown((MOUSE_INPUT)))
#define IS_MOUSE_BUTTON_PRESSED(MOUSE_INPUT)			(GET_INPUT_MANAGER()->isMouseButtonPressed((MOUSE_INPUT)))
#define IS_MOUSE_BUTTON_RELEASED(MOUSE_INPUT)			(GET_INPUT_MANAGER()->isMouseButtonReleased((MOUSE_INPUT)))
#define GET_MOUSE_POSITION()							(GET_INPUT_MANAGER()->getMousePosition())


//time Option
#define GET_DELTA_TIME()								(GET_TIME_MANAGER()->getDeltaTime())
#define GET_RUNNING_TIME()								(GET_TIME_MANAGER()->getRunningTime())


//resource Option
#define GET_STATIC_MESH(FILE_PATH)						(GET_RESOURCE_MANAGER()->getStaticMesh((FILE_PATH)))
#define GET_WAVE_SOUND(FILE_PATH)						(GET_RESOURCE_MANAGER()->getWaveSound((FILE_PATH)))
#define GET_EFFECT(FILE_PATH)							(GET_RESOURCE_MANAGER()->getEffect((FILE_PATH)))
#define GET_TEXTURE(FILE_PATH,WIDTH,HEIGHT)				(GET_RESOURCE_MANAGER()->getTexture(FILE_PATH,WIDTH,HEIGHT))
#define GET_SPRITE_TEXTURE(FILE_PATH,WIDTH,HEIGHT)		(GET_RESOURCE_MANAGER()->getSpriteTexture(FILE_PATH,WIDTH,HEIGHT))
#define GET_CUBE_TEXTURE(FILE_PATH)						(GET_RESOURCE_MANAGER()->getCubeTexture((FILE_PATH)))


//Scene Option
#define CHANGE_SCENE_LOADING(NEXTSCENENAME,ISINIT)		(GET_SCENE_MANAGER()->changeScene_rootLoadingScene(NEXTSCENENAME,ISINIT))
#define CHANGE_SCENE_DIRECT(SCENENAME,ISINIT)			(GET_SCENE_MANAGER()->changeScene_rootDirect(SCENENAME,ISINIT))
#define FIND_SCENE(SCENE)								(GET_SCENE_MANAGER()->findScene(SCENE))


//Sound Option
#define GET_DIRECT_SOUND()								(GET_SOUND_MANAGER()->getDirectSound())
#define GET_PRIMARY_BUFFER()							(GET_SOUND_MANAGER()->getPrimaryBuffer())
#define PLAY_EFFECT_SOUND(FILE_PATH)					(GET_SOUND_MANAGER()->playEffectSound((FILE_PATH)))
#define PLAY_BACKGROUND_SOUND(FILE_PATH)				(GET_SOUND_MANAGER()->playBackgroundSound((FILE_PATH)))

//RenderTarget Option
#define FIND_RENDERTARGET(TARGETNAME)					(GET_RENDERTARGET_MANAGER()->findRenderTarget(TARGETNAME))
#define GET_ORIGIN_RENDERTARGET()						(GET_RENDERTARGET_MANAGER()->getOriginRenderTarget())

// singleton
#define DECLARE_SINGLETON(CLASS_NAME)			\
public:											\
static CLASS_NAME * getInstance(void) {			\
	static CLASS_NAME oInstance;				\
	return &oInstance;							\
}
