#include "CSpriteObject_List.h"
#include"CSpriteObject_Container.h"
#include "CSpriteObject_Button.h"
#include "../../../System/WindowSystem/WindowSystem_CWindowList.h"

CSpriteObject_List::CSpriteObject_List(std::string a_stSpriteTexturePath, std::string a_stExtantion, int a_nAnimationFrame)
	:CSpriteObject(a_stSpriteTexturePath,a_stExtantion,a_nAnimationFrame)
{
	m_ECWindowType = CWindowType::LIST;
}

CSpriteObject_List::~CSpriteObject_List()
{
	this->release();
}

void CSpriteObject_List::update()
{
	if (m_bIsVisible)
	{
		CSpriteObject::update();
		D3DSURFACE_DESC pSurf;
		LPDIRECT3DTEXTURE9 pTex = m_oSpriteTexture[m_nTextureOffset];
		pTex->GetLevelDesc(0, &pSurf);

		m_pWindow->getActiveSize() = SIZE{ (LONG)pSurf.Width,(LONG)pSurf.Height };
		m_pWindow->update();

		setPosition(m_pWindow->getAbsolutePosition());

		for (auto oIterator : m_oChildSpriteObjectList)
		{
			if (oIterator.second->getWindowType() == CWindowType::BUTTON)
			{
				auto stSprite = dynamic_cast<CSpriteObject_Button*>(oIterator.second);
				stSprite->getWindow()->setAbsolutePosition(m_pWindow->getAbsolutePosition() + stSprite->getRelativePos() + m_stMoveOffset);
				stSprite->setPosition(stSprite->getWindow()->getAbsolutePosition());
			}
			if (oIterator.second->getWindowType() == CWindowType::CONTAINER)
			{
				auto stSprite = dynamic_cast<CSpriteObject_Container*>(oIterator.second);
				stSprite->getWindow()->setAbsolutePosition(m_pWindow->getAbsolutePosition() + stSprite->getRelativePos() + m_stMoveOffset);
				stSprite->setPosition(stSprite->getWindow()->getAbsolutePosition());
			}
			if (oIterator.second->getWindowType() == CWindowType::LIST)
			{
				auto stSprite = dynamic_cast<CSpriteObject_List*>(oIterator.second);
				stSprite->getWindow()->setAbsolutePosition(m_pWindow->getAbsolutePosition() + stSprite->getRelativePos() + m_stMoveOffset);
				stSprite->setPosition(stSprite->getWindow()->getAbsolutePosition());
			}
		}

		for (auto oIterator : m_oChildSpriteObjectList)
		{
			//oIterator.second->update();
			if (oIterator.second->getWindowType() == CWindowType::BUTTON)
			{
				auto stSprite = dynamic_cast<CSpriteObject_Button*>(oIterator.second);
				float top = stSprite->getWindow()->getActiveRect().top;
				float bottom = stSprite->getWindow()->getActiveRect().bottom;
			
				if (top>=m_pWindow->getActiveRect().top && bottom<=m_pWindow->getActiveRect().bottom)
				{
					oIterator.second->update();
				}
			}
			if (oIterator.second->getWindowType() == CWindowType::CONTAINER)
			{
				auto stSprite = dynamic_cast<CSpriteObject_Container*>(oIterator.second);
				float top = stSprite->getWindow()->getActiveRect().top;
				float bottom = stSprite->getWindow()->getActiveRect().bottom;
			
				if (top >= m_pWindow->getActiveRect().top && bottom <= m_pWindow->getActiveRect().bottom)
				{
					oIterator.second->update();
				}
			}
			if (oIterator.second->getWindowType() == CWindowType::LIST)
			{
				auto stSprite = dynamic_cast<CSpriteObject_List*>(oIterator.second);
				float top = stSprite->getWindow()->getActiveRect().top;
				float bottom = stSprite->getWindow()->getActiveRect().bottom;
			
				if (top >= m_pWindow->getActiveRect().top && bottom <= m_pWindow->getActiveRect().bottom)
				{
					oIterator.second->update();
				}
			}
		}
	}
}

void CSpriteObject_List::doDrawUI()
{
	CSpriteObject::doDrawUI();
	for (auto oIterator : m_oChildSpriteObjectList)
	{
		if (oIterator.second->getWindowType() == CWindowType::BUTTON)
		{
			auto stSprite = dynamic_cast<CSpriteObject_Button*>(oIterator.second);
			stSprite->getWindow()->createActiveRect();
			float top = stSprite->getWindow()->getActiveRect().top;
			float bottom = stSprite->getWindow()->getActiveRect().bottom;
		
			if (!(bottom<m_pWindow->getActiveRect().top || top>m_pWindow->getActiveRect().bottom))
			{
				oIterator.second->doDrawUI();
			}
		}
		if (oIterator.second->getWindowType() == CWindowType::CONTAINER)
		{
			auto stSprite = dynamic_cast<CSpriteObject_Container*>(oIterator.second);
			stSprite->getWindow()->createActiveRect();
			float top = stSprite->getWindow()->getActiveRect().top;
			float bottom = stSprite->getWindow()->getActiveRect().bottom;
		
			if (!(bottom<m_pWindow->getActiveRect().top || top>m_pWindow->getActiveRect().bottom))
			{
				oIterator.second->doDrawUI();
			}
		}
		if (oIterator.second->getWindowType() == CWindowType::LIST)
		{
			auto stSprite = dynamic_cast<CSpriteObject_List*>(oIterator.second);
			stSprite->getWindow()->createActiveRect();
			float top = stSprite->getWindow()->getActiveRect().top;
			float bottom = stSprite->getWindow()->getActiveRect().bottom;
		
			if (!(bottom<m_pWindow->getActiveRect().top || top>m_pWindow->getActiveRect().bottom))
			{
				oIterator.second->doDrawUI();
			}
		}
	}
}

void CSpriteObject_List::setVisible(bool a_bIsVisible)
{
	m_bIsVisible = a_bIsVisible;
	for (auto iter : m_oChildSpriteObjectList)
	{
		iter.second->setVisible(a_bIsVisible);
	}
}

void CSpriteObject_List::init(std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc, bool a_bIsChild, D3DXVECTOR3 a_rstRelativePos)
{
	D3DSURFACE_DESC pSurf;
	LPDIRECT3DTEXTURE9 pTex = m_oSpriteTexture[m_nTextureOffset];
	pTex->GetLevelDesc(0, &pSurf);
	m_pWindow = new CWindowList("WindowList", CWindowType::LIST, SIZE{ (LONG)pSurf.Width,(LONG)pSurf.Height }, getPosition());
	m_pWindow->init(a_pBeginCallBackFunc, a_pCallBackFunc, a_pEndCallBackFunc);
	m_bIsChild = a_bIsChild;
	m_stRelativePos = a_rstRelativePos;
	m_stListOffset = D3DXVECTOR3(0.0f,-m_pWindow->getActiveSize().cy / 2,0.0f);
	m_stMoveOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void CSpriteObject_List::release()
{
}

void CSpriteObject_List::addChildSpriteObject(std::string a_stSpriteName, CWindowType a_ECWindowType, CSpriteObject * a_pSpriteObject)
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
			float halfSizeY = pSpriteObject->getWindow()->getActiveSize().cy / 2;
			
			pSpriteObject->getRelativePos() = m_stListOffset + D3DXVECTOR3(0.0f, halfSizeY, 0.0f);
			m_oChildSpriteObjectList.push_back(decltype(m_oChildSpriteObjectList)::value_type(a_stSpriteName, a_pSpriteObject));

			m_stListOffset += D3DXVECTOR3(0.0f, halfSizeY * 2, 0.0f);
		}
		else if (a_ECWindowType == CWindowType::CONTAINER)
		{
			auto pSpriteObject = dynamic_cast<CSpriteObject_Container*>(a_pSpriteObject);
			float halfSizeY = pSpriteObject->getWindow()->getActiveSize().cy / 2;

			pSpriteObject->getRelativePos() = m_stListOffset + D3DXVECTOR3(0.0f, halfSizeY, 0.0f);
			m_oChildSpriteObjectList.push_back(decltype(m_oChildSpriteObjectList)::value_type(a_stSpriteName, a_pSpriteObject));

			m_stListOffset += D3DXVECTOR3(0.0f, halfSizeY * 2, 0.0f);
		}
		else if (a_ECWindowType == CWindowType::LIST)
		{
			auto pSpriteObject = dynamic_cast<CSpriteObject_List*>(a_pSpriteObject);
			float halfSizeY = pSpriteObject->getWindow()->getActiveSize().cy / 2;

			pSpriteObject->getRelativePos() = m_stListOffset + D3DXVECTOR3(0.0f, halfSizeY, 0.0f);
			m_oChildSpriteObjectList.push_back(decltype(m_oChildSpriteObjectList)::value_type(a_stSpriteName, a_pSpriteObject));

			m_stListOffset += D3DXVECTOR3(0.0f, halfSizeY * 2, 0.0f);
		}
	}
}

void CSpriteObject_List::deleteChildSpriteObject(std::string a_stSpriteName)
{
	auto oIter = std::find_if(m_oChildSpriteObjectList.begin(), m_oChildSpriteObjectList.end(), [=](std::pair<std::string, CSpriteObject*> obj)->bool {
		if (obj.first == a_stSpriteName)
			return true;
		else return false;
	});

	if (oIter != m_oChildSpriteObjectList.end())
	{
		float SizeY;
		if (oIter->second->getWindowType() == CWindowType::BUTTON)
		{
			auto pSpriteObject = dynamic_cast<CSpriteObject_Button*>(oIter->second);
			SizeY = pSpriteObject->getWindow()->getActiveSize().cy;
		}
		else if (oIter->second->getWindowType() == CWindowType::CONTAINER)
		{
			auto pSpriteObject = dynamic_cast<CSpriteObject_Container*>(oIter->second);
			SizeY = pSpriteObject->getWindow()->getActiveSize().cy;
		}
		else if (oIter->second->getWindowType() == CWindowType::LIST)
		{
			auto pSpriteObject = dynamic_cast<CSpriteObject_List*>(oIter->second);
			SizeY = pSpriteObject->getWindow()->getActiveSize().cy;
		}

		oIter = m_oChildSpriteObjectList.erase(oIter);
		while (oIter != m_oChildSpriteObjectList.end())
		{

			if (oIter->second->getWindowType() == CWindowType::BUTTON)
			{
				auto pSpriteObject = dynamic_cast<CSpriteObject_Button*>(oIter->second);
				D3DXVECTOR3 oldRefPos = pSpriteObject->getRelativePos();
				pSpriteObject->getRelativePos() = oldRefPos - D3DXVECTOR3(0.0f, SizeY, 0.0f);
			}
			else if (oIter->second->getWindowType() == CWindowType::CONTAINER)
			{
				auto pSpriteObject = dynamic_cast<CSpriteObject_Container*>(oIter->second);
				D3DXVECTOR3 oldRefPos = pSpriteObject->getRelativePos();
				pSpriteObject->getRelativePos() = oldRefPos - D3DXVECTOR3(0.0f, SizeY, 0.0f);
			}
			else if (oIter->second->getWindowType() == CWindowType::LIST)
			{
				auto pSpriteObject = dynamic_cast<CSpriteObject_List*>(oIter->second);
				D3DXVECTOR3 oldRefPos = pSpriteObject->getRelativePos();
				pSpriteObject->getRelativePos() = oldRefPos - D3DXVECTOR3(0.0f, SizeY, 0.0f);
			}
			oIter++;
		}

		m_stListOffset.y -= SizeY;
	}
}
