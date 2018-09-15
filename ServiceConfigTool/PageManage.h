#pragma once
#include "PageBaseUI.h"
#include "MainUI.h"
#include "ServiceConfig.h"
#include "simpleLogger.h"
#include "Other.h"
#include "resource.h"
/***
页面信息
***/
typedef struct __page_info
{
	__page_info() {}
	__page_info(PAGE_TYPE ePageType, wstring strXmlName, wstring strPageName, PAGE_TYPE eParentPage = PAGE_BEGIN, CPageBaseUI *pPageBase = NULL);
	
	CPageBaseUI *pPageBase;
	PAGE_TYPE ePageType;
	wstring strXmlName;
	wstring strPageName;
	PAGE_TYPE eParentPage;
}PAGEINFO;


class CPageManage
{
public:
	static CPageManage*GetInstance();
	~CPageManage() {}
	map<PAGE_TYPE, PAGEINFO>&GetPageInfo();

	PAGE_TYPE GetCurPage();

	void ShowPage(PAGE_TYPE ePageType,bool bIsExit=0,LPVOID lParam=NULL);

	HWND GetCurHWND();

	HWND GetPreHWND();

	void ClosePage(PAGE_TYPE ePageType);

	void Reset();

	void InitPageInfo(PAGE_TYPE ePageType,CPageBaseUI *pPageBase=NULL);
private:
	CPageManage();
	
	map<PAGE_TYPE, PAGEINFO> m_sPageInfo;
	CPageBaseUI *m_pCurrentPage;
	PAGE_TYPE m_eCurrentShowPage;
	PAGE_TYPE m_ePrevShowPage;	//之前的页面
};
