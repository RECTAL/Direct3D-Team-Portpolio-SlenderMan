#include "CSpriteObject_Container.h"
#include "CSpriteObject_Button.h"
#include "CSpriteObject_List.h"
#include "CSpriteObject_ScrollBar.h"

#include "../../../System/WindowSystem/WindowSystem_CWindowContainer.h"
#include "../../../Manager/CInputManager.h"

CSpriteObject_Container::CSpriteObject_Container(std::string a_stSpriteTexturePath, std::string a_stExtantion, const float width, const float height, int a_nAnimationFrame)
	:CSpriteObject(a_stSpriteTexturePath,a_stExtantion, width, height, a_nAnimationFrame)
{
	m_ECWindowType = CWindowType::CONTAINER;
}

CSpriteObject_Container::~CSpriteObject_Container()
{
	this->release();
}

void CSpriteObject_Container::update()
{
	if (m_bIsVisible)
	{
		CSpriteObject::update();
	
		m_pWindow->getActiveSize() = SIZE{ (LONG)m_fWidth,(LONG)m_fHeight };
		m_pWindow->update();

		setPosition(m_pWindow->getAbsolutePosition());

		for (auto oIterator : m_oChildSpriteObjectList)
		{
			if (oIterator.second->getWindowType() == CWindowType::BUTTON)
			{
				auto SpriteObj = dynamic_cast<CSpriteObject_Button*>(oIterator.second);
				SpriteObj->getWindow()->setAbsolutePosition(m_pWindow->getAbsolutePosition() + SpriteObj->getRelativePos());
			}
			if (oIterator.second->getWindowType() == CWindowType::CONTAINER)
			{
				auto SpriteObj = dynamic_cast<CSpriteObject_Container*>(oIterator.second);
				SpriteObj->getWindow()->setAbsolutePosition(m_pWindow->getAbsolutePosition() + SpriteObj->getRelativePos());
			}
			if (oIterator.second->getWindowType() == CWindowType::LIST)
			{
				auto SpriteObj = dynamic_cast<CSpriteObject_List*>(oIterator.second);
				SpriteObj->getWindow()->setAbsolutePosition(m_pWindow->getAbsolutePosition() + SpriteObj->getRelativePos());
			}
			if (oIterator.second->getWindowType() == CWindowType::SCROLLBAR)
			{
				auto SpriteObj = dynamic_cast<CSpriteObject_ScrollBar*>(oIterator.second);
				SpriteObj->getWindow()->setAbsolutePosition(m_pWindow->getAbsolutePosition() + SpriteObj->getRelativePos());
			}
		}
		if (PtInRect(&m_pWindow->getActiveRect(), GET_MOUSE_POSITION()))
		{

			if (IS_MOUSE_BUTTON_PRESSED(EMouseInput::LEFT))
			{
				m_pWindow->getBeginCallBackFunc()();
			}
			else if (IS_MOUSE_BUTTON_DOWN(EMouseInput::LEFT))
			{
				m_pWindow->getCallBackFunc()();
			}
			if (IS_MOUSE_BUTTON_RELEASED(EMouseInput::LEFT))
			{
				m_pWindow->getEndCallBackFunc()();
			}
		}
		for (auto oIterator : m_oChildSpriteObjectList)
		{
			oIterator.second->update();
		}
	}
}

void CSpriteObject_Container::doDrawUI()
{
	CSpriteObject::doDrawUI();
	for (auto oIterator : m_oChildSpriteObjectList)
	{
		oIterator.second->doDrawUI();
	}
}

void CSpriteObject_Container::setVisible(bool a_bIsVisible)
{
	m_bIsVisible = a_bIsVisible;
	for (auto oIterator : m_oChildSpriteObjectList)
	{
		oIterator.second->setVisible(a_bIsVisible);
	}
}

void CSpriteObject_Container::init(std::function<void(void)>* a_pCrashCallBackFunc, std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc, bool a_bIsChild, D3DXVECTOR3 a_rstRelativePos)
{
	D3DSURFACE_DESC pSurf;
	LPDIRECT3DTEXTURE9 pTex = m_oSpriteTexture[m_nTextureOffset];
	pTex->GetLevelDesc(0, &pSurf);
	m_pWindow = new CWindowContainer("WindowContainer", CWindowType::CONTAINER, SIZE{ (LONG)pSurf.Width,(LONG)pSurf.Height }, getPosition());
	m_pWindow->init(a_pCrashCallBackFunc, a_pBeginCallBackFunc, a_pCallBackFunc, a_pEndCallBackFunc);
	m_bIsChild = a_bIsChild;
	m_stRelativePos = a_rstRelativePos;
}

void CSpriteObject_Container::release()
{
	SAFE_DELETE(m_pWindow);
}

void CSpriteObject_Container::addChildSpriteObject(std::string a_stSpriteName,CWindowType a_ECWindowType ,CSpriteObject * a_pSpriteObject)
{
	auto oIter = std::find_if(m_oChildSpriteObjectList.begin(), m_oChildSpriteObjectList.end(), [=](std::pair<std::string, CSpriteObject*> obj)->bool {
		if (obj.first == a_stSpriteName)
			return true;
		else return false;
	});

	if (oIter == m_oChildSpriteObjectList.end())
	{
		if (a_ECWindowType == CWindowType::BUTTON)
		{
			auto pSpriteObject = dynamic_cast<CSpriteObject_Button*>(a_pSpriteObject);
			m_oChildSpriteObjectList.push_back(decltype(m_oChildSpriteObjectList)::value_type(a_stSpriteName, a_pSpriteObject));
		}
		else if (a_ECWindowType == CWindowType::CONTAINER)
		{
			auto pSpriteObject = dynamic_cast<CSpriteObject_Container*>(a_pSpriteObject);
			m_oChildSpriteObjectList.push_back(decltype(m_oChildSpriteObjectList)::value_type(a_stSpriteName, a_pSpriteObject));
		}
		else if (a_ECWindowType == CWindowType::LIST)
		{
			auto pSpriteObject = dynamic_cast<CSpriteObject_List*>(a_pSpriteObject);
			m_oChildSpriteObjectList.push_back(decltype(m_oChildSpriteObjectList)::value_type(a_stSpriteName, a_pSpriteObject));
		}
		else if (a_ECWindowType == CWindowType::SCROLLBAR)
		{
			auto pSpriteObject = dynamic_cast<CSpriteObject_ScrollBar*>(a_pSpriteObject);
			m_oChildSpriteObjectList.push_back(decltype(m_oChildSpriteObjectList)::value_type(a_stSpriteName, a_pSpriteObject));
		}
	}
}

void CSpriteObject_Container::deleteChildSpriteObject(std::string a_stSpriteName)
{

	auto oIter = std::find_if(m_oChildSpriteObjectList.begin(), m_oChildSpriteObjectList.end(), [=](std::pair<std::string, CSpriteObject*> obj)->bool {
		if (obj.first == a_stSpriteName)
			return true;
		else return false;
	});

	if (oIter != m_oChildSpriteObjectList.end())
	{
		m_oChildSpriteObjectList.erase(oIter);
	}

}
