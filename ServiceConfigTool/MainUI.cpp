#include "MainUI.h"
#define WM_CHEK_SERVICE    WM_USER + 520
#define WM_RESTART_SERVICE WM_USER + 521


CMainUI::CMainUI()
{
	m_pMenu = NULL;
	m_pSaveButton = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("save")));
}

CMainUI::~CMainUI()
{
	if (m_pMenu != NULL) {
		delete m_pMenu;
		m_pMenu = NULL;
	}
	KillTimer(this->GetHWND(), WM_CHEK_SERVICE);
	KillTimer(this->GetHWND(), WM_RESTART_SERVICE);
}

void CMainUI::Close(UINT nRet)
{
	__super::Close(nRet);
}

void CMainUI::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
}

void CMainUI::Notify(TNotifyUI & msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == TEXT("minbtn"))
		{
			::ShowWindow(this->GetHWND(),SW_MINIMIZE);
		}
		else if (msg.pSender->GetName() == TEXT("closebtn"))
		{
			::ShowWindow(m_hWnd, SW_HIDE);
		}
		else if (msg.pSender->GetName() == TEXT("serviceconfig_option"))
		{
			CServiceConfigUI*pServiceConfig = (CServiceConfigUI*)CPageManage::GetInstance()->GetPageInfo()[PAGE_SERVICECONFIG].pPageBase;
			if (pServiceConfig)
			{
				HWND hwnd = CPageManage::GetInstance()->GetCurHWND();
				if (hwnd)
				{
					::ShowWindow(hwnd, SW_HIDE);
				}
				::ShowWindow(pServiceConfig->GetHWND(), SW_SHOW);
				CPageManage::GetInstance()->ShowPage(PAGE_SERVICECONFIG,1);
				return;
			}	
			CPageManage::GetInstance()->ShowPage(PAGE_SERVICECONFIG);
		}
		else if (msg.pSender->GetName() == TEXT("other"))
		{
			COtherUI*pOther = (COtherUI*)CPageManage::GetInstance()->GetPageInfo()[PAGE_OTHER].pPageBase;
			if (pOther)
			{
				HWND hwnd = CPageManage::GetInstance()->GetCurHWND();
				if (hwnd)
				{
					::ShowWindow(hwnd, SW_HIDE);
				}
				::ShowWindow(pOther->GetHWND(), SW_SHOW);
				CPageManage::GetInstance()->ShowPage(PAGE_OTHER,1);
				return;
			}
			CPageManage::GetInstance()->ShowPage(PAGE_OTHER);
		}
		else if (msg.pSender->GetName() == TEXT("save"))
		{
			CServiceConfigUI *pServiceConfig = (CServiceConfigUI*)CPageManage::GetInstance()->GetPageInfo()[PAGE_SERVICECONFIG].pPageBase;
			if (pServiceConfig)
			{
				pServiceConfig->SaveConfig();
				DWORD sStatus = CServiceManege::GetInstance()->GetStatus(VAS_MANAGER);
				switch (sStatus)
				{
				case STOPPED:
					CMsgTip::ShowMsgTip(this->GetHWND(), m_pm.GetFont(4), L"保存成功", 2);
					break;
				case RUNNING:
				{
					m_pSaveButton->SetEnabled(false);
					CMsgTip::ShowMsgTip(this->GetHWND(), m_pm.GetFont(4), L"保存成功,正在重启服务...", 2);
					bool bIsFailed = CServiceManege::GetInstance()->Stop(VAS_MANAGER);
					if (!bIsFailed)
					{
						SetTimer(this->GetHWND(), WM_RESTART_SERVICE, 1000, 0);
					}
					else
					{
						MessageBox(NULL, TEXT("重启服务失败"), TEXT("提示"), MB_OK);
					}
					break;
				}	
				default:
					break;
				}
			}
			else
			{
				LOG("save failed pServiceConfig is NULL\n");
			}
		}
	}
}

LRESULT CMainUI::OnDestroy(UINT, WPARAM, LPARAM, BOOL & bHandled)
{
	m_trayIcon.DeleteTrayIcon();
	bHandled = FALSE;
	// 退出程序
	PostQuitMessage(0);
	return 0;
}

LRESULT CMainUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_TIMER:
		if (wParam == WM_CHEK_SERVICE)
		{
			DWORD sStatus = CServiceManege::GetInstance()->GetStatus(VAS_MANAGER);
			switch (sStatus)
			{
			case STOPPED:
				if (m_nCurrentIconState != ICON2)
				{
					this->SetIcon(IDI_ICON2);
					m_trayIcon.SetIcon(IDI_ICON2);
					m_nCurrentIconState = ICON2;
				}
				break;
			case RUNNING:
				if (m_nCurrentIconState != ICON1)
				{
					this->SetIcon(IDI_ICON1);
					m_trayIcon.SetIcon(IDI_ICON1);
					m_nCurrentIconState = ICON1;
				}
				break;
			default:
				this->SetIcon(IDI_ICON2);
				m_trayIcon.SetIcon(IDI_ICON2);
				m_nCurrentIconState = ICON2;
				break;
			}
		}
		else if (wParam == WM_RESTART_SERVICE)
		{
			DWORD sStatus = CServiceManege::GetInstance()->GetStatus(VAS_MANAGER);
			switch (sStatus)
			{
			case STOPPED:
				CServiceManege::GetInstance()->Start(VAS_MANAGER);
				KillTimer(this->GetHWND(), WM_RESTART_SERVICE);
				m_pSaveButton->SetEnabled(true);
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	// 关闭窗口，退出程序
	if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	else if (uMsg == UIMSG_TRAYICON)
	{
		UINT uIconMsg = (UINT)lParam;
		if (uIconMsg == WM_LBUTTONUP || uIconMsg == WM_LBUTTONDBLCLK) {
			BOOL bVisible = IsWindowVisible(m_hWnd);
			if (!bVisible)
				::ShowWindow(m_hWnd, SW_SHOW);
		}
		else if (uIconMsg == WM_RBUTTONUP) {
			if (m_pMenu != NULL) {
				delete m_pMenu;
				m_pMenu = NULL;
			}
			m_pMenu = new CMenuWnd();
			CDuiPoint point;
			::GetCursorPos(&point);
			point.x += 10;
			point.y -= 135;
			m_pMenu->Init(NULL, _T("menu.xml"), point, &m_pm);
			m_pMenu->ResizeMenu();
			DWORD sStatus = CServiceManege::GetInstance()->GetStatus(VAS_MANAGER);
			CMenuUI*rootMenu = m_pMenu->GetMenuUI();
			CMenuElementUI*pStart = static_cast<CMenuElementUI*>(rootMenu->FindSubControl(TEXT("start")));
			CMenuElementUI*pStop = static_cast<CMenuElementUI*>(rootMenu->FindSubControl(TEXT("stop")));
			switch (sStatus)
			{
			case STOPPED:
				pStart->SetEnabled(true);
				pStop->SetEnabled(false);
				pStop->SetText(L"停止");
				break;
			case STARTING:
				pStart->SetEnabled(false);
				pStart->SetText(L"正在启动");
				break;
			case STOPPING:
				pStop->SetEnabled(false);
				pStart->SetEnabled(false);
				pStop->SetText(L"正在停止");
				break;
			case RUNNING:
				pStart->SetEnabled(false);
				pStart->SetText(L"正在运行");
				pStop->SetText(L"停止");
				pStop->SetEnabled(true);
				break;
			case CONTINUE:

				break;
			case PAUSING:

				break;
			default:
				break;
			}
		}
	}
	else if (uMsg == WM_MENUCLICK)
	{
		MenuCmd *pMenuCmd = (MenuCmd*)wParam;
		if (pMenuCmd)
		{
			BOOL bChecked = pMenuCmd->bChecked;
			CDuiString sMenuName = pMenuCmd->szName;
			m_pm.DeletePtr(pMenuCmd);

			if (sMenuName.CompareNoCase(_T("setting")) == 0)
			{
				BOOL bVisible = IsWindowVisible(m_hWnd);
				if(!bVisible)
					::ShowWindow(m_hWnd,  SW_SHOW);
			}
			else if (sMenuName.CompareNoCase(_T("exit")) == 0)
			{
				Close(0);
			}
			else if (sMenuName.CompareNoCase(_T("start")) == 0)
			{
				if (m_pMenu)
				{
					CMenuUI*rootMenu = m_pMenu->GetMenuUI();
					CMenuElementUI*pStart = static_cast<CMenuElementUI*>(rootMenu->FindSubControl(TEXT("start")));
					CMenuElementUI*pStop = static_cast<CMenuElementUI*>(rootMenu->FindSubControl(TEXT("stop")));
					pStart->SetEnabled(false);
					bool bIsSuccess=CServiceManege::GetInstance()->Start(VAS_MANAGER);
					if (!bIsSuccess&&m_nCurrentIconState != ICON1)//0表示成功
					{
						this->SetIcon(IDI_ICON1);
						m_trayIcon.SetIcon(IDI_ICON1);
						m_nCurrentIconState = ICON1;
					}
					else
					{
						MessageBox(NULL,TEXT("服务启动失败"),TEXT("提示"),MB_OK);
					}
				}	
			}
			else if (sMenuName.CompareNoCase(_T("stop")) == 0)
			{
				if (m_pMenu)
				{
					CMenuUI*rootMenu = m_pMenu->GetMenuUI();
					CMenuElementUI*pStart = static_cast<CMenuElementUI*>(rootMenu->FindSubControl(TEXT("start")));
					CMenuElementUI*pStop = static_cast<CMenuElementUI*>(rootMenu->FindSubControl(TEXT("stop")));
					pStop->SetEnabled(false);
					bool bIsSuccess = CServiceManege::GetInstance()->Stop(VAS_MANAGER);
					if (!bIsSuccess&&m_nCurrentIconState!=ICON2)
					{
						this->SetIcon(IDI_ICON2);
						m_trayIcon.SetIcon(IDI_ICON2);
						m_nCurrentIconState = ICON2;
					}
					else
					{
						MessageBox(NULL, TEXT("停止服务失败"), TEXT("提示"), MB_OK);
					}
				}
				
			}

		}
	}
}

void CMainUI::InitWindow()
{
	
}

void CMainUI::InitUI(int nShowID, LPVOID lpParam)
{
	m_trayIcon.CreateTrayIcon(this->GetHWND(), IDI_ICON1, _T("配置工具"));
	m_nCurrentIconState = ICON1;
	__super::InitUI(nShowID,lpParam);
	SetTimer(this->GetHWND(),WM_CHEK_SERVICE,3000,0);
}

void CMainUI::SetEnabledBtn(bool bIsEnabledBtn)
{
	if(m_MainLayout)
		m_MainLayout->SetEnabled(bIsEnabledBtn);
}
