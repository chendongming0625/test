#include "PageBaseUI.h"
#include "PageManage.h"

CPageBaseUI::CPageBaseUI()
{
	m_ePageType = CPageManage::GetInstance()->GetCurPage();
	auto &sPageInfo = CPageManage::GetInstance()->GetPageInfo();
	m_strXmlName = sPageInfo[m_ePageType].strXmlName;
	m_strPageName = sPageInfo[m_ePageType].strPageName;

	HWND hParent = NULL;
	CPageBaseUI* pParentPage = NULL;
	DWORD dwStyle = UI_WNDSTYLE_FRAME&(~WS_SYSMENU)&(~WS_MAXIMIZEBOX);

	if (sPageInfo[m_ePageType].eParentPage != PAGE_BEGIN
		&& sPageInfo[m_ePageType].eParentPage != PAGE_END)
	{
		pParentPage = sPageInfo[sPageInfo[m_ePageType].eParentPage].pPageBase;
		dwStyle |= UI_WNDSTYLE_CHILD;
		hParent = pParentPage ? pParentPage->GetHWND() : NULL;
	}
	this->Create(hParent, m_strPageName.c_str(), dwStyle, 0, 0, 0, 0, 0, NULL);
	this->ShowWindow();
	this->SetIcon(IDI_ICON1);
	if (hParent && pParentPage)
	{
		CControlUI* phor = static_cast<CControlUI*>(pParentPage->GetControl(TEXT("childPos")));
		if (phor)
		{
			auto pos = phor->GetFixedXY();
			::SetWindowPos(this->GetHWND(), HWND_TOP, pos.cx, pos.cy, phor->GetFixedWidth(), phor->GetFixedHeight(), SWP_SHOWWINDOW);
		}
	}
	else
	{
		this->CenterWindow();
	}
}
CPageBaseUI::~CPageBaseUI()
{

}

CDuiString CPageBaseUI::GetSkinFile()
{
	return CDuiString(m_strXmlName.c_str());
}

CDuiString CPageBaseUI::GetSkinFolder()
{
	return TEXT("");
}

UILIB_RESTYPE CPageBaseUI::GetResourceType() const
{
	return UILIB_FILE;
}

LPCTSTR CPageBaseUI::GetWindowClassName(void) const
{
	return m_strPageName.c_str();
}

void CPageBaseUI::Close(UINT nRet)
{
	__super::Close(nRet);
}

void CPageBaseUI::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

void CPageBaseUI::Notify(TNotifyUI & msg)
{
	return __super::Notify(msg);
}

LRESULT CPageBaseUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

CControlUI * CPageBaseUI::GetControl(wstring strControlName)
{
	return m_pm.FindControl(strControlName.c_str());
}

void CPageBaseUI::InitWindow()
{
}

void CPageBaseUI::InitUI(int nShowID, LPVOID lpParam)
{
	thread m_thread([] {
		Sleep(200);
		if (CPageManage::GetInstance()->GetPageInfo()[PAGE_MAIN].pPageBase != NULL)
		{
			CMainUI* pMainUI = (CMainUI*)CPageManage::GetInstance()->GetPageInfo()[PAGE_MAIN].pPageBase;
			pMainUI->SetEnabledBtn(true);
		}
	});

	m_thread.detach();
}
