#include "PageManage.h"

CPageManage * CPageManage::GetInstance()
{
	static CPageManage page;
	return &page;
}

map<PAGE_TYPE, PAGEINFO>& CPageManage::GetPageInfo()
{
	return m_sPageInfo;
}

PAGE_TYPE CPageManage::GetCurPage()
{
	return m_eCurrentShowPage;
}

void CPageManage::ShowPage(PAGE_TYPE ePageType, bool bIsExit, LPVOID lParam)
{
	if (ePageType == m_eCurrentShowPage
		|| ePageType == PAGE_BEGIN
		|| ePageType == PAGE_END)
		return;
	if (bIsExit)
	{
		m_ePrevShowPage = m_eCurrentShowPage;
		m_eCurrentShowPage = ePageType;
		return;
	}
	auto &pPageBase = m_sPageInfo[ePageType].pPageBase;
	if (pPageBase)
		return;

	if (m_sPageInfo[ePageType].eParentPage != PAGE_BEGIN
		&& m_sPageInfo[ePageType].eParentPage != PAGE_END)	//如果有父窗口
	{
		if (NULL == m_sPageInfo[m_sPageInfo[ePageType].eParentPage].pPageBase)
		{
			//LOG("ShowPage parentPage...\n");
			this->ShowPage(m_sPageInfo[ePageType].eParentPage);		//父窗口未创建  则先创建父窗口
			m_eCurrentShowPage = m_ePrevShowPage;
		}
	}

	if (this->GetPageInfo()[PAGE_MAIN].pPageBase != NULL)
	{
		CMainUI* pMainUI = (CMainUI*)this->GetPageInfo()[PAGE_MAIN].pPageBase;
		pMainUI->SetEnabledBtn(false);
	}

	m_ePrevShowPage = m_eCurrentShowPage;
	//this->ClosePage(m_ePrevShowPage);
	m_eCurrentShowPage = ePageType;

	switch (ePageType)
	{
	case PAGE_MAIN:
		pPageBase = new CMainUI();
		break;
	case PAGE_SERVICECONFIG:
		pPageBase = new CServiceConfigUI();
		break;
	case PAGE_OTHER:
		pPageBase = new COtherUI();
		break;
	case PAGE_END:
		break;
	default:
		break;
	}
	if(pPageBase)
		pPageBase->InitUI(bIsExit,lParam);
}

HWND CPageManage::GetCurHWND()
{
	if (m_sPageInfo.find(m_eCurrentShowPage) == m_sPageInfo.end())
		return NULL;
	if (m_sPageInfo[m_eCurrentShowPage].pPageBase)
		return m_sPageInfo[m_eCurrentShowPage].pPageBase->GetHWND();
	else
		return NULL;
}

HWND CPageManage::GetPreHWND()
{
	if (m_sPageInfo.find(m_ePrevShowPage) == m_sPageInfo.end())
		return NULL;
	if (m_sPageInfo[m_ePrevShowPage].pPageBase)
		return m_sPageInfo[m_ePrevShowPage].pPageBase->GetHWND();
	else
		return NULL;
}

void CPageManage::ClosePage(PAGE_TYPE ePageType)
{
	if (m_sPageInfo.find(ePageType) == m_sPageInfo.end())
		return;

	if (m_sPageInfo[ePageType].pPageBase)
	{
		m_sPageInfo[ePageType].pPageBase->Close();
		m_sPageInfo[ePageType].pPageBase = NULL;
	}
}
void CPageManage::Reset()
{
	for (size_t i = PAGE_BEGIN; i < PAGE_END; ++i)
	{
		if (m_sPageInfo[(PAGE_TYPE)i].pPageBase)
		{
			m_sPageInfo[(PAGE_TYPE)i].pPageBase->Close();
			m_sPageInfo[(PAGE_TYPE)i].pPageBase = NULL;
		}
	}
	LOG("CPageManage::Reset()...\n");
}
CPageManage::CPageManage()
{
	for (int i = PAGE_BEGIN; i < PAGE_END; ++i)
	{
		InitPageInfo((PAGE_TYPE)i);
	}
	m_eCurrentShowPage = PAGE_BEGIN;
}
void CPageManage::InitPageInfo(PAGE_TYPE ePageType, CPageBaseUI * pPageBase)
{
	wstring strXmlName = TEXT("");
	wstring strPageName = TEXT("");
	PAGE_TYPE  eParentPage = PAGE_BEGIN;
	switch (ePageType)
	{
	case PAGE_MAIN:
		strXmlName = TEXT("MainUI.xml");
		//strPageName = TEXT("配置工具");
		strPageName = TEXT("Vastool");//modified by cdm,There are problems with Chinese coding
		break;
	case PAGE_SERVICECONFIG:
		eParentPage = PAGE_MAIN;
		strXmlName = TEXT("ServiceConfigUI.xml");
		strPageName = TEXT("服务器配置");
		break;
	case PAGE_OTHER:
		eParentPage = PAGE_MAIN;
		strXmlName = TEXT("other.xml");
		strPageName = TEXT("其他配置");
		break;
	default:
		break;
	}
	m_sPageInfo[ePageType] = PAGEINFO(ePageType,strXmlName,strPageName,eParentPage,pPageBase);
}

__page_info::__page_info(PAGE_TYPE ePageType, wstring strXmlName, wstring strPageName, PAGE_TYPE eParentPage, CPageBaseUI * pPageBase)
{
	this->ePageType = ePageType;
	this->strXmlName = strXmlName;
	this->strPageName = strPageName;
	this->pPageBase = pPageBase;
	this->eParentPage = eParentPage;
}
