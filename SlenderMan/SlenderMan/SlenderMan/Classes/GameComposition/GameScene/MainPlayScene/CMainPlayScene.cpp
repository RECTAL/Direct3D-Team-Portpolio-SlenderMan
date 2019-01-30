#include "CMainPlayScene.h"
#include "../../../Utility/Object/StaticObject/CStaticObject.h"

CMainPlayScene::CMainPlayScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CMainPlayScene::~CMainPlayScene()
{
	SAFE_DELETE(testMesh);
}

void CMainPlayScene::init()
{	
	CScene::init();

	testMesh = this->createStaticMesh();
}

void CMainPlayScene::createWindowUI()
{
}

CStaticObject * CMainPlayScene::createStaticMesh()
{
	CStaticObject::STParameters stParameters = {
		"Resources/Meshes/Elementalist/Elementalist.x",
		"Resources/Effects/Example_15.fx"
	};

	return new CStaticObject(stParameters);
}

void CMainPlayScene::update(void)
{
	CScene::update();
	testMesh->update();
}

void CMainPlayScene::draw(void)
{
	CScene::draw();
	testMesh->draw();
}

void CMainPlayScene::drawUI(void)
{
	CScene::drawUI();
}

LRESULT CMainPlayScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
