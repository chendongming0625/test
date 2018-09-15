#pragma once
#include "PageBaseUI.h"
#include "resource.h"
#include "PageManage.h"
#include "ServiceManage.h"
enum 
{
	STOPPED = 1,
	STARTING,
	STOPPING,
	RUNNING,
	CONTINUE,
	PAUSING,
	PAUSED

};
enum 
{
	ICON1=1,		//服务启动时的图标
	ICON2			//服务停止时的图标
};

class CPageBaseUI;
class CMainUI : public CPageBaseUI
{
public:
	CMainUI();
	~CMainUI();
	virtual void Close(UINT nRet = 1);

	void OnFinalMessage(HWND hWnd);
	void Notify(TNotifyUI &msg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();
	virtual void InitUI(int nShowID = 0, LPVOID lpParam = NULL);
	void SetEnabledBtn(bool bIsEnabledBtn);
private:
	CTrayIcon m_trayIcon;
	CMenuWnd *m_pMenu;
	CHorizontalLayoutUI *m_MainLayout;
	CButtonUI*m_pSaveButton;
	int m_nCurrentIconState;
};