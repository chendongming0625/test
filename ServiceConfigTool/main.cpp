#include <iostream>
#include <string>
#include <windows.h>
#include "SimpleLogger.h"
#include "MainUI.h"
#include "PageManage.h"


/*1.0.0 date:2018 5
初版*/

/*1.0.1 date:2018 6.3

1.增加标志位修改图标前先对标志位进行判断 
2.修复执行程序时间过长时导致崩溃问题   
3.点击托盘图标时若界面在显示状态则break*/

/*1.0.2 date:2018 7.6
1.修改配置文件的路径
2.修改窗口名为英文 中文由于编码格式可能会找不到
*/

#define Version "1.0.2"  
HANDLE hClient = NULL;
void InitResource();
BOOL IsExistsClientMutex();
BOOL CreateClientMutex();
void CloseClientMutex();


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	LOG("Version=%s\n", Version);
	if (IsExistsClientMutex())
	{
		HWND hwnd = ::FindWindow(L"Vastool",NULL);
		if (IsWindow(hwnd))
		{
			LOG("FindWindow success\n");
			BOOL bVisible = IsWindowVisible(hwnd);
			if(!bVisible)
				::ShowWindow(hwnd, SW_SHOW);
			return 0;		
		}
		else
		{
			LOG("FindWindow failed..\n");
			return -1;
		}
		
	}
	::CoInitialize(NULL);
	CPaintManagerUI::SetResourceType(UILIB_FILE);
	CPaintManagerUI::SetInstance(GetModuleHandle(NULL));
	InitResource();
	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + L"skin\\");
	CPageManage::GetInstance()->ShowPage(PAGE_SERVICECONFIG);
	CPaintManagerUI::MessageLoop();
	CloseClientMutex();
	::CoUninitialize();

	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//初始化资源
void InitResource()
{
#ifndef _DEBUG
	CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
	HRSRC hResource = ::FindResource((CPaintManagerUI::GetInstance()), MAKEINTRESOURCE(IDR_ZIPRES1), _T("ZIPRES"));
	if (hResource != NULL)
	{
		DWORD dwSize = 0;
		HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetInstance(), hResource);
		if (hGlobal != NULL)
		{
			dwSize = ::SizeofResource(CPaintManagerUI::GetInstance(), hResource);
			if (dwSize > 0)
			{
				CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
			}
		}
		::FreeResource(hResource);
	}
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
#else
	CPaintManagerUI::SetResourceType(UILIB_FILE);
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath() + _T("Skin\\");
#endif
	CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
}

////////////////////////////////////////////////////////////////是否已经存在互斥对象
BOOL IsExistsClientMutex()
{
	char szMutexName[64] = "Vastool";
	HANDLE hMutex = OpenMutexA(NULL, FALSE, szMutexName);
	int nError = GetLastError();
	if (nError == ERROR_ALREADY_EXISTS || nError == ERROR_ACCESS_DENIED)
	{
		//LOG("IsExistsClientMutex, hMutex: %x, error: %x\n", hMutex, GetLastError());
		return TRUE;
	}
	else
	{
		CloseHandle(hMutex);
		//LOG("IsExistsClientMutex, hMutex: %x, error: %x\n", hMutex, GetLastError());
		CreateClientMutex();
		return FALSE;
	}
	
}
BOOL CreateClientMutex()
{
	char szMutexName[64] = "Vastool";
	hClient = CreateMutexA(NULL, TRUE, szMutexName);
	int nError = GetLastError();
	if (nError == ERROR_ALREADY_EXISTS || nError == ERROR_ACCESS_DENIED)
	{
		//LOG("CreateClientMutex, hMutex: %x, error: %x\n", hMutex, GetLastError());
		return FALSE;
	}
	else
	{
		//LOG("CreateClientMutex, hMutex: %x, error: %x\n", hMutex, GetLastError());
		return TRUE;
	}
}
void CloseClientMutex()
{
	ReleaseMutex(hClient);
	CloseHandle(hClient);
}
